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
 * analyze.c
 * Author: Kalettuce
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <wchar.h>

/* definitions of the how the time strings are formatted, and a corresponding buffer length */
#define TIME_FORMAT "%Y/%m/%d %H:%M"
#define TIME_STR_LEN 26

// print the result for one interval
void printIntervalResult(const uint64_t interval_num, char *begin_time_s, char *end_time_s,\
                         uint64_t begin_ts, uint64_t end_ts, uint64_t ins_count);

// close all files pointed to by **files
void fclose_all(FILE **files, size_t f_count);

// process an single data file against a timestamp file
void processFile(FILE *values, FILE *timestamps,\
                 const double lower, const double upper, const double threshold);

int main(int argc, char *argv[]) {
    // argument check
    if (argc < 6) {
        fprintf(stderr, "Usage: temp_analyze LOW HIGH threshold ul_ts_filename infilename...\n");
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

void processFile(FILE *values, FILE *timestamps,\
                 const double lower, const double upper, const double threshold) {
    // save the original file position to restore to
    fpos_t ts_pos;
    fpos_t val_pos;
    fgetpos(values, &val_pos);
    fgetpos(timestamps, &val_pos);
    fseek(timestamps, 0, SEEK_SET);
    fseek(values, 0, SEEK_SET);

    // set up containers
    double temp_reading;
    // the begin time, end time, data instances of each interval and the interval count
    time_t begin_ts, end_ts;
    uint64_t ins_count = 1, duration, total_duration = 0, total_ins = 0,\
             total_data_count = 0, interval_count = 0;
    uint64_t timestamp_buf; // to read unimportant timestamps into
    // used as a boolean to determine if the stream pointer is in an interval
    uint8_t in_interval = 0;

    char *begin_time_s = malloc(TIME_STR_LEN);
    char *end_time_s = malloc(TIME_STR_LEN);
    
    
    // traversing through the input file
    while (fread(&temp_reading, sizeof(double), 1, values) && !feof(timestamps)) {
        total_data_count++;
        // if temp_reading is within range
        if (temp_reading >= lower && temp_reading <= upper) {
            if (!in_interval) {
                in_interval= 1;
                // read timestamp as begin time
                fread(&begin_ts, sizeof(uint64_t), 1, timestamps);
            } else {
                ins_count++;
                // push the stream pointer in timestamps 1 value back
                fread(&timestamp_buf, sizeof(uint64_t), 1, timestamps);
            }
        } else { // temp_reading not within range
            if (in_interval) {
                in_interval = 0;

                // read timestamp as end time
                fread(&end_ts, sizeof(uint64_t), 1, timestamps);
                // convert time to string
                strftime(begin_time_s, TIME_STR_LEN, TIME_FORMAT, gmtime(&begin_ts));
                strftime(end_time_s, TIME_STR_LEN, TIME_FORMAT, gmtime(&end_ts));
                duration = end_ts - begin_ts;

                // print if interval lasts longer threshold
                if (threshold * 3600 * 24 <= duration) {
                    printIntervalResult(++interval_count, begin_time_s,
                                        end_time_s, begin_ts, end_ts, ins_count);
                }

                // update "total" variables
                total_ins += ins_count;
                total_duration += duration;
                // reset individual counters
                ins_count = 1;
                duration = 0;
            } else {
                // synchronize file streams
                fread(&timestamp_buf, sizeof(uint64_t), 1, timestamps);
            }
        }
    }

    // offset the timestamps by 1, since when the while condition evaluated for the last time, fread
    // is executed for the input but not the timestamps. Also, if still in interval, print it out
    // with the read end_ts value
    fread(&end_ts, sizeof(uint64_t), 1, timestamps);
    
    // if interval unterminated, print the unterminated interval
    if (in_interval) {
        // extract the last value in timestamps read into &timestamp_buf
        end_ts = timestamp_buf;
        // convert to string
        strftime(begin_time_s, TIME_STR_LEN, TIME_FORMAT, gmtime(&begin_ts));
        strftime(end_time_s, TIME_STR_LEN, TIME_FORMAT, gmtime(&end_ts));

        duration = end_ts - begin_ts;
        total_duration += duration;
        total_ins += ins_count;

        printIntervalResult(++interval_count, begin_time_s, end_time_s,\
                            begin_ts, end_ts, ins_count);
    }

    // printing summary
    if (interval_count > 0) {
        putchar('\n');
        printf("Summary:\n\tTotal Duration: %.2f days\n\tTotal instances: %llu\n\tPercentage: %.2f%%"
               "\n",total_duration / (3600.0f*24), total_ins, 100.0f * total_ins / total_data_count);
        putchar('\n');
    }

    // housekeeping
    free(begin_time_s);
    free(end_time_s);
    // reset the file position pointers
    fsetpos(timestamps, &ts_pos);
    fsetpos(values, &val_pos);
}

// print the statistics to stdout of a certain interval  based on the given values.
void printIntervalResult(uint64_t interval_num, char *begin_time_s, char *end_time_s,
                         uint64_t begin_ts, uint64_t end_ts, uint64_t ins_count) {
    printf("Interval %llu:\n\tBegin time: %s PST\t(%llu)\n\tEnd time: %s PST\t(%llu)\n\tDuration: "
           "%.2f days\n\tInstance count: %llu\n",\
            interval_num,
            begin_time_s,
            begin_ts,
            end_time_s,
            end_ts,
            (end_ts - begin_ts) / (3600.0f*24),
            ins_count);
}

// close all files pointed to by **files, f_count 
void fclose_all(FILE **files, size_t f_count) {
    while (f_count-- > 0) {
        fclose(files[f_count]);
    }
}
