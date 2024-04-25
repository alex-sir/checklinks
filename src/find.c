/**
 *  Alex Carbajal
 *  CPT_S 360, Spring 2024
 *  checklinks
 */

#include "find.h"

int make_regex_pattern(regex_t *regex_pattern)
{
    char *regex_str = "<[^>]*href=(\"?)(https?:[^\"?#>]*)[^\">]*(\"?)[^>]*>";
    char regex_err[REG_ERR_SIZE] = "";
    int regcomp_result = 0;

    // make the regex pattern
    if ((regcomp_result = regcomp(regex_pattern, regex_str, REG_EXTENDED | REG_ICASE)) != 0)
    {
        // an error occurred when attempting to make the regex - print out the error
        regerror(regcomp_result, regex_pattern, regex_err, REG_ERR_SIZE);
        fprintf(stderr, "regex error: %s\n", regex_err);
        return -1;
    }

    return 0;
}

void test_regex_pattern(regex_t *regex_pattern)
{
    int regexec_result = 0;
    char *regex_find1 = "<a href=\"http://www.tricity.wsu.edu/this_link_does_not_exist\">link</a>";
    char *regex_find2 = "<a href=http://www.tricity.wsu.edu/this_link_does_not_exist>link</a>";

    if ((regexec_result = regexec(regex_pattern, regex_find1, 0, NULL, 0)) != 0)
    {
        printf("regex_find1: match not found\n");
    }

    if ((regexec_result = regexec(regex_pattern, regex_find2, 0, NULL, 0)) != 0)
    {
        printf("regex_find2: match not found\n");
    }
}
