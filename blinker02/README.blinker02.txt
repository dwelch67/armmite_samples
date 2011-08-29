See the top level README file for information about the users guide
and schematic.s

This second blinker example is ARM assembler only.  You will need
binutils (assembler and linker) but not gcc (C compiler) to build this.

This example is the same as the blinker01 example except it uses thumb
mode.  ARM mode or ARM instructions are the full 32 bit instruction word
instruction set.  Thumb instructions are 16 bit, they map one to one
to ARM instructions, obviously only a subset of the ARM instructions
are supported.  The thumb instructions are not just half the ARM
instruction they map to, they have their own encoding.  You have to
deliberatly switch from ARM mode to thumb mode and back.  The
ARM7TDMI in this part uses the architecture ARMv4T.  The oldest/earliest
thumb able instruction set.  For ARMv4T the BX instruction is the
only way to switch modes.  If you are going from ARM to ARM or thumb
to thumb, the BX instruction is perfectly safe.  In fact instead of
using mov pc,lr to return from an instruction as you may have done for
ARMv4 or older programs, bx lr is preferred, this way you dont ever
have to care which mode called your function.

The ARMv4T and other traditional ARM instruction set processors use
ARM mode only when servicing an exception.  So every entry in the
exception table is an ARM mode instruction.  Because bx takes two
instructions you need to call ARM code from the exception table.
The example shows the reset exception calling ARM code and that code
changing to thumb mode to handle the exception.

Loading the program and running it is described in the top level
README.


