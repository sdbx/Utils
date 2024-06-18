# BTN Specification

The purpose of this specification is to describe overall components which a conforming `btn` implementation is expected to have.

### Change Log

- 24.06.10 :: Initial writing.
- 24.06.13 :: Minor fix.
- 24.06.14 :: Minor fix.
- 24.06.18 :: Minor fix.

## Use Case

This program, `btn`, produces a string with white-space characters removed, which make the string look so *condensed* that eventually it becomes highly distinguishable from other usual, normal strings. Therefore, one may find this program useful in cases where one is in need of attention of other participants in a chat room.

## Behavior

A conforming implementation must produce a string, hereafter referred to as **the result**, which satisfies the following conditions:

- The result is a sequence of *valid characters*.

> Definition.  
> A valid character is a character which is not any of white-space characters.
>
> Note: white-space characters = { space ( ; 32), horizontal tab (\t; 9), line feed (\n; 10), vertical tab (\v; 11), form feed (\f; 12), carriage return (\r; 13) }

- Any number of multibyte characters can be included in.
  - As such, a conforming implementation should handle multibyte characters in a proper manner.
- The string must have a line feed character `\n` after every `N` numbers of valid multibyte characters.
- A line feed character must be placed at the end of the result string, unless one have been already put (because of the above condition).

> Definition.  
> `N` is of type `int` and has a value between 1 ~ 32 (inclusive).

### Example: the result

Suppose the program is executed with `N` being 10 and the input from stdin being "I want to sleep... I didn't know documentation is very difficult..."

Then a conforming implementation must yield the result as follows:

```
// This numbers are for showing each line exactly
// has 10 characters and thus do not have to do
// with the output of the program.
0123456789

Iwanttosle
ep...Ididn
'tknowdocu
mentationi
sverydiffi
cult...
```

### Error Handling

In case of an error occurred, a conforming implementation must write a relevant, descriptive error message to stderr and terminate immediately, returning an abnormal termination status code.

### Edge Case

It is very easy to imagine a case where there is no input at all from stdin. In this case, a conforming implementation, not having written anything yet to stdout, must follow the error handling behavior described in the Error Handling section.

## I/O

A conforming implementation must take its input from stdin and print the output to stdout.


## Command Line Argument

A conforming implementation must have only one command line argument (except for the program name), `N`.

The implementation must follow the error handling behavior described in the Error Handling section in the following situations:

- The number of command line arguments is not equal to 1.
- Failure to get the command line argument.
- `N` not in the range of [1, 32].

## Program Name

The program name is determined by whether the language in which a conforming implementation is written is of compiled or of interpreted.

If a compiled langauge, then the conforming implementation must have its built executable file have the name `btn`. Otherwise, the conforming implementation must have its source code file have the name `btn` (including a corresponding extension, if needed).