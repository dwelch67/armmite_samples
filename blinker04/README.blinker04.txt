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

This program also modifies the MAM control registers.  This has to
do with how fast flash is read.  By default a safe, slow, mode is used
the user/program hs to know how fast the clock to the ARM is and as
a result a safe fetch speed.  The schematic shows a 20Mhz clock feeding
the part, so we should assume that is our clock rate.  The table shows
that < 20MHz you can set MAMTIM to 1, 20MHz to 40MHz you can go as fast
as a 2.  Assuming 20Mhz we shouldnt go faster than a 2. Try commenting
out the write of 2 to MAMCR, leaving the MAM logic off, and comparing
the blink rate with it on and off.  This MAM timing is something we
definitely want to adjust in our applications for performance reasons.

Loading the program and running it is described in the top level
README.
