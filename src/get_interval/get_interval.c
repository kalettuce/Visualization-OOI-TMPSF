/**
 * This is the implementation for get_interval.h; to find the documentation, go to get_interval.h
 * that is in the same directory.
 *
 * get_interval.c
 * Author: Kalettuce
 */

#include "extio.h"
#include "get_interval.h"

void processFile(FILE *values, FILE *timestamps,\
                 const double lower, const double upper, const double threshold) {
    // save the original file position to restore to
    fpos_t ts_pos;
    fpos_t val_pos;
    fgetpos(values, &val_pos);
    fgetpos(timestamps, &val_pos);
    // reset the file pointers to the beginning
    fseek(timestamps, 0, SEEK_SET);
    fseek(values, 0, SEEK_SET);

    // set up containers
    double temp_reading;
    // data instances of each interval and the interval count
    time_t begin_ts, end_ts; // the begin and end timestamps
    uint64_t duration, total_duration = 0, total_duration_r = 0; /* duration, total durations
                                                                  * and total duration that are
                                                                  * printed as intervals */
    uint64_t ins_count = 0, total_ins = 0, total_ins_r = 0; // instance counter, named as durations
    uint64_t total_data_count = 0, interval_count = 0; // interval counter and data counter
    uint64_t timestamp_buf; // to read unimportant timestamps to
    bool in_interval = false; // true if the file pointer is in an interval

    // allocating space to put the time strings in
    char *begin_time_s = malloc(TIME_STR_LEN);
    char *end_time_s = malloc(TIME_STR_LEN);
    
    
    // traversing through the input file
    while (fread(&temp_reading, sizeof(double), 1, values) && !feof(timestamps)) {
        total_data_count++;
        // if temp_reading is within range
        if (temp_reading >= lower && temp_reading <= upper) {
            if (!in_interval) {
                in_interval = true;
                // read timestamp as begin time
                fread(&begin_ts, sizeof(uint64_t), 1, timestamps);
            } else {
                // move the stream pointer in timestamps forward by 1
                fread(&timestamp_buf, sizeof(uint64_t), 1, timestamps);
            }
            ins_count++;
        } else { // when temp_reading not within range
            if (in_interval) {
                in_interval = false;

                // read timestamp as end time
                fread(&end_ts, sizeof(uint64_t), 1, timestamps);
                // convert time to string
                strftime(begin_time_s, TIME_STR_LEN, TIME_FORMAT, gmtime(&begin_ts));
                strftime(end_time_s, TIME_STR_LEN, TIME_FORMAT, gmtime(&end_ts));
                duration = end_ts - begin_ts;

                // print if interval is longer threshold
                if (threshold * SEC_IN_DAY <= duration) {
                    total_duration_r += duration;
                    total_ins_r += ins_count;
                    printIntervalResult(++interval_count, begin_time_s,
                                        end_time_s, begin_ts, end_ts, ins_count);
                }

                // update "total" variables
                total_ins += ins_count;
                total_duration += duration;
                // reset individual counters
                ins_count = 0;
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

        if (threshold * SEC_IN_DAY <= duration) {
            total_ins_r += ins_count;
            total_duration_r += duration;
            printIntervalResult(++interval_count, begin_time_s,
                    end_time_s, begin_ts, end_ts, ins_count);
        }
    }

    // printing summary
    if (interval_count > 0) {
        putchar('\n');
        printf("Summary:\n"
               "\tTotal Duration: %.2f days\n"
               "\tConcentrated Duration: %.2f days\n\n"
               "\tTotal Percentage: %.2f%%\n"
               "\tConcentrated Percentage: %.2f%%\n\n"
               "\tTotal instances: %llu\n"
               "\tConcentrated instances: %llu\n",
               total_duration / SEC_IN_DAY,
               total_duration_r / SEC_IN_DAY,
               GET_PERC(total_ins, total_data_count),
               GET_PERC(total_ins_r, total_data_count),
               total_ins,
               total_ins_r);
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
    printf("Interval %llu:\n"
           "\tBegin time: %s PST (%llu)\n"
           "\tEnd time: %s PST (%llu)\n"
           "\tDuration: %.2f days\n"
           "\tInstance count: %llu\n",\
            interval_num,
            begin_time_s,
            begin_ts,
            end_time_s,
            end_ts,
            (end_ts - begin_ts) / SEC_IN_DAY,
            ins_count);
}
