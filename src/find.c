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

int check_url(const char url[])
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
            fprintf(stderr, "checklinks: unable to retrieve URL\n");
            return -1; // URL is not valid
        }
    }
    else
    {
        fprintf(stderr, "checklinks: \"wget\" terminated abnormally\n");
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

int download_url(const char url[], FILE **url_content)
{
    const char *command = "wget --no-cache --delete-after -q -O - ";
    char download_command[sizeof(command) + URL_MAX + 2] = ""; // +2 for the single quotes around the URL

    snprintf(download_command, sizeof(download_command), "%s'%s'", command, url);

    // pipe the output of the download command
    *url_content = popen(download_command, "r");
    if (*url_content == NULL)
    {
        print_err();
        return -1;
    }

    return 0;
}

int check_content(FILE **url_content, Checklinks_Result **checklinks_results)
{
    char content_buffer[BUFFER_SIZE] = "";

    // go through the content in the HTML file of the URL
    while (fgets(content_buffer, sizeof(content_buffer), *url_content) != NULL)
    {
        printf("%s", content_buffer);
    }

    return 0;
}
