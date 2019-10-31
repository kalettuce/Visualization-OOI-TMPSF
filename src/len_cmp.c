/**
 * This program compares the length of 2 files in terms of number of 8-bytes, and prints to the
 * output the count for each of the files
 *
 * Author: Kalettuce
 */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    // input file setup
    FILE *f1 = fopen(argv[1], "r");
    if (!f1) {
        fprintf(stderr, "file %s not found or insufficient privilege.\n", argv[1]);
        return 1;
    }

    FILE *f2 = fopen(argv[2], "r");
    if (!f2) {
        fprintf(stderr, "file %s not found or insufficient privilege.\n", argv[2]);
        return 2;
    }

    // counter declarations and buffers to read in
    unsigned long count1 = 0, count2 = 0;
    void *bf1 = malloc(sizeof(double)), *bf2 = malloc(sizeof(double));

    // main execution of the program to deplete the file content
    while (fread(bf1, sizeof(double), 1, f1)) {
        count1++;
    }

    while (fread(bf2, sizeof(double), 1, f2)) {
        count2++;
    }

    // housekeeping
    free(bf1);
    free(bf2);
    fclose(f1);
    fclose(f2);
    
    // printing result to stdin
    printf("8-byte data count in %s: %lu\n8-byte data count in %s: %lu\n",
            argv[1], count1, argv[2],  count2);

    return 0;
}
