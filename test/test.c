#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/wait.h>

#define TESTCASE_LEN 32
#define OUTPUT_LEN   1024
#define SYSTEM_FAILED   -1
#
#define TEST(_name, _argv, _input, _output) \
   if (tlen == TESTCASE_LEN) \
      raise_err("test: reached TESTCASE_LEN."); \
   tests[tlen++] = (testcase) { \
      .name = _name, \
      .argv = _argv, \
      .input = _input, \
      .output = _output \
   };

typedef struct testcase_ {
   char *name;
   char *argv;
   char *input;
   char *output;
} testcase;

void raise_err(char *err_msg);
testcase *acquire_testcases(int *testcases_len_ptr);
bool perform_test(testcase *testcases, int testcases_len, char *executable_name);

int main(int argc, char **argv) {
   if (argc != 2)
      raise_err(
         "test: usage:\n"
         "\t./test\n"
         "\t\tPATH_EXECUTABLE");
   if (!system(NULL))
      raise_err("test: no command processor available.");
   
   testcase *testcases;
   int testcases_len;
   bool test_failed;

   testcases = acquire_testcases(&testcases_len);
   test_failed = perform_test(testcases, testcases_len, argv[1]);
   free(testcases);

   return !test_failed
            ? EXIT_SUCCESS
            : EXIT_FAILURE;
}

/*
 * raise_err
 * prints an error message to stderr.
 */
void raise_err(char *err_msg) {
   fputs(err_msg, stderr);
   putc('\n', stderr);
   exit(EXIT_FAILURE);
}

/*
 * acquire_testcases
 * writes the length of testcases to the first argument
 * and returns a pointer to a memory block where test-
 * cases are.
 */
testcase *acquire_testcases(int *testcases_len_ptr) {
   testcase *tests;
   int tlen;

   tests = malloc(TESTCASE_LEN * sizeof(testcase));
   if (tests == NULL)
      raise_err("test: failed to malloc.");
   tlen = 0;
   
   /*
    * Include test cases. This line will be replaced
    * to something like #include "nsy.test".
    */
   §TESTCASE§

   if (tlen == 0)
      raise_err("test: testcase length of 0; terminating.");

   *testcases_len_ptr = tlen;
   return tests;
}

char *convert_linefeed(const char *src, char *replace_str, int len);

/*
 * perform_test
 * conducts tests with testcases.
 */
bool perform_test(testcase *testcases, int testcases_len, char *executable_name) {
   bool test_failed;
   
   test_failed = false; /* will change to true when a testcase fails. */

   /* Iterate testcases and perform each testcase. */
   for (int i = 0; i < testcases_len; i++) {
      /*
       * [Step 1] Opens .temp file in order to save
       * the output from the program to be tested.
       */
      FILE *temp;
      
      temp = fopen(".temp", "w+");  /* truncates the file content. */
      if (temp == NULL)
         raise_err("test: failed to open .temp file.");

      /*
       * [Step 2] Prepares a command which will be used to
       * execute the program to be tested.
       */
      char *command;
      int cmd_len;
      int return_value;

      cmd_len =
         strlen("  >.temp <<EOF\n\nEOF") +
         strlen(executable_name) +
         strlen(testcases[i].argv) +
         strlen(testcases[i].input) +
         1;  /* for \0 */

      command = malloc(cmd_len * sizeof(char));
      if (command == NULL)
         raise_err("test: failed to malloc.");

      return_value = snprintf(
         command,
         cmd_len,
         "%s %s >.temp <<EOF\n%s\nEOF",
         executable_name,
         testcases[i].argv,
         testcases[i].input);
      if (return_value < 0)
         raise_err("test: encoding error.");

      /*
       * [Step 3] Execute the command.
       */
      int status_code;

      status_code = system(command);
      if (status_code == SYSTEM_FAILED)
         raise_err("test: system() failed.");
      if (WIFEXITED(status_code)) {
         status_code = WEXITSTATUS(status_code);
         if (status_code != EXIT_SUCCESS) {
            fprintf(stderr,
               "test: system() returned with exit status %d.\n",
               status_code);
            if (fclose(temp) == EOF)
               raise_err("test: failed to close .temp file.");
            if (remove(".temp") != 0)
               raise_err("test: failed to remove .temp file.");
            exit(EXIT_FAILURE);
         }
      }
      else
         raise_err("test: WIFEXITED = false.\n");
      free(command);

      /*
       * [Step 4] Performs a test.
       */
      char *actual_output;          // ao,
      const char *expected_output;  // eo, hereafter.
      int ao_cur_idx, ao_max_num;
      bool str_not_equal;        /*
                                  * Without abbreviations, identifiers
                                  * would become so verbose UwU~~
                                  */

      ao_max_num = OUTPUT_LEN;
      actual_output = malloc(ao_max_num * sizeof(char));
      if (actual_output == NULL)
         raise_err("test: failed to malloc.");
      ao_cur_idx = 0;
      expected_output = testcases[i].output;
      str_not_equal = false;

      /*
       * A rough explanation about the below while statement:
       *
       * 1. Get a line from .temp file by using fgets().
       * 2. If the line is ended with \n, i.e. that is the whole line,
       *    carry out a testcase.
       * 3. Else, i.e. if the line is not complete,
       *    3.1. If the line does not have enough space to hold
       *         the result of the next fgets() call, realloc.
       *    3.2. Prepare the next fgets() call.
       */
      while (fgets(actual_output + ao_cur_idx, OUTPUT_LEN, temp) != NULL) {
         const int ao_len = strlen(actual_output);
         bool end_of_line, end_of_file;
         bool overflow_expected, line_ended;

         /* in case the last character of a stream is not \n. */
         end_of_file = feof(temp) ? true : false;
         end_of_line = actual_output[ao_len - 1] == '\n';

         /*
          * actual_output will not have enough space to hold the
          * result of next fgets call if this condition holds:
          *
          * num. of cur. elements (without \0)
          *       + num. of chars to be written
          *  > length of output
          * 
          * Note: refer to the comment in the below else clause
          *       for the reason \0 is not counted.
          */
         overflow_expected = ao_len + OUTPUT_LEN > ao_max_num;
         line_ended = end_of_line || end_of_file;

         if (line_ended) {
            if (strncmp(actual_output, expected_output, ao_len)) {
               char *eo_for_print, *ao_for_print;
               int eo_for_print_len;

               eo_for_print_len = strcspn(expected_output, "\n");
               if (expected_output[eo_for_print_len] == '\n')  /* since it could be '\0' */
                  eo_for_print_len++;  /* in order to print \n too */

               eo_for_print = convert_linefeed(expected_output, "\n\t\t\t", eo_for_print_len);
               ao_for_print = convert_linefeed(actual_output,   "\n\t\t\t", ao_len);

               printf(
                  "test: \"%s\": failed.\n"
                  "\texpected:\t%s\n"
                  "\tactual:\t\t%s\n",
                  testcases[i].name,
                  eo_for_print,
                  ao_for_print);
               
               free(eo_for_print);
               free(ao_for_print);

               str_not_equal = true;
               break;
            }
            expected_output += ao_len; // eo = eo + offset.
         }
         else {
            if (overflow_expected) {
               /*
                * Since the current line which is read from .temp file is not
                * complete, we need to realloc output to have more space.
                */
               ao_max_num *= 2;
               actual_output = realloc(actual_output, ao_max_num * sizeof(char));
               if (actual_output == NULL)
                  raise_err("test: failed to realloc.");
            }

            ao_cur_idx += OUTPUT_LEN - 1;
            /* 
             * Suppose OUTPUT_LEN = 5. Considering the last character is \0,
             * in order to concat the previous string and the next string,
             * the \0 from the prev. string must be overwritten.
             * 
             *    abcd0
             * +      efgh0
             * ------------
             *    abcdefgh0
             * 
             * Therefore, we need to add (OUTPUT_LEN - 1), rather than OUTPUT_LEN.
             */
         } // <== the end of the if statement
      } // <== the end of the while statement
      if (!str_not_equal) {
         if (*expected_output != '\0')
            fprintf(stderr,
               "test: \"%s\": failed; the reason being that "
               "the actual output has been exhausted, "
               "but the expected output remains:\n\t%s\n",
               testcases[i].name,
               expected_output);
         else
            printf("test: \"%s\": passed.\n", testcases[i].name);
      }
      else if (!test_failed)
         test_failed = true;      

      free(actual_output);
      if (fclose(temp) == EOF)
         raise_err("test: failed to close .temp file.");
   } // <== the end of the for statement
   if (remove(".temp") != 0)
      raise_err("test: failed to remove .temp file.");

   return test_failed;
}

/*
 * convert_linefeed
 * converts '\n' to replace_str.
 */
char *convert_linefeed(const char *src, char *replace_str, int len) {
   if (len < 0)
      raise_err("test: len < 0.");
   
   /* Scan the whole string, counting the occurrence of \n. */
   int src_idx, count;

   src_idx = 0;
   count = 0;
   while (src[src_idx] != '\0' && src_idx < len)
      if (src[src_idx++] == '\n')
         count++;

   int replace_str_len;
   int dest_len;
   char *dest;

   replace_str_len = strlen(replace_str);
   dest_len = 1 + src_idx - count + count * replace_str_len;

   dest = malloc(dest_len * sizeof(char));
   if (dest == NULL)
      raise_err("test: failed to malloc.");
   dest[dest_len - 1] = '\0';

   for (int dest_idx = 0, i = 0; i < src_idx; i++)
      if (src[i] == '\n') {
         strncpy(dest + dest_idx, replace_str, replace_str_len);
         dest_idx += replace_str_len;
      }
      else {
         dest[dest_idx] = src[i];
         dest_idx++;
      }

   return dest;
}