## Introduction

This program adds `# ` to the beginning of the stdin and prints it.

## Implementations

- C (Does not support trim)
- C++
- JavaScript (Bun)
- Rust

## Build

### C

```sh
clang hd.c -o hd
```

### C++

```sh
clang++ hd.cpp -Oz --std=c++23 -o hd
```

### JavaScript

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