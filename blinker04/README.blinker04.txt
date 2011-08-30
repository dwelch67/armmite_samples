See the top level README file for information about the users guide
and schematic.s

This sample is the same as the one before with the addition of using
the LLVM tools.

The way I use llvm is to compile the .C files into bitcode files (I use
.bc for these), specifically NOT optimizing (yet).  If you have multiple
source files then use llvm-link to combine the individual, unoptimized,
bitcode files into a single file.  Then, if desired, optimize one .bc
file with the output being another bitcode file.  Then use llc to
compile the bitcode into assembly language for the specific target,
this is the first time the target gets involved (except for the -m32
used with clang to keep to 32 bits if on a 64 bit machine).  binutils
is used to assemble this and then link it with startup.o and any other
components that were written originally in assembly language.

The Makefile in this example creates both an optimized and unoptimized
program.  With such a simple starting point the optimization opportunities
are minimal.  But the foundation is here.

Loading the program and running it is described in the top level
README.
