# libdstruct
Datastructure library for C language, written in ANSI C with complete set of Unit tests and full demo samples.
Provides a complete interface to fundamental data-structres.

Build and tested on FreeBSD and OpenBSD, NetBSD (Should work perfectly on Linux) using Clang and gcc.

## Types
Currently the library supports the below types:

- List
- Queue
- Stack
- Hash Map
- Set
- Array Stack
- Aarray Queue

For use please check the The **test** and the **demos** folders.

### Build
$ cd src

$ make

### Build/Run Test
(requires libcheck https://github.com/libcheck/check)

$ cd test

$ make

### Installation
$ cd src

$ sudo make install clean

### Deinstallation
$ cd src

$ sudo make deinstall

This will build both static and dynamic libraries. 

**************************************************



