See the top level README file for information about the users guide
and schematic.s

This third blinker example is a mixture of assembly language and C.
You will need both binutils and gcc (see top level README file).

This example is the same as the ones before it.  Blinking an led using
a counter based loop to delay.  The difference being this one has
some assembly language startup code and the interesting part of the
program is in C.   Note the reason for the dummy() function is to
prevent the optimizer from removing the otherwise useless delay loop.

Using notmain() instead of main reinforces the fact that this is not
a C program on top of an operating system, but perhaps instead an
assembly language program with some code written in C to make life
easier.

My personal preference, due to experience with all compilers getting
it wrong, is to explicitly make I/O functions.  In this case PUT32
and GET32.  This prevents the compiler from getting it wrong.  Also
this layer of abstraction has many other benefits like testing code
against simulations or using the same code on different operating
systems and environments without modification.

Interestingly if you leave off the -mthumb gcc option the program
still works.  The thumb code calls bl not main, bl is not one of
the instructions that can switch modes from thumb to ARM.  How it
works is that the linker knows thumb from ARM and chooses to call
a trampoline function:

00000100 <__notmain_from_thumb>:
 100:   4778            bx      pc
 102:   46c0            nop                     ; (mov r8, r8)
 104:   eaffffd7        b       68 <notmain>

Instead of the real function.  This trampoline does the switch from
thumb to ARM so that neither the thumb assembler nor the ARM function
has to know about the other.

The lpc documents for this processor take a little time to explain
the pros and cons of using thumb mode.  For 16 bit wide memory
busses, thumb really shines, ARM instructions take two cycles to fetch,
and the performance difference is noticeable.  This chip has a 32 bit
wide bus and ARM and thumb take the same amount of time.  So the savings
here is the size of the binary.  Thumb code can take more instructions
to do the same thing as ARM so it might be slower in that respect, but
approaches half the binary size.  You can pack a lot more program into
the same flash using thumb mode.  The amount of thumb code compared
to ARM approaches 200% (cant quite get there but tries), where the
thumb to ARM performance hit is only 5 - 10% or so.  YMMV.

The linkerscript and Makefile have some rules.  There is no .data
segment defined in the linker script.  This is on purpose, first, never
use uninitialized variables (never assume a variable boots up as zero).

not supported (and a bad habit):

unsigned int x;
void myfun ( void )
{
   while(x==0)
   {
      ...
   }
}

supported:

unsigned int x;
void myfun ( void )
{
  x = 0;
  while(x==0)
  {
     ...
  }
}

And, these examples do not support non-const variables defined during
their definition:

Not supported:

unsigned int x=5;
void myfun ( void )
{
   while(x==5)
   {
      ...
   }
}

supported:

unsigned int x;
void myfun ( void )
{
  x = 5;
  while(x==5)
  {
     ...
  }
}

Because that variable takes up space both in the flash and the ram
and has to be copied on boot.  Not a bad habit, just makes life easier
on you when writing your startup code if you do this.  Although it
can be argued either way, there is a chance that your overall binary
is smaller.  Your startup code is definitely faster if you do not have
to prep .bss or .data.  If you already adopt the habit of initializing
variables before using them (in the code) then whether you init as
zero or init as non-zero doesnt matter, same habit, same style.

Constant, read-only data is supported because that is part of the
.text segment.

const char version[]="Myprog version 1.0";
void myfun ( void )
{
    show_string(version);
}

It is a good programming habit, embedded or not to declare all read only
variables/objects as const.  As well as function declarations where
an input (string) is read-only.

To keep the linker script simple, the command line for the linker
($(ARMGNU)-ld) is position sensitive.  The startup code needs to be
the first object on that command line.  The gnu linker, at least for
the last 10+ years, links items in the order presented on the command
line (with the exception of items called out in the linker script).
In this case startup.o needs to be first.  It also helps debugging when
you can see the files in the binary/disassembly in the order they
were defined in the command line.  There are as many different linker
scripts as there are authors of linker scripts.

Every other person you talk to about gnu linker scripts is going to have
a different philosophy. My philosophy on not just the linker scripts
but the whole approach described above) is the K.I.S.S (Keep It Simple
Stupid) approach.  As much as practical make your code do the work not
the compiler or tools.  Avoid compiler specific features.  Write portable
code.  Portable in the sense that you dont have to do a lot of work to
it or the build system.  Figuring out 17 different command line options
for this compiler and 12 for that to make the program build properly is
not portable, nor is a custom header file for each compiler.  Every
second or third person who operates at this level is going to have a
different approach, mine is but one of millions.  Form your own
philosophy and run with it.  The point here is to explain that there
are some programming rules you need to follow when modifying these
examples to avoid failure.  Bottom line, dissassemble often, make
sure the code being generated is what and where you anticipated.

Loading the program and running it is described in the top level
README.
