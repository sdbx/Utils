# NSY Specification

The purpose of this specification is to describe overall components which a conforming `nsy` implementation is expected to have.

### Change Log
24.06.10 :: Initial writing.

## Use Case

This program, `nsy`, produces a string full of punctuations, which make the string look greatly *noisy*, hence the program name. Therefore one may find this program useful in cases where the feeling of loud voices are in need, such as:

- to shout/yell/laugh/scream at chat rooms for whatsoever reasons.
- to emphasize/stress something of interest.
- to complain/claim/blame/argue on something. 

## Behavior

A conforming implementation must produce a string, hereafter referred to as **the result**, which satisfies the following conditions:

- The result is a sequence of *valid characters*.

> Definition.  
> A valid character is a character which is not any of white-space characters.

- Any number of multibyte characters can be included in.
  - As such, a conforming implementation should handle multibyte characters in a proper manner.
- A specific delimiter character, determined by the `MARK_KIND` command line argument, must exist between every valid multibyte characters in the string.

> Definition.  
> `MARK_KIND` -> `e` | `q` | `eu` | `qu`
>
> Note: those values are initialisms of the delimiter characters for which the values represent; each values stand for exclamation mark, question mark, upside-down exclamation mark, and upside-down question mark, respectively.

- As a special case, one specific delimiter character must exist at the beginning and the end of the result.

### Example: the result
Suppose the program is executed with `MARK_KIND` being `e` and the input from stdin being "Hello, world!"

Then a conforming implementation must yield the result as follows: "！H！e！l！l！o！,！w！o！r！l！d！!！"

### Error Handling

In case of an error occurred, a conforming implementation must write an relevant, descriptive error message to stderr and terminate immediately, returning an abnormal termination status code.

### Edge Case

It is very easy to imagine a case where there is no input at all from stdin. In this case, a conforming implementation, not having written anything yet to stdout, must follow the error handling behavior described in the Error Handling section.

## I/O

A conforming implementation must take its input from stdin and print the output to stdout.

## Command Line Argument

A conforming implementation must have only one command line argument (except the program name), `MARK_KIND`.

The implementation must follow the error handling behavior described in the Error Handling section in the following situations:

- The number of command line arguments is not equal to 1.
- Failure to match the command line argument to every possible values of `MARK_KIND`.

## Delimiters

A conforming implementation must support the following delimiters: `U+FF01`, `U+FF1F`, `U+00A1`, `U+00BF`. It is within each implementation's discretion to provide other delimiters and such delimiters must have different `MARK_KIND` values against the above standard delimiters.

It would be worth noticing that `U+FF01` and `U+FF1F` is fullwidth characters and thus different with the usual `!` and `?` characters, which are halfwidth characters. There are two reasons why such fullwidth characters have been chosen: The first reason is for readability. Without them, the result string would be quite hard to read. Second, with them, it wouldn't be sufficient for the result string to be noisier, which is highly opposite to what this program attempts to pursue.

However, `U+00A1` and `U+00BF` are halfwidth characters and therefore would be going to harm the readability as is. Considering this issue, a conforming implementation must place a space character in front and behind of these characters.

Meanwhile, as to non-standard delimiters, it's freely up to implementations how they are to print such characters.

### Example: a non-standard delimiter

One implementation may provide U+002E, namely `.`, as an additional delimiter. In such a case, the implementation may choose `p` as one of the possible `MARK_KIND` value so as to differentiate it among other standard ones. Therefore `MARK_KIND` may be defined as follows:
 
> `MARK_KIND` -> `e` | `q` | `eu` | `qu` | `p`

Since this delimiter doesn't disturb the readability, the implementation may choose not to put any padding characters to both sides of it.

## Program Name

The program name is determined by whether the language in which a conforming implementation is written is of compiled or of interpreted.

If a compiled langauge, then the conforming implementation must have its built executable file have the name `nsy`. Otherwise, the conforming implementation must have its source code file have the name `nsy` (including a corresponding extension, if needed).