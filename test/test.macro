#define TESTCASE_LEN    32
#define READLINE_UNIT   64
#define SYSTEM_FAILED   -1
#define TEST_FAILURE    false
#define TEST_SUCCESS    true

#define TEST(_name, _argv, _input, _outputs_len, ...) \
   if (tlen == TESTCASE_LEN) \
      raise_err("test: reached TESTCASE_LEN."); \
   if (((_outputs_len) <= 0)) \
      raise_err( \
         "test: the field 'expected outputs' has an " \
         "invalid length %d in the testcase \"%s\".", \
         (_outputs_len), (_name)); \
   { \
      testcase *tc = &tests[tlen++]; \
      static char *temp[] = __VA_ARGS__; \
      \
      (tc->name = (_name)); \
      (tc->argv = (_argv)); \
      (tc->input = (_input)); \
      (tc->outputs_len = (_outputs_len)); \
      tc->outputs = temp; \
   }
   