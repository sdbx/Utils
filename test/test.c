#include "test.h"

int main(int argc, char **argv) {
   check_correct_execution(argc);
   
   int testcases_len;

   testcase *testcases = acquire_testcases(&testcases_len);
   const bool test_status = perform_test(testcases, testcases_len, argv[1]);
   
   free(testcases);
   return test_status
            ? EXIT_SUCCESS
            : EXIT_FAILURE;
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
         "test: usage:\n"
         "\t./test\n"
         "\t\tPATH_EXECUTABLE");
   if (!system(NULL))
      raise_err("test: no command processor available.");
}

testcase *acquire_testcases(int *testcases_len_ptr) {
   testcase *tests;
   int tlen;
   
   tests = malloc(TESTCASE_LEN * sizeof(testcase));
   if (tests == NULL)
      raise_err("test: failed to malloc.");
   tlen = 0;
   
   // This line will be replaced to something like #include "nsy.test".
   §TESTCASE§

   if (tlen == 0)
      raise_err("test: testcase length of 0; terminating.");

   *testcases_len_ptr = tlen;
   return tests;
}

bool perform_test(testcase *testcases, int testcases_len, const char *executable_name) {
   int successful_testcase_count;

   successful_testcase_count = 0;
   for (int i = 0; i < testcases_len; i++) {
      FILE *temp = fopen(".temp", "w+");
      if (temp == NULL)
         raise_err("test: failed to open .temp file.");

      char *command
         = make_command(&testcases[i], executable_name);
      execute_command(command);
      free(command);

      char *actual_output = acquire_output(temp);
      const int ao_len = strlen(actual_output);
      const int ops_len = testcases[i].outputs_len;
      testresult results[ops_len];

      const eval_tc_params eval_tc_params = {
         .res_arr = results,
         .tc_ptr = &testcases[i],
         .actual_output = actual_output,
         .ops_len = ops_len,
         .ao_len = ao_len
      };
      const bool eval_status = evaluate_testcase(&eval_tc_params);
      if (eval_status)
         successful_testcase_count++;

      const disp_tc_res_params disp_tc_res_params = {
         .res_arr = results,
         .tc_ptr = &testcases[i],
         .actual_output = actual_output,
         .eval_status = eval_status,
         .res_len = ops_len
      };
      display_testcase_result(&disp_tc_res_params);

      free(actual_output);
      if (fclose(temp) == EOF)
         raise_err("test: failed to close .temp file.");
   }
   if (remove(".temp") != 0)
      raise_err("test: failed to remove .temp file.");

   return successful_testcase_count == testcases_len
            ? true
            : false;
}

char *make_command(const testcase *tc_ptr, const char *executable_name) {
   const testcase tc = *tc_ptr;
   const int cmd_len =
      strlen("  >.temp <<EOF\n\nEOF") +
      strlen(executable_name) +
      strlen(tc.argv) +
      strlen(tc.input) +
      1;  /* for \0 */

   char *command;

   command = malloc(cmd_len * sizeof(char));
   if (command == NULL)
      raise_err("test: failed to malloc.");

   const int return_value = snprintf(
      command,
      cmd_len,
      "%s %s >.temp <<EOF\n%s\nEOF",
      executable_name,
      tc.argv,
      tc.input);
   if (return_value < 0)
      raise_err("test: encoding error.");

   return command;
}

void execute_command(const char *cmd) {
   int status_code;

   status_code = system(cmd);
   if (status_code == SYSTEM_FAILED)
      raise_err("test: system() failed.");
   
   if (WIFEXITED(status_code)) {
      status_code = WEXITSTATUS(status_code);
      if (status_code != EXIT_SUCCESS)
         raise_err(
            "test: system() returned with exit status %d.\n",
            status_code);
   }
   else
      raise_err("test: WIFEXITED = false.\n");
}

char *acquire_output(FILE *tempfile) {
   char *actual_output;
   int ao_cur_idx, ao_max_len;

   ao_max_len = READLINE_UNIT;
   actual_output = malloc(ao_max_len * sizeof(char));
   if (actual_output == NULL)
      raise_err("test: failed to malloc.");
   ao_cur_idx = 0;

   while (fgets(actual_output + ao_cur_idx, READLINE_UNIT, tempfile) != NULL) {
      const int ao_len = strlen(actual_output);
      const bool end_of_line = actual_output[ao_len - 1] == '\n';
      const bool end_of_file = feof(tempfile) ? true : false;

      bool overflow_expected, line_ended;

      overflow_expected = ao_len + READLINE_UNIT > ao_max_len;
      line_ended = end_of_line || end_of_file;

      if (!line_ended) {
         if (overflow_expected) {
            ao_max_len *= 2;
            actual_output = realloc(actual_output, ao_max_len * sizeof(char));
            if (actual_output == NULL)
               raise_err("test: failed to realloc.");
         }
         ao_cur_idx += READLINE_UNIT - 1; // so as to overwrite the trailing \0.
      }
      else if (!end_of_file)
         ao_cur_idx += strlen(actual_output + ao_cur_idx);
   }
   if (ferror(tempfile))
      raise_err("test: an error occurred while fgets().");
   
   return actual_output;
}

bool evaluate_testcase(const eval_tc_params *params_ptr) {
   const eval_tc_params params = *params_ptr;
   const testcase tc = *params.tc_ptr;
   const char *actual_output = params.actual_output;
   const int ops_len = params.ops_len;
   const int ao_len = params.ao_len;

   testresult *res = params.res_arr;
   int success_count;

   success_count = 0;
   for (int k = 0; k < ops_len; k++) {
      char *expected_output = tc.outputs[k];
      const int eo_len = strlen(expected_output);

      if (eo_len != ao_len) {
         res[k].result = TEST_FAILURE;
         if (eo_len > ao_len)
            res[k].idx = ao_len + 1;
         else
            res[k].idx = eo_len;
      }
      else {
         bool different = false;
         int m;

         for (m = 0; m < eo_len; m++)
            if (actual_output[m] != expected_output[m]) {
               different = true;
               break;
            }
         
         if (different) {
            res[k].result = TEST_FAILURE;
            res[k].idx = m + 1;
         }
         else {
            res[k].result = TEST_SUCCESS;
            res[k].idx = m;
            success_count++;
         }
      }
   }

   return success_count > 0
            ? true
            : false;
}

void display_testcase_result(const disp_tc_res_params *params_ptr) {
   const disp_tc_res_params params = *params_ptr;
   const testresult *res = params.res_arr;
   const testcase tc = *params.tc_ptr;
   const char *actual_output = params.actual_output;
   const bool eval_status = params.eval_status;
   const int res_len = params.res_len;

   int return_value;

   return_value = printf(
      "test: %s: %s.\n",
      tc.name,
      eval_status ? "passed" : "failed"
   );
   if (return_value < 0) raise_err("test: an error occurred.");

   if (eval_status) {
      const char *success_msg
         = "\texpected[%d] was equivalent "
           "to the actual output.\n";

      for (int i = 0; i < res_len; i++)
         if (res[i].result == TEST_SUCCESS)
            if (printf(success_msg, i) < 0)
               raise_err("test: an error occurred.");
   }
   else {
      const char *format = " \\\n\t\t\t";

      for (int i = 0; i < res_len; i++) {
         char *formatted
            = convert_linefeed(tc.outputs[i], format, res[i].idx);

         return_value = printf(
            "\texpected[%d] =\t%s "
            "<- different at position %d\n",
            i, formatted, res[i].idx - 1);
         if (return_value < 0)
            raise_err("test: an error occurred.");
         free(formatted);
      }
      char *formatted
         = convert_linefeed(actual_output, format, strlen(actual_output));

      return_value = printf("\tactual =\t%s\n", formatted);
      if (return_value < 0)
         raise_err("test: an error occurred.");
      free(formatted);
   }
}

char *convert_linefeed(const char *src, const char *replace_str, int len) {
   if (len < 0)
      raise_err("test: len (= %d) < 0.", len);
   
   int src_idx, count;

   src_idx = 0;
   count = 0;
   while (src[src_idx] != '\0' && src_idx < len)
      if (src[src_idx++] == '\n')
         count++;

   const int replace_str_len = strlen(replace_str);
   const int dest_len = 1 + src_idx - count + count * replace_str_len;
   char *dest;

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