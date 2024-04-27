/**
 *  Alex Carbajal
 *  CPT_S 360, Spring 2024
 *  checklinks
 */

#ifndef FIND
#define FIND

#include <stdio.h>    // for printf(), fprintf(), snprintf(), remove()
#include <stdlib.h>   // for system(), mkstemp(), malloc(), free()
#include <unistd.h>   // for close(), fstat(), lseek(), read(), fork(), execvp()
#include <string.h>   // for strlen(), strncpy(), strcmp()
#include <fcntl.h>    // for open()
#include <regex.h>    // for regex_t, regmatch_t, regcomp(), regexec(), regerror()
#include <sys/stat.h> // for stat
#include <sys/wait.h> // for wait()

// sizes for strings
#define PATHNAME_MAX 4096 // max size of a local file pathname
#define URL_MAX 2000      // max size of a web URL
#define STATUS_MAX 6      // max size of the "status" string in Checklinks_Result
#define REG_ERR_SIZE 256  // max size for the "errbuf" string in "regerror"

#define BUFFER_SIZE 4096 // buffer size for when reading the contents of a URL or file
#define TMP_NAME_SIZE 11 // size of the name of the temporary file for checking for regex patterns

#define CHECKLINKS_RESULT_NUM 100 // default number of Checklinks_Result elements

// possible values for "status" in Checklinks_Result
#define IS_ACCESSIBLE "okay"
#define NOT_ACCESSIBLE "error"

// 0 = option not given by user, 1 = option given by user
typedef struct
{
    int local_filename;
    int parallel;
} Options_Given;

/*
   holds a URL, the status of a URL (okay/error) after running checklinks,
   and the process id of the child that checked the link (only if running in parallel)
*/
typedef struct
{
    char url[URL_MAX];
    char status[STATUS_MAX];
    pid_t child_pid;
} Checklinks_Result;

// holds a list of Checklinks_Result and the count of that list
typedef struct
{
    Checklinks_Result *urls;
    int count;
} Checklinks_Results;

#include "helpers.h"

/**
 * @brief make a regex pattern and store it in regex_pattern
 *
 * @param regex_pattern address to a regex_t variable where the regex pattern is stored
 * @return int 0 = success | -1 = error
 */
extern int make_regex_pattern(regex_t *regex_pattern);
/**
 * @brief check if a given URL is able to be retrieved
 *
 * @param url string representation of a URL
 * @param err_printing 0 = don't print error messages | 1 = print error messages
 * @return int 0 = success | -1 = error
 */
extern int check_url(const char url[], int err_printing);
/**
 * @brief check if a given URL is able to be retrieved (parallel)
 *
 * @param url string representation of a URL
 * @param childpid address to a pid_t to store the process id of the forked process
 * @return int 0 = success | -1 = error
 */
extern int check_url_p(char url[], pid_t *childpid);
/**
 * @brief remove the trailing forward slash in a URL if it is present
 *
 * @param url string representation of a URL
 */
extern void remove_url_slash(char url[]);
/**
 * @brief download the contents (HTML file) from a URL and save it into a temporary file
 *
 * @param url string representation of a URL
 * @param tmp_filename name of the temporary file
 * @return int 0 = success | -1 = error
 */
extern int download_url(const char url[], const char tmp_filename[]);
/**
 * @brief check if a URL is a duplicate in a Checklinks_Results URLs list
 *
 * @param checklinks_results address to a Checklinks_Results struct
 * @param url URL to check for duplicity
 * @return int 0 = NOT a duplicate | 1 = duplicate
 */
extern int is_dup_url(Checklinks_Results *checklinks_results, const char url[]);
/**
 * @brief search through an HTML file to find and test URLs for existence
 *
 * @param temp_buffer string holding the contents of the HTML file
 * @param regex_pattern regex pattern to look for when looking for URLs
 * @param checklinks_results address to a Checklinks_Results struct
 * @return int 0 = success | -1 = error
 */
extern int search_content(char *temp_buffer, const regex_t *regex_pattern, Checklinks_Results *checklinks_results);
/**
 * @brief check and fill the statuses of URLs (parallel)
 *
 * @param checklinks_results address to a Checklinks_Results struct
 * @return int 0 = success | -1 = error
 */
extern int check_statuses_p(Checklinks_Results *checklinks_results);
/**
 * @brief search through an HTML file to find and test URLs for existence (parallel)
 *
 * @param temp_buffer string holding the contents of the HTML file
 * @param regex_pattern regex pattern to look for when looking for URLs
 * @param checklinks_results address to a Checklinks_Results struct
 * @return int 0 = success | -1 = error
 */
extern int search_content_p(char *temp_buffer, const regex_t *regex_pattern, Checklinks_Results *checklinks_results);
/**
 * @brief check the status of all URLs in an HTML file downloaded from a URL
 *
 * @param temp_fd file descriptor to a temporary file where the downloaded HTML of a URL is stored
 * @param regex_pattern regex pattern to look for when looking for URLs
 * @param checklinks_results address to a Checklinks_Results struct
 * @param run_parallel 0 = DON'T run processes in parallel | 1 = run processes in parallel
 * @return int 0 = success | -1 = error
 */
extern int check_content(int temp_fd, const regex_t *regex_pattern,
                         Checklinks_Results *checklinks_results, const int run_parallel);
/**
 * @brief process all URLs present in a URL by attempting to access them
 *
 * @param url string to the URL to check URLs for
 * @param regex_pattern regex pattern to look for when looking for URLs
 * @param checklinks_results address to a Checklinks_Results struct
 * @param run_parallel 0 = DON'T run processes in parallel | 1 = run processes in parallel
 * @return int 0 = success | -1 = error
 */
extern int process_url(char url[], const regex_t *regex_pattern,
                       Checklinks_Results *checklinks_results, const int run_parallel);
/**
 * @brief process all URLs present in a local HTML file by attempting to access them
 *
 * @param local_filename string of a path to a local file
 * @param regex_pattern regex pattern to look for when looking for URLs
 * @param checklinks_results address to a Checklinks_Results struct
 * @param run_parallel 0 = DON'T run processes in parallel | 1 = run processes in parallel
 * @return int int 0 = success | -1 = error
 */
extern int process_file(const char local_filename[], const regex_t *regex_pattern,
                        Checklinks_Results *checklinks_results, const int run_parallel);

#endif
