/**
 * This program takes a binary file containing OOI-style timestamps and convert it to epoch-style.
 * The file needs to be encoded as double precision floats.
 *
 * to_epoch.c
 * Author: Kalettuce
 */

#include <stdio.h>

/* definition of the differnce between OOI timestamps and epoch timestamps */
#define EPOCH_SCI_DIFF 2208988800.0

int main(int argc, char **argv) {

    // argument check
    if (argc != 3) {
        fprintf(stderr, "Usage: to_epoch infilename outfilename\n");
        return -1;
    }

    // set up io files and container
    FILE *input = fopen(argv[1], "r");

    if (!input) {
        fprintf(stderr, "Input file (%s) does not exist\n", argv[1]);
        return 1;
    }

    FILE *output = fopen(argv[2], "w");

    if (!output) {
        fprintf(stderr, "Insufficient privilege to access/create output file (%s).\n", argv[2]);
        return 2;
    }

    double buffer;
    unsigned long counter = 0;

    // traversing the data file
    while (fread(&buffer, sizeof(double), 1, input)) {
        buffer -= EPOCH_SCI_DIFF;
        fwrite(&buffer, sizeof(double), 1, output);
        counter++;
    }

    // see if the entire input is traversed
    if (feof(input)) {
        printf("Operation completed. Converted %lu values.\n", counter);
    } else {
        fprintf(stderr, "Unexpected problem encountered. Exitting.\n");
    }

    // housekeeping
    fclose(input);
    fclose(output);
    
    return 0;
}
