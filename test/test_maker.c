#include "test_maker.h"

int main(int argc, char **argv) {
   check_correct_execution(argc);

   char *dest_filename
      = make_dest_filename(argv[1]);

   preprocess_testcase(argv[1], dest_filename);
   write_modified_file(dest_filename);
   build_executable();
   cleanup(dest_filename);
   
   // the reason I've bothered to use atexit()
   // is that it feels like I've become a gosu
   atexit(announce);
   return 0;
}

void raise_err(const char *err_msg, ...) {
   va_list ap;
   
   va_start(ap, err_msg);
   vfprintf(stderr, err_msg, ap);
   va_end(ap);
   fprintf(stderr, "\n");
   exit(EXIT_FAILURE);
}

void check_correct_execution(int argc) {
   if (argc != 2)
      raise_err(
         "test_maker: usage:\n"
         "\t./test_maker\n"
         "\t\tPATH_TEST_CASES");
}

char *make_dest_filename(const char *testcase_filename) {
   const int filename_len = strlen(testcase_filename);
   const int postfix_len = strlen(TESTCASE_FILE_POSTFIX);
   char *dest_filename
      = malloc((filename_len + postfix_len + 1) * sizeof(char));

   if (dest_filename == NULL)
      raise_err("test_maker: failed to malloc.");
   (void) strncpy(dest_filename, testcase_filename, filename_len);
   (void) strncat(dest_filename, TESTCASE_FILE_POSTFIX, postfix_len);
   
   return dest_filename;
}

void preprocess_testcase(const char *src_name, const char *dest_name) {
   FILE *src, *dest;

   src = fopen(src_name, "r");
   if (src == NULL)
      raise_err("test_maker: failed to open %s.", src_name);
   dest = fopen(dest_name, "w");
   if (dest == NULL)
      raise_err("test_maker: failed to open %s.", dest_name);

   int lengths[TESTCASE_LEN];
   
   count_outputs_len(lengths, src);
   rewind(src);
   write_modified_testcase(lengths, src, dest);

   if (fclose(src) == EOF)
      raise_err("test_maker: failed to close %s.", src_name);
   if (fclose(dest) == EOF)
      raise_err("test_maker: failed to close %s.", dest_name);
}

void count_outputs_len(int lengths[], FILE *src) {
   int ch;
   int num_dq_mark = 0;   // double quotation
   bool escape_char_flag = false;
   int num_testcases = 0;
   
   while ((ch = getc(src)) != EOF) {
      if (escape_char_flag)
         escape_char_flag = false;
      else
         if (ch == '\\')   /* '\' */
            escape_char_flag = true;
         else if (ch == '\"')
            num_dq_mark++;

      // That the number of dq marks are even
      // means the outside of a string.
      if (ch == '}' && num_dq_mark % 2 == 0) {
         const int outputs_len
            = (num_dq_mark - NUM_DQ_MARKS_BEFORE_OUTPUTS_ARR) / 2;
         
         if (num_testcases == TESTCASE_LEN)
            raise_err("test_maker: reached TESTCASE_LEN.");
         lengths[num_testcases++] = outputs_len;
         num_dq_mark = 0;
      }
   }
   if (ferror(src))
      raise_err("test: an error occurred while getc().");
}

void write_modified_testcase(int lengths[], FILE *src, FILE *dest) {
   int ch;
   int num_dq_mark = 0;
   bool escape_char_flag = false;
   int num_testcases = 0;

   while ((ch = getc(src)) != EOF) {
      if (escape_char_flag)
         escape_char_flag = false;
      else
         if (ch == '\\')   /* '\' */
            escape_char_flag = true;
         else if (ch == '\"')
            num_dq_mark++;

      int return_value;

      return_value = putc(ch, dest);
      if (return_value == EOF)
         raise_err("test_maker: an error occurred.");

      if (ch == ',' && num_dq_mark == NUM_DQ_MARKS_BEFORE_OUTPUTS_ARR) {
         return_value = fprintf(dest, "\n   %d,", lengths[num_testcases++]);
         if (return_value < 0)
            raise_err("test_maker: an error occurred.");
      }

      if (ch == '}' && num_dq_mark % 2 == 0)
         num_dq_mark = 0;
   }
   if (ferror(src))
      raise_err("test: an error occurred while getc().");
}

void write_modified_file(const char *testcase_modified_filename) {
   FILE *src, *dest;

   src = fopen("test.c", "r");
   if (src == NULL)
      raise_err("test_maker: failed to open test.c.");
   dest = fopen("test.modified.c", "w");
   if (dest == NULL)
      raise_err("test_maker: failed to open test.modified.c.");
   
   const int directive_len = strlen(TM_INCLUDE);
   char *line;
   int line_cur_idx, line_max_num;

   line_cur_idx = 0;
   line_max_num = LINE_LEN;
   line = malloc(line_max_num * sizeof(char));
   if (line == NULL)
      raise_err("test_maker: failed to malloc.");
   
   while (fgets(line + line_cur_idx, LINE_LEN, src) != NULL) {
      const int line_len = strlen(line);
      const bool end_of_line = line[line_len - 1] == '\n';
      const bool end_of_file = feof(src) ? true : false;

      bool overflow_expected, line_ended;

      overflow_expected = line_len + LINE_LEN > line_max_num;
      line_ended = end_of_line || end_of_file;

      if (line_ended) {
         int return_value;

         if (strncmp(line, TM_INCLUDE, directive_len) == 0) {
            return_value = fprintf(
               dest,
               "   #include \"%s\"\n",
               testcase_modified_filename);
            if (return_value < 0)
               raise_err("test_maker: an error occurred.");
         }
         else {
            return_value = fputs(line, dest);
            if (return_value == EOF)
               raise_err("test_maker: an error occurred.");
         }
         line_cur_idx = 0;
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
   if (ferror(src))
      raise_err("test: an error occurred while fgets().");
   free(line);

   if (fclose(src) == EOF)
         raise_err("test_maker: failed to close test.c.");
   if (fclose(dest) == EOF)
         raise_err("test_maker: failed to close test.modified.c.");
}

void build_executable(void) {
   int status_code;

   status_code = system("gcc -o test test.modified.c");
   if (status_code == SYSTEM_FAILED)
         raise_err("test_maker: system() failed.");
   if (WIFEXITED(status_code)) {
      status_code = WEXITSTATUS(status_code);
      if (status_code != EXIT_SUCCESS)
         raise_err(
            "test_maker: system() returned with exit status %d.\n",
            status_code);
   }
   else
      raise_err("test: WIFEXITED = false.\n");
}

void cleanup(char *dest_filename) {
   if (remove(dest_filename) != 0)
      raise_err("test_maker: failed to remove %s.", dest_filename);
   free(dest_filename);
   if (remove("test.modified.c") != 0)
      raise_err("test_maker: failed to remove test.modified.c file.");
}

void announce(void) {
   if (puts("test_maker: done.") == EOF)
      raise_err("test_maker: an error occurred.");
}