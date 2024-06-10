# BTN Specification

The purpose of this specification is to describe overall components which a conforming `btn` implementation is expected to have.

### Change Log
24.06.10 :: Initial writing.

## Use Case

This program, `btn`, produces a string with white-space characters removed, which make the string look so *condensed* that eventually it becomes highly distinguishable from other usual, normal strings. Therefore one may find this program useful in cases where one is in need of attention of other participants in a chat room.

## Behavior

A conforming implementation must produce a string, hereafter referred to as **the result**, which satisfies the following conditions:

- The result is a sequence of *valid characters*.

> Definition.  
> A valid character is a character which is not any of white-space characters.

- Any number of multibyte characters can be included in.
  - As such, a conforming implementation should handle multibyte characters in a proper manner.
- The string must have a line feed character `\n` after every `N` numbers of valid multibyte characters.

> Definition.  
> `N` is of type `int` and has a value between 1 ~ 32 (inclusive).

### Example: the result
Suppose the program is executed with the input from stdin being "I want to sleep... I didn't know documentation is very difficult..."

Then a conforming implementation must yield the result as follows: "Iwanttosleep...Ididn'tknowdocumentationisverydifficult..."

### Error Handling

In case of an error occurred, a conforming implementation must write an relevant, descriptive error message to stderr and terminate immediately, returning an abnormal termination status code.

### Edge Case

It is very easy to imagine a case where there is no input at all from stdin. In this case, a conforming implementation, not having written anything yet to stdout, must follow the error handling behavior described in the Error Handling section.

## I/O

A conforming implementation must take its input from stdin and print the output to stdout.


## Command Line Argument

A conforming implementation must have only one command line argument (except the program name), `N`.

The implementation must follow the error handling behavior described in the Error Handling section in the following situations:

- The number of command line arguments is not equal to 1.
- Failure to get the command line argument.
- `N` not in the range of [1, 32].

## Program Name

The program name is determined by whether the language in which a conforming implementation is written is of compiled or of interpreted.

If a compiled langauge, then the conforming implementation must have its built executable file have the name `btn`. Otherwise, the conforming implementation must have its source code file have the name `btn` (including a corresponding extension, if needed).