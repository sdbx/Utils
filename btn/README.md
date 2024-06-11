## Introduction

This program prints a long string having no white-space characters, given a text file from stdin. Thus a would-be attention seeker may find this program useful for his doing so-called *buntang*s, hence originated the program name `btn`. (Buntang is a Korean word meaning disturbance or commotion; apparently it is used as a kind of the Internet slang nowadays to refer to such confusing misbehaviors themselves or the one who is exerting such dizziness-causing attitudes usually at online communities.)

## Usage

```
./btn N < TEXT_FILE_NAME

where
   N = the number of characters before a new-line character;
       its range: 1 ~ 32,
   TEXT_FILE_NAME = the file name to be input-redirected.
```

## Implementations

### C

```
make
```

### Python

```
python btn.py N < TEXT_FILE_NAME
```

### Haskell

```
ghc buntang.hs
```