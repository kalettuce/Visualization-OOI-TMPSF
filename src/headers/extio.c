/*
 * This is the actual implementation of ioext.h
 * For a description of this header file or a documentation of the implemented functions,  check
 * ioext.h in the same directory.
 *
 * extio.c
 * Author: Kalettuce
 */

#include "extio.h"

size_t fopen_all(char **fnames, const char * restrict mode, const size_t count, FILE **files) {
    size_t i; // iterator
    for (i = 0; i < count; i++) {
        files[i] = fopen(fnames[i], mode); // open the file and save it to **files
        if (!files[i]) {
            fclose_all(files, i); // close all opened files
            return i; // return the index of the failed operation
        }
    }
    return i; // the number of files successfully read
}

size_t fclose_all(FILE **files, size_t count) {
    size_t i;
    for (i = 0; i < count; i++) {
        fclose(files[i]);
    }
    return i;
}
