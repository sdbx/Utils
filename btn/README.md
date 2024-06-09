## Introduction

This program prints a long string having no white-space characters, given a text file from stdin. Thus a would-be attention seeker may find this program useful for his doing so-called 'buntang,' hence the program name 'btn.'

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