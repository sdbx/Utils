#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/wait.h>

#define LINE_LEN  1024
#define TM_INCLUDE   "   §TESTCASE§\n"
#define SYSTEM_FAILED   -1

void raise_err(char *err_msg);

int main(int argc, char **argv) {
   if (argc != 2)
      raise_err(
         "test_maker: usage:\n"
         "\t./test_maker\n"
         "\t\tPATH_TEST_CASES");

   FILE *src, *dest;

   src = fopen("test.c", "r");
   if (src == NULL)
      raise_err("test_maker: failed to open test.c.");
   dest = fopen("test.modified.c", "w");
   if (dest == NULL)
      raise_err("test_maker: failed to open test.modified.c.");
   
   /* Write test.modified.c. */
   const int directive_len = strlen(TM_INCLUDE);
   char *line;
   int line_cur_idx, line_max_num;

   line_cur_idx = 0;
   line_max_num = LINE_LEN;
   line = malloc(line_max_num * sizeof(char));
   if (line == NULL)
      raise_err("test_maker: failed to malloc.");
   
   /*
      A rough explanation of the below while statement:

      1. Get a line using fgets.
      2. If the line is ended with \n, i.e. that is the
         whole line, do 2.1 or 2.2.
         2.1. If the line is the same as TM_INCLUDE,
               print "#include ..." instead.
         2.2. Else, print the line itself.
      3. Else, i.e. if the line is not complete,
         3.1. If the line does not have enough space to hold
               the result of the next fgets() call, realloc.
         3.2. Prepare the next fgets() call.
   */

   while (fgets(line, LINE_LEN, src) != NULL) {
      const int line_len = strlen(line);
      int ch;
      bool end_of_line, end_of_file;
      bool overflow_expected, line_ended;

      if ((ch = fgetc(src)) == EOF)
         end_of_file = true;
      else {
         end_of_file = false;
         ungetc(ch, src);
      }
      end_of_line = line[line_len - 1] == '\n';

      overflow_expected = line_len + LINE_LEN > line_max_num;
      line_ended = end_of_line || end_of_file;

      if (line_ended) {
         if (strncmp(line, TM_INCLUDE, directive_len) == 0)
            fprintf(dest, "   #include \"%s\"\n", argv[1]);
         else
            fputs(line, dest);
      }
      else {
         if (overflow_expected) {
            line_max_num *= 2;
            line = realloc(line, line_max_num);
            if (line == NULL)
               raise_err("test_maker: failed to malloc.");
         }
         line_cur_idx += LINE_LEN - 1;
      }
   }
   free(line);

   if (fclose(src) == EOF)
         raise_err("test_maker: failed to close test.c.");
   if (fclose(dest) == EOF)
         raise_err("test_maker: failed to close test.modified.c.");

   /* Build the executable. */
   int status_code;

   status_code = system("gcc -o test test.modified.c");
   if (status_code == SYSTEM_FAILED)
         raise_err("test_maker: system() failed.");
   if (WIFEXITED(status_code)) {
      status_code = WEXITSTATUS(status_code);
      if (status_code != EXIT_SUCCESS) {
         fprintf(stderr,
            "test_maker: system() returned with exit status %d.\n",
            status_code);
         
         exit(EXIT_FAILURE);
      }
   }
   else
      raise_err("test: WIFEXITED = false.\n");

   if (remove("test.modified.c") != 0)
      raise_err("test_maker: failed to remove test.modified.c file.");

   return 0;
}

void raise_err(char *err_msg) {
   fputs(err_msg, stderr);
   putc('\n', stderr);
   exit(EXIT_FAILURE);
}