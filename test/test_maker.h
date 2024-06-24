#ifndef TEST_MAKER_H
#define TEST_MAKER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include <sys/wait.h>

#define LINE_LEN  64
#define TESTCASE_LEN    32
#define TM_INCLUDE   "   §TESTCASE§\n"
#define SYSTEM_FAILED   -1
#define TESTCASE_FILE_POSTFIX    ".modified"
#define NUM_DQ_MARKS_BEFORE_OUTPUTS_ARR    6

/**
 * @brief prints an error message to stderr.
 * 
 * @param err_msg an error message to be printed
 * @param ... variable argument list
 */
void raise_err(const char *err_msg, ...);

/**
 * @brief checks whether the program has been executed with the correct number of command line arguments.
 * 
 * @param argc the number of command line arguments
 */
void check_correct_execution(int argc);

/**
 * @brief returns a string which is the name of the modified testcase file.
 * 
 * @param testcase_filename the name of an original testcase file
 * @return the name which a modified testcase file will have
 *
 * @details The return value of this function must be `free`d once it's no longer needed.
 */
char *make_dest_filename(const char *testcase_filename);

/**
 * @brief reads a testcase file and makes a modified version of it.
 * 
 * @param src_name the name of an original testcase file
 * @param dest_name the name of the modified version of the original testcase file
 */
void preprocess_testcase(const char *src_name, const char *dest_name);

/**
 * @brief counts the length of the 'outputs' array that each testcase has and writes it to lengths[i].
 * 
 * @param lengths an array of integers which will contain how many expected outputs each testcase has
 * @param src an original testcase file
 */
void count_outputs_len(int lengths[], FILE *src);

/**
 * @brief writes a modified testcase file.
 * 
 * @param lengths an array of integers containing how many expected outputs each testcase has
 * @param src an original testcase file
 * @param dest a modified testcase file
 */
void write_modified_testcase(int lengths[], FILE *src, FILE *dest);

/**
 * @brief makes a modified 'test.c' file.
 * 
 * @param testcase_modified_filename the name of a modified testcase file
 */
void write_modified_file(const char *testcase_modified_filename);

/**
 * @brief builds the executable from the 'test.modified.c'.
 */
void build_executable(void);

/**
 * @brief erases temporary files.
 */
void cleanup(char *dest_filename);

/**
 * @brief prints a message notifying that the program has finished its work.
 */
void announce(void);

#endif