/**
 *  Alex Carbajal
 *  CPT_S 360, Spring 2024
 *  checklinks
 */

#include <stdio.h>
#include <stdlib.h> // for exit
#include <unistd.h> // for getopt()
#include <regex.h>  // for regcomp(), regexec(), regerror()

#include "find.h"
#include "helpers.h"

#define REG_ERR_SIZE 100

/* test for regex
    <a href="http://www.tricity.wsu.edu/this_link_does_not_exist">link</a>
    <a href=http://www.tricity.wsu.edu/this_link_does_not_exist>link</a>
*/
/* regex patterns
    <[^>]*href="?(https?:[^"?#]*?)[^"]*?(?(?<=")")[^>]*>
    <[^>]*href=(?:(https?:[^"?#]*?>)|("https?:[^"?#]*">))
    C String:
        <[^>]*href=(?:(https?:[^\"?#]*?>)|(\"https?:[^\"?#]*\">))
            The "?:" syntax to not capture the group does not work in POSIX Extended Regex
        <[^>]*href=((https?:[^\"?#]*?>)|(\"https?:[^\"?#]*\">))
    NOTE: The "//g" syntax to search globally does not work in POSIX Extended Regex
*/

int main(int argc, char *argv[])
{
    // no URL is provided
    if (argc == 1)
    {
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    regex_t regex_pattern;
    char *regex_str = "<[^>]*href=((https?:[^\"?#]*?>)|(\"https?:[^\"?#]*\">))";
    char regex_err[REG_ERR_SIZE] = "";
    int regcomp_result = 0;

    if ((regcomp_result = regcomp(&regex_pattern, regex_str, REG_EXTENDED | REG_ICASE)) != 0)
    {
        regerror(regcomp_result, &regex_pattern, regex_err, REG_ERR_SIZE);
        printf("regex error: %s\n", regex_err);
        exit(EXIT_FAILURE);
    }

    int regexec_result = 0;
    char *regex_find1 = "<a href=\"http://www.tricity.wsu.edu/this_link_does_not_exist\">link</a>";
    char *regex_find2 = "<a href=http://www.tricity.wsu.edu/this_link_does_not_exist>link</a>";
    if ((regexec_result = regexec(&regex_pattern, regex_find1, 0, NULL, 0)) != 0)
    {
        printf("regex_find1: match not found\n");
    }
    if ((regexec_result = regexec(&regex_pattern, regex_find2, 0, NULL, 0)) != 0)
    {
        printf("regex_find2: match not found\n");
    }

    exit(EXIT_SUCCESS);
}
