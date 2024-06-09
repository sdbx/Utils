## Introduction

This program prints some "ridiculous" sentences to stdout, given two data files: hsr.dat and usr.dat. Meanwhile, it would be a good thing to use this program with the other programs: btn and/or nsy via pipelines.

P.S. hhss stands for hweng-seol-su-seol, which means gibberish in Korean.

## Requirement

hsr.dat has to have at least 5 valid lines and usr.dat at least 1 valid line. A valid line is a line which is neither a comment nor an empty line. A comment line starts with a # character. A valid line can have more than one `${user}` string in it, which is to be replaced with a random valid line in usr.dat.

## Implementations

- C/C++
- Haskell

## Build

### C

```
make
```

### C++

It can be cross-compiled using CMake.

```
mkdir build
cd build
cmake ..
make
```

## Usage

```
./hhss NUM_SENTENCES

where
   NUM_SENTENCES = the number of sentences to print out.
```
