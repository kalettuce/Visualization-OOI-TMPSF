/**
 * nc_extract takes an input .nc file, finds the specified variable, extracts it and encodes it
 * into a binary file.
 * This program is incomplete in the following ways:
 *  - it can only take 1 input file, 1 output file and 1 variable name at a time.
 *  - it only supports encoding as doubles.
 * You can fix these issuses if you can read and make change to the source code below; a more
 * generalized version will also be developed in the future
 *
 * nc_extract.c
 * Author: Kalettuce
 */

#include <netcdf.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* definition of error codes */
#define ARG_ERR 1
#define FILE_ERR 2
#define NC_ERR -1

/* handles nc errors by printing out error message and quits */
void handle_err(int status);

int main(int argc, char **argv) {

    // argument check
    if (argc != 4) {
        fprintf(stderr, "Usage: nc_extract input.nc outfilename variable_name");
        exit(ARG_ERR);
    }

    int ncid;
    int status;
    // check and open the input file in read-only mode
    if ((status = nc_open(argv[1], NC_NOWRITE, &ncid))) {
        handle_err(status);
    }

    /************* Extraction of miscellaneous data ***************/
    // containers declaration
    int varid, dimid, ndims;
    size_t dimlen;
    // get the varid
    if ((status = nc_inq_varid(ncid, argv[3], &varid))) {
        handle_err(status);
    }

    // get the size of the var
    if ((status = nc_inq_varndims(ncid, varid, &ndims))) {
        handle_err(status);
    }

    // get the dimid of the desired dimension
    if ((status = nc_inq_dimid(ncid, "obs", &dimid))) {
        handle_err(status);
    }

    // get the length of the dimension
    if ((status = nc_inq_dimlen(ncid, dimid, &dimlen))) {
        handle_err(status);
    }
    /****************** End of misc data *********************/
    /****************** Extraction of main bulk of data ********************/
    /*----------------- Reading data into memory ----------------*/
    double *var_data = malloc(dimlen * sizeof(double));
    // size_t index[5] = {0};
    if ((status = nc_get_var_double(ncid, varid, var_data))) {
    // if ((status = nc_get_var1_double(ncid, varid, index, var_data))) {
        handle_err(status);
    }
    for (int i = 0; i < 20; i++) {
        printf("%f\n", var_data[i]);
    }

    /*---------------- Writing data to output ----------------*/
    // set up the file stream, append if file exists
    FILE *output;
    if (access(argv[2], F_OK)) { // when the file doesn't exist
        output = fopen(argv[2], "w");
    } else if (access(argv[2], W_OK)) { // when the user doesn't have writing permission
        fprintf(stderr, "Permission denied.\n");
        exit(FILE_ERR);
    } else {
        output = fopen(argv[2], "a");
    }
    // a second check in case when the file doesn't exist and user doesn't have permission
    // to write in the target directory
    if (!output) {
        fprintf(stderr, "Permission denied, or the output path might be a directory.\n");
        exit(FILE_ERR);
    }

    fwrite(var_data, dimlen, sizeof(double), output);
    /******************* End Extraction ************************/

    // housekeeping
    fclose(output);
    nc_close(ncid);
    free(var_data);

    return 0;
}

// handles nc errors by printing out error message and exits
void handle_err(int status) {
    fprintf(stderr,"The nc module encounters an error:\n\t%s\n", nc_strerror(status));
    exit(NC_ERR);
}
