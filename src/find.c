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
        regerror(regcomp_result, regex_pattern, regex_err, sizeof(regex_err));
        fprintf(stderr, "regex error: %s\n", regex_err);
        return -1;
    }

    return 0;
}

int check_url(const char url[], int print_err)
{
    const char *command = "wget --spider -q --delete-after -T10 -t1 ";
    char check_command[sizeof(command) + URL_MAX] = ""; // +2 for the single quotes around the URL
    int status = 0;

    snprintf(check_command, sizeof(check_command), "%s'%s'", command, url);
    status = system(check_command);

    // check if the command terminated normally
    if (WIFEXITED(status))
    {
        int exit_status = WEXITSTATUS(status);

        // check the exit status of the command
        if (exit_status != 0)
        {
            if (print_err)
            {
                fprintf(stderr, "checklinks: unable to retrieve URL\n");
            }
            return -1; // URL is not valid
        }
    }
    else
    {
        if (print_err)
        {
            fprintf(stderr, "checklinks: \"wget\" terminated abnormally\n");
        }
        return -1; // command terminated abnormally
    }

    return 0; // valid URL
}

void remove_url_slash(char url[])
{
    if (url[strlen(url) - 1] == '/')
    {
        url[strlen(url) - 1] = '\0';
    }
}

int download_url(const char url[], const char tmp_filename[])
{
    const char *command = "wget --no-cache -q -O ";
    // +4 for the single quotes around the URL, 1 space, and a null terminator
    char download_command[strlen(command) + strlen(tmp_filename) + URL_MAX + 4];
    int status = 0;

    snprintf(download_command, sizeof(download_command), "%s%s '%s'", command, tmp_filename, url);
    system(download_command);

    // check if the command terminated normally
    if (WIFEXITED(status))
    {
        int exit_status = WEXITSTATUS(status);

        // check the exit status of the command
        if (exit_status != 0)
        {
            fprintf(stderr, "checklinks: unable to download URL contents\n");
            return -1; // can't download the contents from the URL
        }
    }
    else
    {
        fprintf(stderr, "checklinks: \"wget\" terminated abnormally\n");
        return -1; // command terminated abnormally
    }

    return 0;
}

int is_dup_url(Checklinks_Results *checklinks_results, const char url[])
{
    for (int i = 0; i < checklinks_results->count; i++)
    {
        if (strcmp(url, checklinks_results->urls[i].url) == 0)
        {
            return 1; // link is a duplicate
        }
    }

    return 0; // link is NOT a duplicate
}

int check_content(int temp_fd, const regex_t *regex_pattern, Checklinks_Results *checklinks_results)
{
    // get the size of the temp file
    struct stat temp_stat;
    if (fstat(temp_fd, &temp_stat) == -1)
    {
        print_err();
        return -1;
    }
    off_t temp_size = temp_stat.st_size;

    // string to hold the contents of the temp file for parsing
    char *temp_buffer = (char *)malloc(temp_size + 1);
    if (temp_buffer == NULL)
    {
        print_err();
        return -1;
    }

    // copy all the contents of the temp file to the buffer
    if (read(temp_fd, temp_buffer, temp_size) == -1)
    {
        free(temp_buffer);
        print_err();
        return -1;
    }
    temp_buffer[temp_size] = '\0';

    char *current_match = temp_buffer;
    int regexec_result = 0;
    const int NUM_GROUPS = 3; // number of groups in the regex pattern
    regmatch_t match[NUM_GROUPS];
    regoff_t match_len;
    const int GROUP_CAPTURE = 2; // group to capture in the regex pattern
    int check_url_result = 0;

    // search for matches
    while ((regexec_result = regexec(regex_pattern, current_match, NUM_GROUPS, match, 0)) == 0)
    {
        // get the group
        match_len = match[GROUP_CAPTURE].rm_eo - match[GROUP_CAPTURE].rm_so;
        char match_group[match_len + 1];
        strncpy(match_group, &current_match[match[GROUP_CAPTURE].rm_so], match_len);
        match_group[match_len] = '\0';

        // validate memory
        if (checklinks_results_mem(checklinks_results) == -1)
        {
            return -1;
        }
        remove_url_slash(match_group);
        // check if the URL is a duplicate
        if (is_dup_url(checklinks_results, match_group))
        {
            current_match += match[0].rm_eo;
            continue;
        }

        strncpy(checklinks_results->urls[checklinks_results->count].url, match_group, URL_MAX);
        // valid URL
        if ((check_url_result = check_url(match_group, 0)) == 0)
        {
            strncpy(checklinks_results->urls[checklinks_results->count].status,
                    IS_ACCESSIBLE, STATUS_MAX);
        }
        else // invalid URL
        {
            strncpy(checklinks_results->urls[checklinks_results->count].status,
                    NOT_ACCESSIBLE, STATUS_MAX);
        }
        checklinks_results->count++;

        current_match += match[0].rm_eo; // move pointer to the end of the match
    }

    free(temp_buffer);
    return 0;
}

int process_url(char url[], const regex_t *regex_pattern, Checklinks_Results *checklinks_results)
{
    if (check_url(url, 1) == -1)
    {
        return -1;
    }

    remove_url_slash(url);

    // create a temp file
    char tmp_filename[TMP_SIZE] = "tmp_XXXXXX";
    int temp_fd = mkstemp(tmp_filename);
    if (temp_fd == -1)
    {
        print_err();
        return -1;
    }

    // download the contents of the URL to the temp file
    if (download_url(url, tmp_filename) == -1)
    {
        return -1;
    }

    if (check_content(temp_fd, regex_pattern, checklinks_results) == -1)
    {
        return -1;
    }

    if (close(temp_fd) == -1)
    {
        print_err();
        return -1;
    }

    if (remove(tmp_filename) == -1)
    {
        print_err();
        return -1;
    }

    return 0;
}
