/**
 *  Alex Carbajal
 *  CPT_S 360, Spring 2024
 *  checklinks
 */

#include "helpers.h"

void print_err(void)
{
    fprintf(stderr, "%s\n", strerror(errno));
}

void usage(const char *progname)
{
    fprintf(stderr, "usage: %s [option]* urlOrFilename \n", progname);
}
