/**
 *  Alex Carbajal
 *  CPT_S 360, Spring 2024
 *  checklinks
 */

#ifndef HELPERS
#define HELPERS

#include <stdio.h>  // for fprintf()
#include <stdlib.h> // for realloc()
#include <string.h> // for strerror()
#include <errno.h>  // for errno

#include "find.h"

/**
 * @brief print an error message
 */
extern void print_err(void);
/**
 * @brief print a message informing about the use of the program
 *
 * @param progname name of the running program
 */
extern void usage(const char *progname);
/**
 * @brief print a help message containing additional information about program use (for the "-h" option)
 */
extern void help(void);
/**
 * @brief check if a Checklinks_Results struct's URLs list needs to be reallocated with additional memory
 *
 * @param checklinks_results address to a Checklinks_Results struct
 * @return int 0 = success | -1 = error
 */
extern int checklinks_results_mem(Checklinks_Results *checklinks_results);
/**
 * @brief print out all URLs and their status stored in a Checklinks_Results struct
 *
 * @param checklinks_results address to a Checklinks_Results struct
 */
extern void print_urls(Checklinks_Results *checklinks_results);

#endif
