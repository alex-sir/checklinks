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
    printf("-f   treat the \"urlOrFilename\" argument as a local file\n");
    printf("-h   print a help message and exit (this page)\n");
    printf("-p   run in parallel\n");
}

int checklinks_results_mem(Checklinks_Results *checklinks_results)
{
    // checklinks_results filled
    if (checklinks_results->count % CHECKLINKS_RESULT_NUM == 0 && checklinks_results->count != 0)
    {
        checklinks_results->urls = (Checklinks_Result *)realloc(
            checklinks_results->urls,
            (checklinks_results->count + CHECKLINKS_RESULT_NUM) * sizeof(Checklinks_Result));

        if (checklinks_results->urls == NULL)
        {
            print_err();
            return -1; // error
        }
    }

    return 0;
}

void print_urls(Checklinks_Results *checklinks_results)
{
    for (int i = 0; i < checklinks_results->count; i++)
    {
        printf("%-*s%s\n", 7, checklinks_results->urls[i].status,
               checklinks_results->urls[i].url);
    }
}
