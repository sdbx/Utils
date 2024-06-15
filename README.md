# Utils

Last Update: 24.06.15

## Introduction

This repository consists of helper programs which assist you to make a sentence looking somewhat fun, somewhat ridiculous.

### Repository Structure

There are currently 5 programs:

- `btn`, a remover for white-space characters.
- `nsy`, a generator for a noisy sentence.
- `hhss`, a generator for ridiculous sentences.
- `hd`, a generator for a header sentence.
- `yandere`, a generator for a yandere-like sentence.

Note: for `test`, please refer to the below Contribution section.

Each program has its specification, which is a document that describes the behavior of a conforming implementation and that instructs how the program should be implemented. You can find the respective specifications in their folders.

As of now, the above programs have been implemented in various languages. It might be helpful to visit each program's README.md file to be informed of how to build such implementations.

## Contribution

In general, contributions are achieved via Pull Requests.

### I would like to fix/modify the current implementations

In this case, if you would like to, you are able to check whether the fixed/modified version of a source code still conforms the specification with `test`. Please refer to `./test/README.md` for further information.

Currently, three programs have their respective testcase files: `btn`, `nsy`, and `hd`. These files can be found under `./test/.test` directory.

### I would like to add an implementation

If you should be inclined to add an implementation to whatever programs, it is highly recommended for you to read the specification for the program in advance of the actual development of the program.