#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/wait.h>

#define TESTCASE_LEN 32
#define OUTPUT_LEN   1024
#define SYSTEM_FAILED   -1

typedef struct testcase_ {
   char *name;
   char *argv;
   char *input;
   char *output;
} testcase;

void raise_err(char *err_msg);

int main(int argc, char **argv) {
   if (argc != 2)
      raise_err(
         "test: usage:\n"
         "\t./test\n"
         "\t\tPATH_EXECUTABLE");
   if (!system(NULL))
      raise_err("test: no command processor available.");
   
   /* Prepare to include test cases. */
   testcase *tests;
   
   tests = malloc(TESTCASE_LEN * sizeof(testcase));
   if (tests == NULL)
      raise_err("test: failed to malloc.");

   int tlen;
   
   /* Include test cases. Ex: #include "nsy.test" */
   §TESTCASE§

   /* Perform tests. */
   for (int i = 0; i < tlen; i++) {
      FILE *temp;
      
      temp = fopen(".temp", "w+");  /* truncates the file content. */
      if (temp == NULL)
         raise_err("test: failed to open .temp file.");

      char *command;
      int cmd_len;

      cmd_len =
         strlen("./  >.temp <<EOF\n\nEOF") +
         strlen(argv[1]) +
         strlen(tests[i].argv) +
         strlen(tests[i].input) +
         1;
      command = malloc(cmd_len * sizeof(char));

      int return_value;
      int status_code;

      return_value = snprintf(
         command,
         cmd_len,
         "%s %s >.temp <<EOF\n%s\nEOF",
         argv[1],
         tests[i].argv,
         tests[i].input);
      if (return_value < 0)
         raise_err("test: encoding error.");

      status_code = system(command);
      free(command);
      if (status_code == SYSTEM_FAILED)
         raise_err("test: system() failed.");
      if (WIFEXITED(status_code))
         if (WEXITSTATUS(status_code) == EXIT_FAILURE)
            raise_err("test: abnormal termination.");
      
      char *output;
      int output_cur_idx, output_max_num;
      int offset;
      bool str_not_equal;

      output_cur_idx = 0;
      output_max_num = OUTPUT_LEN;
      output = malloc(output_max_num * sizeof(char));
      if (output == NULL)
         raise_err("test: failed to malloc.");

      offset = 0;
      str_not_equal = false;

      /*
         A rough explanation of the below while statement:

         1. Get a line using fgets.
         2. If the line is ended with \n, i.e. that is the whole line,
            carry out a testcase.
         3. Else, i.e. if the line is not complete,
            3.1. If the line does not have enough space to hold
                 the result of the next fgets() call, realloc.
            3.2. Prepare the next fgets() call.
      */

      while (fgets(output + output_cur_idx, OUTPUT_LEN, temp) != NULL) {
         const int output_len = strlen(output);
         int ch;
         bool end_of_line, end_of_file;
         bool overflow_expected, line_ended;

         /* In case the last character of a stream is not \n. */
         if ((ch = fgetc(temp)) == EOF)
            end_of_file = true;
         else {
            end_of_file = false;
            ungetc(ch, temp);
         }
         end_of_line = output[output_len - 1] == '\n';

         /*
            output will not have enough space to hold the
            result of next fgets call if this condition holds:

            num. of cur. elements (without \0)
               + num. of chars to be written
            > length of output

            Note: refer to the comment in the below else clause
                  for the reason \0 is not counted.
         */
         overflow_expected = output_len + OUTPUT_LEN > output_max_num;
         line_ended = end_of_line || end_of_file;

         if (line_ended) {
            if (strncmp(output, tests[i].output + offset, output_len) != 0) {
               printf(
                  "test: \"%s\": failed.\n"
                  "\texpected: %s\n"
                  "\tactual: %s\n",
                  tests[i].name, tests[i].output + offset, output);
               str_not_equal = true;
               break;
            }
            offset += output_len;
         }
         else {
            if (overflow_expected) {
               /*
                  Since the current line which is read from .temp file is not
                  complete, we need to realloc output to have more space.
               */
               output_max_num *= 2;
               output = realloc(output, output_max_num);
               if (output == NULL)
                  raise_err("test: failed to realloc.");
            }

            output_cur_idx += OUTPUT_LEN - 1;
            /* 
               Suppose OUTPUT_LEN = 5. Considering the last character is \0,
               in order to concat the previous string and the next string,
               the \0 from the prev. string must be overwritten.

                  abcd0
               +      efgh0
               ------------
                  abcdefgh0

               Therefore, we need to add (OUTPUT_LEN - 1), rather than OUTPUT_LEN.
            */
         }
      }
      free(output);
      if (!str_not_equal)
         printf("test: \"%s\": passed.\n", tests[i].name);
      
      if (fclose(temp) == EOF)
         raise_err("test: failed to close .temp file.");
   }

   free(tests);
   if (remove(".temp") != 0)
      raise_err("test: failed to remove .temp file.");

   return 0;
}

void raise_err(char *err_msg) {
   fputs(err_msg, stderr);
   putc('\n', stderr);
   exit(EXIT_FAILURE);
}