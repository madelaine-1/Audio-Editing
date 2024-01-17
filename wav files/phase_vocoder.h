#include <stdio.h>

int getSamplesPerFrame(int miliseconds, int sampleRate);
int getFrame(int sampleNumber, int samplesPerFrame, short sampleSize, FILE *file, void *frame);