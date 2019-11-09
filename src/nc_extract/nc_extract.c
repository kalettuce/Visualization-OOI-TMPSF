
/**
 * This is the actual implementation of nc_extract.h; for a documentation go to nc_extract.h
 *
 * nc_extract.c
 * Author: Kalettuce
 */

#include "extio.h"
#include "nc_extract.h"

// handles nc errors by printing out error message and exits
void handle_err(int status) {
    fprintf(stderr,"The nc module encounters an error:\n\t%s\n", nc_strerror(status));
    exit(NC_ERR);
}
