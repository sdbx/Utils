#ifndef TEST_H
#define TEST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>
#include <sys/wait.h>

#include "test.macro"
#include "test.typedef"

// raise_err() prints an error message to stderr.
void raise_err(const char *err_msg, ...);

// check_correct_execution() checks whether the program has been executed
// with the correct number of command line arguments.
void check_correct_execution(int argc);

// acquire_testcases() gets testcases, write the length of testcases via
// testcases_len_ptr, and returns the pointer to the first testcase.
// The return value of this function must be `free`d once it's no longer needed.
testcase *acquire_testcases(int *testcases_len_ptr);

// perform_test() conducts tests with testcases.
bool perform_test(testcase *testcases, int testcases_len, const char *executable_name);

// make_command() `malloc`s a string, write a command to it, and returns it.
// The return value of this function must be `free`d once it's no longer needed.
char *make_command(const testcase *tc_ptr, const char *executable_name);

// execute_command() runs the command acquired from the call of make_command().
void execute_command(const char *cmd, FILE *tempfile);

// acquire_output() reads lines in the tempfile, writes them in a string, and returns it.
// The return value of this function must be `free`d once it's no longer needed.
char *acquire_output(FILE *tempfile);

// evaluate_testcase() checks if there is at least one expected output that
// matches the actual output.
bool evaluate_testcase(const eval_tc_params *params_ptr);

// prints the result of the testcase.
void display_testcase_result(const disp_tc_res_params *params_ptr);

// convert_linefeed() replaces \n character(s) in src with replace_str.
// The return value of this function must be `free`d once it's no longer needed.
char *convert_linefeed(const char *src, const char *replace_str, int len);

#endif