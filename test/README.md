## Introduction

`test` / `test_maker`: auxiliary programs for test automation.

## Mechanism

These programs require two things in order to run: (1) an executable of a program to be tested (2) a testcase file.

A testcase file has test cases to be used during a test. It must have the following shape, since this file is to be included in a `test.c` source code file, which will be compiled soon.

```
add(tests, &tlen, (testcase) { .name = "test 1", .argv = "...", .input = "...", .output = "..." });
add(tests, &tlen, (testcase) { .name = "test 2", .argv = "...", .input = "...", .output = "..." });
...
add(tests, &tlen, (testcase) { .name = "test n", .argv = "...", .input = "...", .output = "..." });
```

- name = the name of a test case.
- argv = command line arguments to pass to the program.
- input = an input to pass to the program via stdin.
- output = an expected output of the program via stdout.
- tlen = the total number of test cases.

The below diagram shows how they work:

```
gcc -o test_maker test_maker.c
   │
   ↓
┌────────────┐
│ test_maker │ ←─ argv[1] = the path to the testcase file.
└────────────┘
   │
   ↓
┌──────┐
│ test │ ←─ argv[1] = the path to the executable.
└──────┘
```