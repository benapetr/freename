freename
===========

This is an extremely simple and fast C utility written for UNIX,
its purpose is to quickly find an available file name for use in
shell scripts and so on.

It's a faster replacement for alternative hack in shell script,
which is aimed to be more flexible in the future.

Basic usage is to provide a filename as an argument with optional
string(s) `$counter` that will be replaced with an actual counter.

If counter variable is not provided, filename has .$counter appended.

Examples:
```
# No files "x" exist:
$ ./freename x
x
$ touch x
$ ./freename x
x.0
$ touch x.0
$ ./freename x
x.1
$ touch `./freename x`
$ touch `./freename x`
$ ./freename x
x.3
```
