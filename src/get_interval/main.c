/**
 * This programs reads from 1 or more binary data file and a corresponding binary timestamps file,
 * and prints to the output each and every intervals of time where data values are between 
 * user-defined upper and lower bounds. Only intervals with length greater than user-defined
 * threshold value will be display, but all intervals are going to be included in the summary
 * section as part of the total statistics.
 *
 * Cautions:
 *  - the data files need to be encoded as doubles
 *  - the timestampe file needs to be encoded as 64-bit unsigned integers, using epoch time
 *  - each of the data file needs to have the same length as the timestamp file
 *  - the upper bound needs to be greater of equal to the lower bound
 *
 * main.c (get_interval)
 * Author: Kalettuce
 */

#include "extio.h"
#include "get_interval.h"

int main(int argc, char *argv[]) {
    // argument check
    if (argc < 6) {
        fprintf(stderr, "Usage: analyze LOW HIGH threshold ul_ts_filename infilename...\n");
        return -1;
    }

    /******************************** ARGUMENT LOADING AND CHECKS **************************/
    // a temp reading needs to be between lower & upper bounds to be included in a interval
    double lower_bound, upper_bound;
    sscanf(argv[1], "%lf", &lower_bound);
    sscanf(argv[2], "%lf", &upper_bound);
    printf("Lower bound: %.4f / Upper bound: %.4f\n", lower_bound, upper_bound);
    if (lower_bound > upper_bound) {
        fprintf(stderr, "Your lower bound is greater than the upper bound.\n");
        return 3;
    }

    // if the length of a interval > threshold, display the statistics for it
    double threshold;
    sscanf(argv[3], "%lf", &threshold);
    if (threshold < 0) {
        fprintf(stderr, "Argument error: the threshold needs to be at least 0\n");
        return 4;
    }

    // set up timestamp file
    FILE *tsfile = fopen(argv[4], "r");
    if (!tsfile) {
        fprintf(stderr, "Cannot access timestamps file (%s).\n", argv[2]);
        return 2;
    }

    // set up input file(s)
    size_t infile_count = argc - 5;
    FILE *inputs[infile_count];
    for (size_t i = 0; i < infile_count; i++) { 
        inputs[i] = fopen(argv[5+i], "r");
        if (!inputs[i]) {
            fprintf(stderr, "Cannot access input file (%s).\n", argv[1]);
            fclose_all(inputs, i);
            fclose(tsfile);
            return 1;
        }
    }

    /***************************** END OF ARGUMENT LOADING AND CHECKS **************************/

    // iterating through all of the input files
    for (uint32_t i = 0; i < infile_count; i++) {
        printf("Results of %s:\n", argv[5+i]);
        processFile(inputs[i], tsfile, lower_bound, upper_bound, threshold);
    }

    // yo-hoo!
    printf("Analysis complete.\n");
    
    // housekeeping
    fclose_all(inputs, infile_count);
    fclose(tsfile);

    return 0;
}
