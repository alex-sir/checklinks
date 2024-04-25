/**
 *  Alex Carbajal
 *  CPT_S 360, Spring 2024
 *  checklinks
 */

#ifndef FIND
#define FIND

#include <stdio.h> // for fprintf()
#include <regex.h> // for regex_t, regcomp(), regexec(), regerror()

// sizes for strings
#define PATHNAME_MAX 4096   // max size of a local file pathname
#define URL_MAX 2000        // max size of a web URL
#define ACCESSIBILITY_MAX 5 // max size of the "accessibility" string in Checklinks_Result
#define REG_ERR_SIZE 100    // max size for the "errbuf" string in "regerror"

#define CHECKLINKS_RESULT_NUM 100 // default number of Checklinks_Result elements

// possible values for "accessibility" in Checklinks_Result
#define IS_ACCESSIBLE "okay"
#define NOT_ACCESSIBLE "error"

// 0 = option not given by user, 1 = option given by user
typedef struct
{
    int local_filename;
    int parallel;
} Options_Given;

// holds a URL and the results of a URL (okay/error) after running checklinks
typedef struct
{
    char url[URL_MAX];
    char accessibility[ACCESSIBILITY_MAX];
} Checklinks_Result;

#include "helpers.h"

/**
 * @brief make a regex pattern and store it in regex_pattern
 * 
 * @param regex_pattern address to a regex_t variable where the regex pattern is stored
 * @return int 0 = success | -1 = error
 */
extern int make_regex_pattern(regex_t *regex_pattern);
// TEMP: for testing regexec
extern void test_regex_pattern(regex_t *regex_pattern);

#endif
