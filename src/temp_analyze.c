/**
 * This programs reads from a binary data file(with data encoded as doubles) and a corresponding
 * timestamps file, and prints to the output each and every intervals of time where data values 
 * are between user-defined upper and lower bounds. Only intervals with length greater than 
 * user-defined threshold value will be display, but all intervals are going to be included in the
 * summary section as part of the total statistics
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <wchar.h>

#define TIME_FORMAT "%Y/%m/%d %H:%M"
#define TIME_STR_LEN 26

void printIntervalResult(unsigned long interval_num, char *begin_time_s, char *end_time_s,\
                       unsigned long begin_ts, unsigned long end_ts, unsigned long ins_count);

int main(int argc, char *argv[]) {
    // argument check
    if (argc != 6) {
        fprintf(stderr, "Usage: temp_analyze infilename ul_ts_filename LOW HIGH threshold\n");
        return -1;
    }

    // set up input file
    FILE *input = fopen(argv[1], "r");
    if (!input) {
        fprintf(stderr, "Cannot access input file (%s).\n", argv[1]);
        return 1;
    }

    // set up timestamp file
    FILE *tsfile = fopen(argv[2], "r");
    if (!tsfile) {
        fprintf(stderr, "Cannot access timestamps file (%s).\n", argv[2]);
        return 2;
    }

    // argument loading and check
    // a temp reading needs to be between lower & upper bounds to be included in a interval
    double lower_bound, upper_bound;
    sscanf(argv[3], "%lf", &lower_bound);
    sscanf(argv[4], "%lf", &upper_bound);
    printf("Lower bound: %.4f / Upper bound: %.4f\n", lower_bound, upper_bound);
    if (lower_bound > upper_bound) {
        fprintf(stderr, "Your lower bound is greater than the upper bound.\n");
        return 3;
    }

    // if the length of a interval > threshold, display the statistics for it
    double threshold;
    sscanf(argv[5], "%lf", &threshold);
    if (threshold < 0) {
        fprintf(stderr, "Argument error: the threshold needs to be at least 0\n");
        return 4;
    }
    
    // set up containers
    double temp_reading;
    // the begin time, end time, data instances of each interval and the interval count
    unsigned long begin_ts, end_ts, ins_count = 1, duration, total_duration = 0, total_ins = 0,\
                  total_data_count = 0, interval_count = 0;
    unsigned long container; // to read unimportant timestamps into
    // used as a boolean
    unsigned char in_interval = 0;

    char *begin_time_s = malloc(TIME_STR_LEN);
    char *end_time_s = malloc(TIME_STR_LEN);
    
    // traversing through the input file
    while (fread(&temp_reading, sizeof(double), 1, input) && !feof(tsfile)) {
        total_data_count++;
        // if temp_reading is within range
        if (temp_reading >= lower_bound && temp_reading <= upper_bound) {
            if (!in_interval) {
                in_interval= 1;
                // read timestamp as begin time
                fread(&begin_ts, sizeof(unsigned long), 1, tsfile);
            } else {
                ins_count++;
                // push the stream pointer in timestamps 1 value back
                fread(&container, sizeof(unsigned long), 1, tsfile);
            }
        } else { // temp_reading not within range
            if (in_interval) {
                in_interval = 0;

                // read timestamp as end time
                fread(&end_ts, sizeof(unsigned long), 1, tsfile);
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
                fread(&container, sizeof(unsigned long), 1, tsfile);
            }
        }
    }

    // offset the tsfile by 1, since when the while condition evaluated for the last time, fread
    // is executed for the input but not the tsfile. Also, if still in interval, print it out with
    // the read end_ts value
    fread(&end_ts, sizeof(unsigned long), 1, tsfile);
    
    // if interval unterminated, print the result.
    if (in_interval) {
        // the last value in timestamps was read into &container
        end_ts = container;
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
    putchar('\n');
    printf("Summary:\n\tTotal Duration: %.2f days\n\tTotal instances: %lu\n\tPercentage: %.2f\%\n",\
            total_duration / (3600.0f*24), total_ins, 100.0f * total_ins / total_data_count);

    // see if the input is completely traversed
    int a = feof(input), b = feof(tsfile);
    if (a && b) {
        printf("Analysis complete.\n");
    } else {
        printf("Encountered unknown errors, printed results may be incomplete..\n");
    }

    // housekeeping
    fclose(input);
    fclose(tsfile);
    free(begin_time_s);
    free(end_time_s);

    return 0;
}

// print the statistics to stdout of a certain interval  based on the given values.
void printIntervalResult(unsigned long interval_num, char *begin_time_s, char *end_time_s,
    unsigned long begin_ts, unsigned long end_ts, unsigned long ins_count) {
    printf("Interval %lu:\n\tBegin time: %s PST (%lu)\n\tEnd time: %s PST (%lu)\n\tDuration: %.2f " 
           "days\n\tInstance count: %lu\n",\
            interval_num,
            begin_time_s,
            begin_ts,
            end_time_s,
            end_ts,
            (end_ts - begin_ts) / (3600.0f*24),
            ins_count);
}
