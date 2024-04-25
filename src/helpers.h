/**
 *  Alex Carbajal
 *  CPT_S 360, Spring 2024
 *  checklinks
 */

#ifndef HELPERS
#define HELPERS

#include <stdio.h>  // for fprintf()
#include <string.h> // for strerror()
#include <errno.h>  // for errno

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

#endif
