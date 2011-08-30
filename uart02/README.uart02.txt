
See the top level README file for information about the users guide
and schematic.s

This is derived from blinker06, adding uart to it.

blinker06 uses the PLL to run the microcontroller at 60Mhz.  The APB
clock is set to divide by 1 so the peripherals also run at 60Mhz.
The uart is divided down from there to 38400 baud.  The timer uses
the prescaler to divide by one million, leaving 60 counts per second
on the main counter.  the code also shoves a byte into the uart
transmitter every time the led changes, at 10 seconds between that
is no problem.  If this code is modified to blink fast enough that
can and will overflow the uart transmit buffer.


