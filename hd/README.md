## Introduction

This program adds the string `"# "` to the beginning of the stdin and prints it, only if it doesn't start with the string.

## Usage

```
./hd TEXT
```

## Implementations

### C

This implementation does not support trim.

```sh
clang hd.c -o hd
```

### C++

```sh
clang++ hd.cpp -Oz --std=c++23 -o hd
```

### JavaScript (Bun)

First, make sure that [the bun](https://bun.sh/docs/installation) is installed.

```sh
bun build hd.js --compile --outfile hd
```

### Rust

```sh
rustc hd.rs -C debuginfo=0 -C opt-level=z -C strip=symbols -o hd
```

## Test

```
bun test
```