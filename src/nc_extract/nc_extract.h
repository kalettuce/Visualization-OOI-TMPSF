/**
 * This is the header file specifically for nc_extract. It includes the required standard libraries,
 * defines macros and functions used in nc_extract.
 *
 * nc_extract.h
 * Author: Kalettuce
 */

#include <netcdf.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#include "extio.h"

/* definition of error codes */
#define ARG_ERR 1
#define FILE_ERR 2
#define NC_ERR -1

// handles nc errors by printing out error message and exits
void handle_err(int status);
