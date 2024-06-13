#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#define LINE_LEN  1024
#define TM_INCLUDE   "   test_maker::include\n"

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
   dest = fopen("test.modified.c", "w+");
   if (dest == NULL)
      raise_err("test_maker: failed to open test.modified.c.");
   
   /* Write test.modified.c. */
   char *line;

   line = malloc(LINE_LEN);
   if (line == NULL)
      raise_err("test_maker: failed to malloc.");

   int directive_len;
   
   directive_len = strlen(TM_INCLUDE);
   while (fgets(line, LINE_LEN, src) != NULL) {
      if (strncmp(line, TM_INCLUDE, directive_len) == 0)
         fprintf(dest, "   #include \"%s\"\n", argv[1]);
      else
         fputs(line, dest);
   }

   if (fclose(src) == EOF)
         raise_err("test_maker: failed to close test.c.");
   if (fclose(dest) == EOF)
         raise_err("test_maker: failed to close test.modified.c.");

   /* Build the executable. */
   int statcode;

   statcode = system("gcc -o test test.modified.c");
   if (statcode == -1)
         raise_err("test_maker: system() failed.");
   if (WIFEXITED(statcode))
      if (WEXITSTATUS(statcode) == EXIT_FAILURE)
         raise_err("test_maker: abnormal termination.");

   if (remove("test.modified.c") != 0)
      raise_err("test_maker: failed to remove test.modified.c file.");

   return 0;
}

void raise_err(char *err_msg) {
   fputs(err_msg, stderr);
   putc('\n', stderr);
   exit(EXIT_FAILURE);
}