/**
 *  Alex Carbajal
 *  CPT_S 360, Spring 2024
 *  checklinks
 */

#ifndef FIND
#define FIND

#include <stdio.h>  // for printf(), fprintf(), snprintf(), remove()
#include <stdlib.h> // for system(), mkstemp(), malloc()
#include <unistd.h> // for close(), fstat(), lseek(), read()
#include <string.h> // for strlen(), strncpy()
#include <regex.h>  // for regex_t, regmatch_t, regcomp(), regexec(), regerror()
#include <sys/stat.h>

// sizes for strings
#define PATHNAME_MAX 4096   // max size of a local file pathname
#define URL_MAX 2000        // max size of a web URL
#define ACCESSIBILITY_MAX 5 // max size of the "accessibility" string in Checklinks_Result
#define REG_ERR_SIZE 100    // max size for the "errbuf" string in "regerror"

#define BUFFER_SIZE 4096 // buffer size for when reading the contents of a URL or file
#define TMP_SIZE 11      // size of the temporary file for checking for regex patterns

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
extern int process_url(char url[], regex_t *regex_pattern,
                       Checklinks_Result **checklinks_results);
/**
 * @brief check if a given URL is able to be retrieved
 *
 * @param url string representation of a URL
 * @return int 0 = success | -1 = error
 */
extern int check_url(const char url[]);
/**
 * @brief remove the trailing forward slash in a URL if it is present
 *
 * @param url string representation of a URL
 */
extern void remove_url_slash(char url[]);
/**
 * @brief downloads the contents (HTML) from a URL and pipe it into a stream
 *
 * @param url string representation of a URL
 * @param url_content address to a stream to pipe the contents of the URL into
 * @return int 0 = success | -1 = error
 */
extern int download_url(const char url[], const char tmp_filename[]);
extern int check_content(int temp_fd, const regex_t *regex_pattern);

#endif
