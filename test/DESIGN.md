# TEST Design

The purpose of this document is to articulate the overall design of the `test` program.

### Change Log

24.06.11 :: Initial writing.

## Conceptual Design

In this section address essential questions that must be resolved before the actual development begins.

### The Raison D'être

As of the time this document is written, there are five programs in total. In this reason, now it could be burdensome to inspect each program manually:

- Does this program conform to the its specification?
- Does this program produce expected outputs for inputs that represent various situations?

In other words, the very problem is inconvenience; therefore it is contemplated that this is a fairly nice timing when an automation tool is able to come into play. However, a question of how such a test can be automated still remains unsolved. Hence this question leads us to the next section.

### Test Automation

At here, let's clarify the meaning of the so-called *test*.

> Definition.  
> A test is an act of examining an *object program* to check whether it demonstrates a *correct behavior* under the supervision of a *host program*.

> Definition.  
> An object program is a program which is the object of a test.

> Definition.  
> A correct behavior is a successful case that an object program yields an expected output for a predefined input.

> Definition.  
> A host program is a program which conducts a test.

From this analysis of the definition of *test*, therefore, now it is obvious that we need three components in order to perform a test: an object program, a host program, and a *test representation*.

> Definition.  
> A test representation is an abstract idea that consists of two elements: a *predefined input* and an *expected output*.

> Definition.  
> A predefined input is an abstract idea that denotes what is to be fed to a n object program.

> Definition.  
> An expected output is an abstract idea that denotes what is to be produced by an object program.

---

Since a rough conception has been completed, now it is time to elaborate the design.

## Detail Design

In this section describes a more sophisticated design. From here, we'll first discuss on what components the program is made of and how those components communicate. After that, we'll think of a specification of the program.

### Program Structure

I think it is a good idea to apply modular design into the program because it allows easier maintenance. From this reason, the program is divided into two parts: a test-perfoming part and a test-defining part. I'm going to explain the latter first.

#### Test-defining Part

As the name implies, this part consists of *test cases*. Test cases will be used in the *test-performing part* later.

> Definition.  
> A test case is a concrete reality which expresses a *test representation*. It depends on each implementation how it is to be implemented.

#### Test-performing Part

As the name suggests, this part performs actual tests in accordance with *test cases*.

### Specification

#### Behavior

A conforming implementation must perform test cases on an object program and notify the result of the test via stdout.

It is up to implementations that the form of the notification about the test result, but a conforming implementation must include the following information:

- Which test cases were failed, if any?
- If a test case was failed, then why?

#### Error Handling
In case of an error occurred, a conforming implementation must write a relevant, descriptive error message to stderr and terminate immediately, returning an abnormal termination status code.

#### I/O

A conforming implementation does not take any inputs from stdin, while it must print the output to stdout.

#### Command Line Argument

A conforming implementation may take command line argument(s) in order to run.

#### Program Name

The program name is determined by whether the language in which a conforming implementation is written is of compiled or of interpreted.

If a compiled langauge, then the conforming implementation must have its built executable file have the name `test`. Otherwise, the conforming implementation must have its source code file have the name `test` (including a corresponding extension, if needed).

---

Now the hour of development has arrived, and I have no choice but to proceed. The fate of this program&mdash;the future only knows.

## Actual Implementation

This section shows a rough implementation of this program in C.

```c
// file: test.c

#define COMMAND_LEN  512
#define TESTCASE_LEN 32
#define OUTPUT_LEN   1024

typedef struct {
   char *name;
   char *argv;
   char *input;
   char *output;
} testcase;

int main(int argc, char *argv[]) {
   // Suppose that
   //    argv[1] = a command to compile the
   //    source code of a program to be tested.

   // Build the program.
   system(argv[1]);

   // Prepare test cases.
   testcase arr[adequate_num];
   int num_test_case;

   // Please refer to the below for the contents in this file.
   #include "test_cases"

   // Perform tests.
   for (int i = 0; i < num_test_case; i++) {
      char command[adequate_num];
      FILE *cmd_result;

      // Suppose the name of the program is btn.
      // Execute btn and save the result at .temp file.
      cmd_result = fopen(".temp", "w+");
      snprintf(
         command,
         "btn %s > %s < %s",
         arr[i].argv, arr[i].input,
         ".temp");
      system(command);

      // Read .temp file and compare the actual output
      // with the expected output.
      char line[adequate_num];

      fgets(line, adequate_num, cmd_result);
      if (strcmp(line, arr[i].output) == 0)
         // test passed
      else
         // test failed
   }
}
```

Note: for the sake of convenience of implementation, this program relies on three macros. The values outside of these limits break the assumptions that this program has and therefore will cause undefined behaviors.

```c
// file: test_cases

arr[0] = (testcase) { .name = "", .argv = "", .input = "", .output = "" };
num_test_case = 1;
```

However, it can be difficult to change the `#include` preprocessor directive and build the program manually whenever different tests are needed. Therefore, one may find a program handy which modifies the `#include` directive depending on a file name passed to argv and compile the modified source code programmatically for you.