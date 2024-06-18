# HD Specification

The purpose of this specification is to describe overall components which a conforming `hd` implementation is expected to have.

### Change Log

- 24.06.11 :: Initial writing.
- 24.06.13 :: Minor fix.
- 24.06.18 :: Minor fix.

## Use Case

This program, `hd`, puts a string "# ", which signifies a \<h1\> heading, at the beginning of a line if the line does not start with that string. Thus, one may find this program useful to draw attention to a high extent.

## Behavior

A conforming implementation must produce its output according to the following constraints:

- **Condition 1**. A conforming implementation must skip an empty line.

> Definition.  
> An empty line is a line which only contains either "\r\n" or '\n'.

- **Condition 2**. A conforming implementation must skip a line which already starts with a *h1 string*.

> Definition.  
> A h1 string is a string "# ".

- Unless either Condition 1 or Condition 2 is satisfied, a conforming implementation must put a h1 string in front of a line.
- Any number of multibyte characters can be included in.
  - As such, a conforming implementation should handle multibyte characters in a proper manner.
- A newline character(s) must be placed at the end of the output; refer to the 「Newline」 clause in the Implementation-defined Behavior section.

### Example

Suppose the following lines are given via stdin:

```
Please look at me!!!!!
Hi my name is Cor.
```

Then a conforming implementation must yield the following output:

```
# Please look at me!!!!!
# Hi my name is Cor.
```

### Implementation-defined Behavior

- **「Trim」** A conforming implementation may trim the margins at the left and/or the right of a line, if exist(s).

> Note: white-space characters = { space ( ; 32), horizontal tab (\t; 9), line feed (\n; 10), vertical tab (\v; 11), form feed (\f; 12), carriage return (\r; 13) }

- **「Newline」** A conforming implementation may retain the fashion of line break by which the input from stdin abides.

### Error Handling

In case of an error occurred, a conforming implementation must write a relevant, descriptive error message to stderr and terminate immediately, returning an abnormal termination status code.

### Edge Case

It is very easy to imagine a case where there is no input at all from stdin. In this case, a conforming implementation, not having written anything yet to stdout, must follow the error handling behavior described in the Error Handling section.

## I/O

A conforming implementation must take its input from stdin and print the output to stdout.

## Command Line Argument

A conforming implementation doesn't take any command line arguments.

## Program Name

The program name is determined by whether the language in which a conforming implementation is written is of compiled or of interpreted.

If a compiled langauge, then the conforming implementation must have its built executable file have the name `hd`. Otherwise, the conforming implementation must have its source code file have the name `hd` (including a corresponding extension, if needed).