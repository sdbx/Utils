#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/wait.h>

#define COMMAND_LEN  512
#define TESTCASE_LEN 32
#define OUTPUT_LEN   1024

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
         "\t\tPATH_PROGRAM_SOURCE_CODE");
   if (!system(NULL))
      raise_err("test: no command processor available.");

   /* Build a program to be tested. */
   int statcode, retval;
   char *command;
   
   command = malloc(COMMAND_LEN);
   if (command == NULL)
      raise_err("test: failed to malloc.");

   /* Note: compile differently as per implementation? */
   retval = snprintf(command, COMMAND_LEN, "gcc -o object_program %s", argv[1]);
   if (retval < 0)
      raise_err("test: encoding error.");
   if (command[strlen(command) - 1] != 'c')
      raise_err("test: wrong command.");
   
   statcode = system(command);
   if (statcode == 1)
      raise_err("test: system() failed.");
   if (WIFEXITED(statcode)
         && WEXITSTATUS(statcode) == EXIT_FAILURE)
      raise_err("test: abnormal termination.");
   
   /* Prepare to include test cases. */
   testcase *tests;
   int tlen;
   
   tests = malloc(TESTCASE_LEN * sizeof(testcase));
   if (tests == NULL)
      raise_err("test: failed to malloc.");
   
   /* Include test cases. Ex: #include "nsy.test" */
   test_maker::include

   /* Perform tests. */
   char *output;

   output = malloc(OUTPUT_LEN);
   if (output == NULL)
      raise_err("test: failed to malloc.");
   
   for (int i = 0; i < tlen; i++) {
      FILE *temp;
      
      temp = fopen(".temp", "w+");  /* truncates the file content. */
      if (temp == NULL)
         raise_err("test: failed to open .temp file.");

      retval = snprintf(
         command,
         COMMAND_LEN,
         "./object_program %s >.temp <<EOF\n%s\nEOF",
         tests[i].argv,
         tests[i].input);
      if (retval < 0)
         raise_err("test: encoding error.");

      statcode = system(command);
      if (statcode == -1)
         raise_err("test: system() failed.");
      if (WIFEXITED(statcode))
         if (WEXITSTATUS(statcode) == EXIT_FAILURE)
            raise_err("test: abnormal termination.");
      
      int offset;
      bool str_not_equal;

      offset = 0;
      str_not_equal = false;
      while (fgets(output, OUTPUT_LEN, temp) != NULL) {
         int output_len;

         output_len = strlen(output);
         if (strncmp(output, tests[i].output + offset, output_len) != 0) {
            printf("test: \"%s\": failed.\n\texpected: %s\n\tactual: %s\n",
               tests[i].name, tests[i].output + offset, output);
            str_not_equal = true;
            break;
         }
         offset += output_len;
      }
      if (!str_not_equal)
         printf("test: \"%s\": passed.\n", tests[i].name);
      
      if (fclose(temp) == EOF)
         raise_err("test: failed to close .temp file.");
   }

   if (remove(".temp") != 0)
      raise_err("test: failed to remove .temp file.");
   if (remove("object_program") != 0)
      raise_err("test: failed to remove object_program file.");

   return 0;
}

void raise_err(char *err_msg) {
   fputs(err_msg, stderr);
   putc('\n', stderr);
   exit(EXIT_FAILURE);
}