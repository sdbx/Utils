## Introduction

This program generates a "noisy" sentence by including an exclamation/question mark between each characters.

## Usage

```
./nsy < TEXT_FILE_NAME

where
   TEXT_FILE_NAME = the file name to be input-redirected.
```

## Implementations

### C

```
make
```

### Python

```
python nsy.py < TEXT_FILE_NAME
```

### Haskell

```
ghc noisy.hs 
```

### [HIP](https://github.com/ROCm/HIP)

It can be cross-compiled.

- Any NVIDIA GPUs that support CUDA >=6.0 are supported.
- Any AMD GPUs that support ROCm >=4.2 are supported.

In Linux,

```
make all
```

In Windows, use Visual Studio 2022.