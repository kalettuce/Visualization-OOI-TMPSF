/**
 * This is the header file specific made for get_interval. It defines all the standard libraries to
 * be included and other useful definitions for the main function of get_interval.
 *
 * get_interval.h
 * Author: Kalettuce
 */

// standard libraries
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <wchar.h>

/* definitions of the how the time strings are formatted, and a corresponding buffer length */
#define TIME_FORMAT "%Y/%m/%d %H:%M"
#define TIME_STR_LEN 26
/* other definitions */
#define SEC_IN_DAY (3600.0 * 24)
#define GET_PERC(nom,denom) (100.0f*nom) / denom

/**
 * print results for one interval to stdout.
 * Parameters:
 *  interval_num: the index of the interval; starts at 1
 *  begin/end_time_s: the time string for begin and end times
 *  begin_ts, end_ts: the timestamps for when the interval began and ended. (epoch style)
 *  ins_count: the count of data that are in this interval
 */
void printIntervalResult(const uint64_t interval_num, char *begin_time_s, char *end_time_s,\
                         uint64_t begin_ts, uint64_t end_ts, uint64_t ins_count);

/**
 * process an single data file against a timestamp file and prints the results to the stdout.
 * Parameters:
 *  values: the file that contains the data
 *  timestamps: the file that contains the timestamps; needs to have same length as the data file
 *  lower, upper: the bounds that mark the data range
 *  threshold: if an interval is short than 'threshold' days, it will not count as an interval
 */
void processFile(FILE *values, FILE *timestamps,\
                 const double lower, const double upper, const double threshold);
