/**
 *  Alex Carbajal
 *  CPT_S 360, Spring 2024
 *  checklinks
 */

#include <stdlib.h> // for exit, malloc(), qsort()
#include <unistd.h> // for getopt()

#include "find.h"
#include "helpers.h"

// test string for regex: <a href="http://www.tricity.wsu.edu/this_link_does_not_exist">link</a>
// regex pattern: /<[^>]*href="?(https?:[^"?#]*)[^"]*[^>]*>/g

int main(int argc, char *argv[])
{
    // no URL is provided
    if (argc == 1)
    {
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
