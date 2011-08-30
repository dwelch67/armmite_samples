
See the top level README file for information about the users guide
and schematic.s

The microcontroller runs off of a 20Mhz oscillator, there is a PLL
system that can be used to multiply that clock to a higher frequency.
The 2103 is limited to 70Mhz but we cannot get better than 60 with
a 20Mhz oscillator.   Plldiv.c walks through the possible combinations
of M and P (multiply and divide) within the pll rules.

P 4 M  1 FCCO 160000000 CCLK  20000000
P 8 M  1 FCCO 320000000 CCLK  20000000
P 2 M  2 FCCO 160000000 CCLK  40000000
P 4 M  2 FCCO 320000000 CCLK  40000000
P 2 M  3 FCCO 240000000 CCLK  60000000

The app note AN10254 shows a PLL init sequence, this code follows that
with different M and P values reflecting the ARMmite pro using a 20Mhz
reference clock.

For fun the APB clock is set to divide by one so it is also at 60Mhz.

Also for fun the prescaler in the timer is used to knock the main counter
down to millions of counts count to 600 and that should be 10 seconds
at 60MHz.  And it appears to be working.
