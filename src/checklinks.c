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

    Checklinks_Result *checklinks_results = (Checklinks_Result *)malloc(
        CHECKLINKS_RESULT_NUM * sizeof(Checklinks_Result));
    if (checklinks_results == NULL)
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

        // check the status of the URL
        if (check_url(url) == -1)
        {
            free(checklinks_results);
            exit(EXIT_FAILURE);
        }

        remove_url_slash(url);

        FILE *url_content = NULL;
        // download the contents of the URL to standard output
        if (download_url(url, &url_content) == -1)
        {
            free(checklinks_results);
            exit(EXIT_FAILURE);
        }

        check_content(&url_content, &checklinks_results);

        // close the stream
        if (pclose(url_content) == -1)
        {
            print_err();
            return -1;
        }
    }

    test_regex_pattern(&regex_pattern);

    free(checklinks_results);
    exit(EXIT_SUCCESS);
}
