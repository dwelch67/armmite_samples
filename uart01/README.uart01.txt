
See the top level README file for information about the users guide
and schematic.s

This is derived from blinker05, using the crystal 20MHz clock.  APBDEV
is set to 1 to run the peripherals at 20MHz.

Follow the guide in the manual for determining the divisor and fractions
to feed the uart to generate 38400 baud.  The uartdiv program does this
work for you and produces combinations of setings and what the resulting
baud rate will be.

Proglpc when finished waits on and prints out what it finds on the
serial port, so after programming press the reset button on the ARMmite
Pro and see the serial output.

