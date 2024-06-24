#ifndef TEST_TYPES_H
#define TEST_TYPES_H

typedef struct testcase_ {
   char *name;
   char *argv;
   char *input;
   char **outputs;
   int  outputs_len;
} testcase;

typedef struct testresult_ {
   bool result;
   int  idx;
} testresult;

typedef struct evaluate_testcase_params_ {
   const testcase   *tc_ptr;
   const char *actual_output;
   const int   ops_len;
   const int   ao_len;
   testresult *res_arr;
} eval_tc_params;

typedef struct display_testcase_result_params_ {
   const testresult *res_arr;
   const testcase   *tc_ptr;
   const char *actual_output;
   const bool  eval_status;
   const int   res_len;
} disp_tc_res_params;

#endif