# HHSS Specification

The purpose of this specification is to describe overall components which a conforming `hhss` implementation is expected to have.

### Change Log

- 24.06.10 :: Initial writing.
- 24.06.13 :: Minor fix.
- 24.06.14 :: Minor fix.
- 24.06.15 :: Removal of the irrelevant Edge Case section.

## Use Case

This program, `hhss`, produces multiple sentences randomly chosen, based on two data files given. One data file consists of sentences and a sentence may have a template to be replaced by the contents in another data file. Since this program is intended to be used as a *ridiculous* sentence generator, it is highly suggested for those data files to have fun and silly contents.

## Behavior

A conforming implementation must produce `NUM_SENTENCES` lines of sentence under the following constraints:

> Definition.  
> `NUM_SENTENCES` is of type `int` and has a value greater than or equal to 5 and less than the number of sentences acquired from a sentence file.

> Definition.  
> A sentence file is a text file composed of at least 5 sentences and arbitrary number of comments and empty lines.  
> The name of a sentence file must be "hsr.dat".

> Definition.  
> A sentence is a line which is neither a comment nor an empty line.  
> A sentence can have arbitrary number of *user templates* in it.

> Definition.  
> A comment is a line which starts with '#' character.

> Definition.  
> An empty line is a line which only contains either "\r\n" or '\n'.

> Definition.  
> A user template is a string "${user}".

- The implementation must take sentences to utilize in this program from a *sentence file*.

- The implementation must take user names to utilize in this program from a *user file*.

> Definition.  
> A user file is a text file composed of at least one user name and arbitrary number of comments and empty lines.
> The name of a user file must be "usr.dat".

> Definition.  
> A user name is a line which is neither a comment nor an empty line.

- The implementation must choose `NUM_SENTENCES` sentences in a random fashion from the sentences acquired from a sentence file.

- The implementation must replace every occurrence of user templates in chosen sentences with a user name randomly chosen among the user names acquired from the user file.
   - Unless only one user name is available, user name currently picked must not be overlapped with a user name picked previously. This condition holds, irrespective of individual sentences.

### Example: ridiculous sentences

Suppose the following strings are given as a category they are in:

```
// As a sentence
${user}: H3Y ${user}, C 15 R1LLY C00L!!!!!
${user}: No, programming in C will definitely harm the quality of your life.

// As a user name
COR
A SANE PERSON
```

The below is one possible result which can be produced by a conforming implementation:

```
COR: H3Y A SANE PERSON, C 15 R1LLY C00L!!!!!
COR: No, programming in C will definitely harm the quality of your life.
```

This example is illustrative in that the previous user name and the current user name is not the same.

### Error Handling

In case of an error occurred, a conforming implementation must write a relevant, descriptive error message to stderr and terminate immediately, returning an abnormal termination status code.

## I/O

A conforming implementation does not take any input from stdin, while the implementation must print the output to stdout.

## Command Line Argument

A conforming implementation must have only one command line argument (except for the program name), `NUM_SENTENCES`.

The implementation must follow the error handling behavior described in the Error Handling section in the following situations:

- The number of command line arguments is not equal to 1.
- Failure to get the command line argument.
- `NUM_SENTENCES` greater than the number of sentences acquired from a sentence file.
- `NUM_SENTENCES` less than 5.

## Program Name

The program name is determined by whether the language in which a conforming implementation is written is of compiled or of interpreted.

If a compiled langauge, then the conforming implementation must have its built executable file have the name `hhss`. Otherwise, the conforming implementation must have its source code file have the name `hhss` (including a corresponding extension, if needed).

## Further Research

- It would be nice to support other replacement templates in addition to the current ${user}.
