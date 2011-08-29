See the top level README file for information about the users guide
and schematic.s

The schematics show that P15 a.k.a IO15 drives an led.  This is the
led we are going to blink with this program.

This first blinker example is ARM assembler only.  You will need
binutils (assembler and linker) but not gcc (C compiler) to build this.

Looking at the users guide for this part there are legacy or slow
registers for GPIO, and some new fast registers.  The first example
will use the legacy registers.  If you follow the links in the manual
you will see that the part comes out of reset set to use the legacy
GPIO registers.

Also note that PINSEL0 shows the post reset state is 0b00 for the
port 15 settins, indicating the I/O pin is tied to GPIO 15 internally
(instead of an alternate function).  So for now we will assume this
post-reset setting is correct.

We need to set the direction bit to a 1 for port 15 and then use
set and clear to set and clear the output.  Slowing the led blink
rate down so we humans can see it, is done using a simple counter.

Loading the program and running it is described in the top level
README.


