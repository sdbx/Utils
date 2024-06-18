# NSY2 Specification

Since I've realized that documentation is a very tired and painstaking task, this time I'm not going to write this one as thoroughly as the previous ones.

## Behavior

While `nsy` places a delimiter between letters, `nsy2` places between *words*. That is, white-space character(s) between print character(s) will be replaced as a delimiter.

### things to consider

- input: stdin, output: stdout.
- argv[1] = MARK_KIND -> e | q | eu | qu.
- MARK_KIND determines the type of delimiter to put.
- put one delimiter in front of/behind the output.
- put `\n` at the end of the output.