# YANDERE Specification

The purpose of this specification is to describe overall components which a conforming `yandere` implementation is expected to have.

### Change Log
24.06.10 :: Initial writing.

## Use Case

As its name implies, this program prints a yandere-like string to stdout, *ad infinitum*. Therefore one may find this program useful to experience(?) how it feels like to be targeted by a yandere.

## Behavior

A conforming implementation must keep printing only a yandere-like string until the program is terminated by keyboard interrupt. The exact content of the string is implementation-defined, only if it is yandere-like.

> Definition.  
> a yandere-like string is a sentence which is likely to be said by a yandere. It might be helpful to provide some examples:
> - "Look at only me"
> - "Don't run away from me"
> - "Did you think you can leave me?"

Note: If a yandere-like string doesn't have a trailing space character, the output wouldn't look very neat.

### Example

The following depicts a session of this program with "Did you think you can leave me? " as a yandere-like string:

```
$ ./yandere
Did you think you can leave me? Did you think you can leave me? Did you thi
nk you can leave me? Did you think you can leave me? Did you think you can 
leave me? Did you think you can leave me? Did you think you can leave me? D
id you think you can leave me? ^C
```

## I/O

A conforming implementation does not use stdin; it must print to stdout.

## Command Line Argument

A conforming implementation does not use command line arguments.

## Program Name

The program name is determined by whether the language in which a conforming implementation is written is of compiled or of interpreted.

If a compiled langauge, then the conforming implementation must have its built executable file have the name `yandere`. Otherwise, the conforming implementation must have its source code file have the name `yandere` (including a corresponding extension, if needed).