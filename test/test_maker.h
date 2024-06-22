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

// raise_err() prints an error message to stderr.
void raise_err(const char *err_msg, ...);

// check_correct_execution() checks whether the program has been executed
// with the correct number of command line arguments.
void check_correct_execution(int argc);

// make_dest_filename() returns a string which is the name of the modified testcase file.
// The return value of this function must be `free`d once it's no longer needed.
char *make_dest_filename(const char *testcase_filename);

// preprocess_testcase() reads a testcase file and makes a modified version of it.
void preprocess_testcase(const char *src_name, const char *dest_name);

// count_outputs_len() counts the length of the 'outputs' array that each testcase has
// and writes it to lengths[i].
void count_outputs_len(int lengths[], FILE *src);

// write_modified_testcase() writes a modified testcase file.
void write_modified_testcase(int lengths[], FILE *src, FILE *dest);

// write_modified_file() makes a modified 'test.c' file.
void write_modified_file(const char *testcase_modified_filename);

// build_executable() builds the executable from the 'test.modified.c'.
void build_executable(void);

// cleanup() erases temporary files.
void cleanup(char *dest_filename);

// announce() prints a message notifying that the program has finished its work.
void announce(void);

#endif