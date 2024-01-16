#include <stdio.h>

typedef struct {
    char chunkID[4];
    int chunkSize;
    char format[4];
    char subchunkID[4];
    int subchunkSize;
    short audioFormat;
    short numChannels;
    int sampleRate;
    int byteRate;
    short blockAlign;
    short bitsPerSample;
} WavHeader;

//getters for file elements
char *getChunkID(FILE *file);
int getChunkSize(FILE *file);
char *getFormat(FILE *file);
short getAudioFormat(FILE *file);
short getNumberOfChannels(FILE *file);
int getSampleRate(FILE *file);
int getByteRate(FILE *file);
short getBlockAlign(FILE *file);
short getBitsPerSample(FILE *file);
int getDataChunkSize(FILE *file);
int getNumberOfSamples(FILE *file);

short readData(FILE *file, short *numChannels, short *sampleSize);
int printData(FILE *file, char rawData);

