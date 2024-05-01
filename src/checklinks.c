/**
 *  Alex Carbajal
 *  CPT_S 360, Spring 2024
 *  checklinks
 */

/* test for regex
    <a href="http://www.tricity.wsu.edu/">link</a>
    <a href=http://www.tricity.wsu.edu/>link</a>
*/

/* regex patterns for matching URLs found in links
    C String: <[^>]*href=(\"?)(https?:[^\"?#>]*)[^\">]*(\"?)[^>]*>
    Normal: <[^>]*href=("?)(https?:[^"?#>]*)[^">]*("?)[^>]*>
*/

#include <stdlib.h> // for exit, malloc(), free()
#include <string.h> // for strncpy()
#include <unistd.h> // for getopt()
#include <regex.h>  // for regex_t

#include "find.h"
#include "helpers.h"

int main(int argc, char *argv[])
{
    // no URL/file or any option is provided
    if (argc == 1)
    {
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    Options_Given options_given = {0, 0};
    int opt = 0;
    extern char *optarg;
    extern int optind;

    // parse all options given
    while ((opt = getopt(argc, argv, "fhp")) != -1)
    {
        switch (opt)
        {
        case 'f': // treat the "urlOrFilename" argument as a local filename
            options_given.local_filename = 1;
            break;
        case 'h': // print a help message and exit
            usage(argv[0]);
            help();
            exit(EXIT_SUCCESS);
            break;
        case 'p': // run in parallel
            options_given.parallel = 1;
            break;
        case '?': // invalid option character
            usage(argv[0]);
            exit(EXIT_FAILURE);
            break;
        }
    }

    // make sure the user provides a URL or file
    if ((options_given.local_filename || options_given.parallel) &&
        optind > argc - 1)
    {
        fprintf(stderr, "checklinks: please provide a URL or file\n");
        exit(EXIT_FAILURE);
    }

    // make the regex pattern for URLs found in links
    regex_t regex_pattern;
    if (make_regex_pattern(&regex_pattern) == -1)
    {
        exit(EXIT_FAILURE);
    }

    // holds the results of checking the links
    Checklinks_Results checklinks_results = {
        (Checklinks_Result *)malloc(CHECKLINKS_RESULT_NUM * sizeof(Checklinks_Result)), 0};
    if (checklinks_results.urls == NULL)
    {
        print_err();
        exit(EXIT_FAILURE);
    }

    // work with a URL
    if (!options_given.local_filename)
    {
        char url[URL_MAX] = "";
        strncpy(url, argv[optind], sizeof(url));

        // DON'T run in parallel
        if (!options_given.parallel)
        {
            if (process_url(url, &regex_pattern, &checklinks_results, 0) == -1)
            {
                free(checklinks_results.urls);
                exit(EXIT_FAILURE);
            }
        }
        else // run in parallel
        {
            if (process_url(url, &regex_pattern, &checklinks_results, 1) == -1)
            {
                free(checklinks_results.urls);
                exit(EXIT_FAILURE);
            }
        }
    }
    else // work with a local file
    {
        char local_filename[PATHNAME_MAX] = "";
        strncpy(local_filename, argv[optind], sizeof(local_filename));

        if (!options_given.parallel)
        {
            if (process_file(local_filename, &regex_pattern, &checklinks_results, 0) == -1)
            {
                free(checklinks_results.urls);
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            if (process_file(local_filename, &regex_pattern, &checklinks_results, 1) == -1)
            {
                free(checklinks_results.urls);
                exit(EXIT_FAILURE);
            }
        }
    }

    print_urls(&checklinks_results);
    free(checklinks_results.urls);
    exit(EXIT_SUCCESS);
}
