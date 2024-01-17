#include <stdio.h>
#include "phase_vocoder.h"

/*******************/
/* GETTER FUNCTONS */
/*******************/
int getSamplesPerFrame(int miliseconds, int sampleRate) {
    return (sampleRate / 1000) * miliseconds;
}

/**
 * @brief Gives the data of the (sampleNumber)th sample and puts the data in frame.
 *
 * @param sampleNumber (int) The index of the sample to retrieve.
 * @param samplesPerFrame (int) The number of samples in a single frame.
 * @param sampleSize (short) The number of bytes one sample should be.
 * @param file (FILE *) The file to read data from.
 * @param frame (void *) Points to where the frame data should be put.
 *
 * @return 0: Successful
 *         1: Reached end of file
 *         2: Error seeking on file
 *         3: Error reading from file
 */
int getFrame(int sampleNumber, int samplesPerFrame, short sampleSize, FILE *file, void *frame) {
    int index = samplesPerFrame * sampleNumber / 2;
    
    if (fseek(file, 44 + (sampleSize * index), SEEK_SET) != 0) {
        printf("Error: unable to seek on file.\n");
        return 2;
    }
    
    size_t bytesRead = fread(frame, sampleSize, samplesPerFrame, file);
    
    if (bytesRead != (size_t)samplesPerFrame) {
        if (feof(file) != 0){ return 1; }
        else{
            printf("Error: invalid number of Samples"); 
            return 3;
        }
    }
    return 0;
}