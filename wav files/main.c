#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wav.h"

int main(int argc, char **argv) {
    // Get file path
    if (argc != 2) {
        printf("Error: invalid amount of arguments\n");
        return -1;
    }
    char *filePath = malloc(strlen(argv[1]) + 1);
    if(filePath == NULL) {
        printf("Error: Memory alocation failed\n");
        return -1;
    }
    strcpy(filePath, argv[1]);

    // Create file pointer
    FILE *file = fopen(filePath, "r");
    if (file == NULL) {
        printf("Error: unable to open file\n");
        free(filePath);
        return -1;
    }
    /******************/
    /*    TESTING     */
    /******************/

    // print values (testing)
    printData(file, 1);

    // close file and free resources
    if (fclose(file) != 0) {
        printf("Error: Failed to close file\n");
        return -1;
    }
    free(filePath);
}