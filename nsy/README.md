## Introduction

This program generates a sentence including an exclamation/question mark between each characters.

## Implementations

- C
- Haskell
- Python
- [HIP](https://github.com/ROCm/HIP)

## Build

### C

```
make
```

### HIP

It can be cross-compiled.

Any NVIDIA GPUs that support CUDA >=6.0 are supported.

Any AMD GPUs that support ROCm >=4.2 are supported.

In Linux,

```
make
```

In Windows, use Visual Studio 2022.

## Usage

```
./nsy < TEXT_FILE_NAME

where
   TEXT_FILE_NAME = the file name to be input-redirected.
```
