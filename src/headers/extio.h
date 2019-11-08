/**
 * This is the header file that extends the I/O functionality from stdio.h; "extio" as extended I/O.
 * extio.h defines functions for easily handling multiple files.
 *
 * extio.h
 * Author: Kalettuce
 */

#ifndef _EXTIO
#define _EXTIO

#include <stdio.h>

/**
 * open files with filenames that are in **fnames, with the mode specified in mode and save all the
 * file pointers to the provided **files; the count parameter specifies the number of files to read.
 * 
 * Return values:
 * Upon success, it returns the number of files that are successfully opened.
 * Upon failure, it returns the index of the first file that fails to open. All other files in the
 *      parameter, if successfully opened, will be closed.
 */
size_t fopen_all(char **fnames, const char * restrict mode, const size_t count, FILE **files);

/**
 * close files that are pointed to by **files; the count parameter specifies the number of files to
 * close.
 * 
 * Return value: the number of files closed by the operation
 */
size_t fclose_all(FILE **files, size_t count);

#endif
