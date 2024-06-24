#ifndef TEST_H
#define TEST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>
#include <sys/wait.h>

#include "test.macros.h"
#include "test.types.h"

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
 * @brief gets testcases, write the length of testcases via *testcases_len_ptr, and returns the pointer to the first testcase.
 * 
 * @param testcase_len_ptr a pointer to testcases_len
 * @return a pointer to an array of testcases
 * 
 * @details The return value of this function must be `free`d once it's no longer needed.
 */
testcase *acquire_testcases(int *testcases_len_ptr);

/**
 * @brief conducts tests with testcases.
 * 
 * @param testcases a pointer to an array of testcases
 * @param testcases_len the length of the testcases array
 * @param executable_name the name of the program to be tested
 * @return true if every testcases were successful; otherwise false.
 */
bool perform_test(testcase *testcases, int testcases_len, const char *executable_name);

/**
 * @brief `malloc`s a string, write a command to it, and returns it.
 * 
 * @param tc_ptr a pointer to a testcase struct that has necessary information to make the command to execute
 * @param executable_name the name of the program to be tested
 * @return a command to be executed soon; refer to execute_command().
 * 
 * @details The return value of this function must be `free`d once it's no longer needed.
 */
char *make_command(const testcase *tc_ptr, const char *executable_name);

/**
 * @brief runs the command acquired from the call of make_command().
 * 
 * @param cmd a command to execute
 */
void execute_command(const char *cmd);

/**
 * @brief reads lines in the tempfile, writes them in a string, and returns it.
 * 
 * @param tempfile a temporary file which contains the result of the execution of a command
 * @return a string which is the output from an executable that was run with a command
 * 
 * @details The return value of this function must be `free`d once it's no longer needed.
 */
char *acquire_output(FILE *tempfile);

/**
 * @brief checks if there is at least one expected output that matches the actual output.
 * 
 * @param params_ptr a pointer to a struct which contains things necessary to call this function
 * @return true if a testcase was successful; otherwise false.
 */
bool evaluate_testcase(const eval_tc_params *params_ptr);

/**
 * @brief prints the result of the testcase.
 * 
 * @param params_ptr a pointer to a struct which contains things necessary to call this function
 */
void display_testcase_result(const disp_tc_res_params *params_ptr);

/**
 * @brief replaces a line-feed character(s) in src with replace_str.
 * 
 * @param src a source string
 * @param replace_str a string used to replace a `\\n` character.
 * @param len this function reads characters from `src` until (1) it reads the null-terminating character or (2) it reads `len` characters.
 * 
 * @details The return value of this function must be `free`d once it's no longer needed.
 */
char *convert_linefeed(const char *src, const char *replace_str, int len);

#endif