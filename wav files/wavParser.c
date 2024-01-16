#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wav.h"

/*******************/
/* HELPER FUNCTONS */
/*******************/

/// @brief Seeks to a certain position in bytes within the file.
/// @param position (int) position (in bytes) to seek to.
/// @param file (FILE *) file to perform seek on.
/// @return (int) 0 if failed, 1 if successful.
int seekTo(int position, FILE *file) {
    if (fseek(file, position, SEEK_SET) != 0) {
        printf("Error: unable to seek on file.\n");
        return 0;
    }
    return 1;
}

/// @brief Seeks to the data section of a wave file. Also returns important information to read file.
/// @param file (FILE *) file to read data from
/// @param numChannels (short *) Memory address  to put the number of channels in.
/// @param sampleSize (short *) Memory address to put the sample size in (in bytes)
/// @return (int) block align of the wave file given
short readData(FILE *file, short *numChannels, short *sampleSize) {
    *numChannels = getNumberOfChannels(file);
    *sampleSize = getBitsPerSample(file) / 8;
    short blockAlign = getBlockAlign(file);
    if (seekTo(44, file) == 0) { return 0; }
    return blockAlign;
}

/// @brief Prints general data (format, number of channels, bytes per sample, sample rate, number of samples)
///        Also prints the raw data of the wave file if rawData flag is setto 1.
/// @param file (FILE *) File to print data from.
/// @param rawData (char) Flag to indicate if function should print raw data.
/// @return (int) returns -1 for an error, 1 otherwise.
int printData(FILE *file, char rawData) {
    // call readData
    short sampleSize, numChannels;
    short blockAlign = readData(file, &numChannels, &sampleSize);
    if (blockAlign == 0) {
        printf("Error: unable to read from file");
        return -1;
    }
    // print raw data
    if(rawData == 1) {
        // create memory location for data
        char *data = (char *)malloc(blockAlign);
        if (data == NULL) {
            printf("Error: unable to open file\n");
            free(data);
            return -1;
        }
        while (fread(data, blockAlign, 1, file) != 0) {
            int16_t signedValue = *(int16_t *)data;
            printf("%x\t", signedValue);
        }
        free(data);
    }
    // print rest of data
    printf("\n\nFormat: %s\nNumber of channels: %hd\nBytes per sample: %hd\nSample rate: %i\nNumber of Samples: %i\n\n", getFormat(file), numChannels, sampleSize, getSampleRate(file), getNumberOfSamples(file));

    return 1;
}

/*******************/
/* GETTER FUNCTONS */
/*******************/

/// @brief returns the chunk ID of the wav file given
/// @param file (FILE *) file to get chunk ID frim
/// @return (string) chunk ID (should be "RIFF")
char *getChunkID(FILE *file) {
    char *chunkID = malloc(5);
    if (chunkID == NULL) {
        printf("Error: Memory alocation failed\n");
        return NULL;
    }
    fread(chunkID, 1, 4, file);
    chunkID[4] = '\0';
    return chunkID;
}

/// @brief returns the chunk size of the wav file given
/// @param file (FILE *) file to get chunk size frim
/// @return (int) chunk size in bytes
int getChunkSize(FILE *file) { 
    int chunkSize;
    if (seekTo(4, file) == 0) { return -1; }
    fread(&chunkSize, 4, 1, file);
    return chunkSize;
}

/// @brief returns the format of the file given
/// @param file (FILE *) file to get format from
/// @return (string) format (should be "WAVE")
char *getFormat(FILE *file) { 
    char *format = malloc(5);
    if (format == NULL) {
        printf("Error: Memory alocation failed\n");
        return NULL;
    }
    if (seekTo(8, file) == 0) { return NULL; }
    fread(format, 1, 4, file);
    return format;
}

/// @brief returns the audio format of the given file
/// @param file (FILE *) file to get audio format from
/// @return (short) audio format (should be 1. If not, indicates compression)
short getAudioFormat(FILE *file) {
    short audioFormat;
    if (seekTo(20, file) == 0) { return -1; }
    fread(&audioFormat, 2, 1, file);
    return audioFormat;
}

/// @brief returns the amount of channels in the file
/// @param file (FILE *) file to get the amount of channels from
/// @return (short) number of channels (1 means mono, 2 means stereo)
short getNumberOfChannels(FILE *file) {
    short numberOfChannels;
    if (seekTo(22, file) == 0) { return -1; }
    fread(&numberOfChannels, 2, 1, file);
    return numberOfChannels;
}

/// @brief returns the sample rate of the given wave file
/// @param file (FILE *) file to get sample rate from
/// @return (int) sample rate 
int getSampleRate(FILE *file) {
    int sampleRate;
    if (seekTo(24, file) == 0) { return -1; }
    fread(&sampleRate, 4, 1, file);
    return sampleRate;
}

/// @brief returns the byte rate of the given wave file
/// @param file (FILE *) file to get byte rate from
/// @return (int) byte rate (sampleRate * numChannels * bitsPerSample/8)
int getByteRate(FILE *file) {
    int byteRate;
    if (seekTo(28, file) == 0) { return -1; }
    fread(&byteRate, 4, 1, file);
    return byteRate;
}

/// @brief returns the number of bytes per sample
/// @param file (FILE *) file to get the block align from
/// @return (short) number of bytes in one sample (numChannels * bitesPerSample/8)
short getBlockAlign(FILE *file) {
    short blockAlign;
    if (seekTo(32, file) == 0) { return -1; }
    fread(&blockAlign, 2, 1, file);
    return blockAlign;
}

/// @brief retruns the amount of bits in one sample (not including amount of channels)
/// @param file (FILE *) file to get the bits per sample rate from
/// @return (short) amount of bits in one sample
short getBitsPerSample(FILE *file) {
    short bitsPerSample;
    if (seekTo(34, file) == 0) { return -1; }
    fread(&bitsPerSample, 2, 1, file);
    return bitsPerSample;
}

/// @brief returns size of the entire actual data in bytes
/// @param file (FILE *) file to get the size of the data from
/// @return amount of bytes the actual data is
int getDataChunkSize(FILE *file) {
    int dataChunkSize;
    if (seekTo(40, file) == 0) { return -1; }
    fread(&dataChunkSize, 4, 1, file);
    return dataChunkSize;
}

/// @brief gets the number of samples in the raw data of the wav file
/// @param file file to get number of samples from
/// @return (int) amount of samples in the given file (dataChunkSize / blockAlign)
int getNumberOfSamples(FILE *file) {
    return (getDataChunkSize(file) / (int)getBlockAlign(file));
}

/*********************/
/* COMPLEX FUNCTIONS */
/*********************/