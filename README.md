# Utils

**Utils** - a collection of command line tools focused on producing weird strings

- `btn` - removes whitespaces and makes a line break per a specified number of characters
- `nsy` - inserts a specified character between every character in a string
- `nsy2` - inserts a specified character between every word in a string
- `hhss` - prints arbitrary nonsenses
- `hd` - makes \<h1\> headings
- `yandere` - prints a yandere-like sentence ad infinitum
- `iw` - a helper program to make a message like an installation wizard to stdout

## Installation

It's easy to have **Utils** in your system:

```bash
# install all the programs under /usr/local/bin
$ make install

# remove Utils on your system
$ make uninstall
```

## Usage

**DISCLAIMER: USE ANY OF THESE PROGRAMS AT YOUR OWN RISK**

You can use each program separately. However, it will significantly benefit you to chain one program's output to another program's input.

```bash
# EXAMPLE: generate 5 ridiculous sentence and make them <h1>
$ hhss 5 | hd
```

Once you have weird sentences made just for you, it's time to copy & paste them into the chat room you are in! You'll get other people's attention sooner or later (highly likely sooner) **with your reputation getting a serious damage**.

## Contribution

In general, contributions are achieved via Pull Requests. Above all things, if anything feels unclear, feel free to contact the maintainers: [@logic-finder](https://github.com/logic-finder) and [@lshqqytiger](https://github.com/lshqqytiger).

Each program has its specification, which is a document that describes the behavior of a conforming implementation and that instructs how the program should be implemented. You can find the respective specifications in their folders.

As of now, the above programs have been implemented in various languages. It might be helpful to visit each program's README.md file to be informed of how to build such implementations.

### I would like to fix/modify the current implementations

In this case, if you would like to, you are able to check whether the fixed/modified version of a program still conforms the specification with `test`. Please refer to `./test/README.md` for further information.

Currently, three programs have their respective testcase files: `btn`, `nsy`, and `hd`. These files can be found under `./test/.test` directory.

### I would like to add an implementation

If you should be inclined to add an implementation to whatever programs, it is highly recommended for you to read the specification for the program in advance of the actual development of the program.