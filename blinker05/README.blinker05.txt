
See the top level README file for information about the users guide
and schematic.s

The schematic shows a 20MHz oscillator feeding the part.  The APBDIV
register resets to 0x00 which is ABP bus clock is one fourth the
processor clock.  The ABP clock is what the peripherals like the timer
use.  This example program uses timer0 to time the blinks of the
processor clock so that they are not (as) dependent on execution time of
the program.  The assumption is that with APBDIV at the reset value of
0x00, the timer clock is 5MHz.  One fourth of the processor clock.
So if we count to five million times ten timer ticks that should
take ten seconds.  One second for every five million timer ticks at
5MHz.

And indeed this is what happens.  So this means the processor is probably
running at 20Mhz, running off of the external clock.

Microcontrollers do not necessarily do this.  Some have internal
oscillators that are used to boot up, nice and slow and low power and
then if you want to burn more power you can hook in an external oscillator
and/or use a circuit in the chip to mulitply the clock even faster.
That will come in future samples.  For now to test the 20Mhz/4 = 5Mhz
theory, uncomment the line that modifies the APBDIV register to a 1
causing the APB clock to match the processor clock.  Recompile and run
and see the led blink every 2.5 seconds, four times faster.  50 million
counts at 20Mhz is 2.5 seconds.

