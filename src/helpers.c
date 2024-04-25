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
    printf("usage: %s [option]* urlOrFilename \n", progname);
}

void help(void)
{
    printf("Check every link on a web page or in an HTML file for validity (URL is default).\n\n");
    printf("Options:\n");
    printf("-f   treat the \"urlOrFilename\" argument as a local filename\n");
    printf("-h   print a help message and exit (this page)\n");
    printf("-p   run in parallel\n");
}
