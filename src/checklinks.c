/**
 *  Alex Carbajal
 *  CPT_S 360, Spring 2024
 *  checklinks
 */

/* test for regex
    <a href="http://www.tricity.wsu.edu/this_link_does_not_exist">link</a>
    <a href=http://www.tricity.wsu.edu/this_link_does_not_exist>link</a>
*/

/* regex patterns for matching URLs found in links
    C String: <[^>]*href=(\"?)(https?:[^\"?#>]*)[^\">]*(\"?)[^>]*>
    Normal: <[^>]*href=("?)(https?:[^"?#>]*)[^">]*("?)[^>]*>
*/

#include <stdlib.h> // for exit, malloc()
#include <string.h> // for strncpy()
#include <unistd.h> // for getopt()
#include <regex.h>  // regex_t

#include "find.h"
#include "helpers.h"

int main(int argc, char *argv[])
{
    // no URL or any option is provided
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

    // work with a local file
    if (options_given.local_filename)
    {
        char local_filename[PATHNAME_MAX] = "";
        strncpy(local_filename, argv[optind], sizeof(local_filename));
        // open the local file
    }
    else // work with a URL
    {
        char url[URL_MAX] = "";
        strncpy(url, argv[optind], sizeof(url));

        if (process_url(url, &regex_pattern, &checklinks_results) == -1)
        {
            free(checklinks_results.urls);
            exit(EXIT_FAILURE);
        }
    }

    print_urls(&checklinks_results);
    free(checklinks_results.urls);
    exit(EXIT_SUCCESS);
}
