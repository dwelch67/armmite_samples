
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


unsigned int max;

unsigned int msel;
unsigned int psel;
unsigned int fosc;
unsigned int fcco; //between 156Mhz and 320Mhz
unsigned int cclk;
int main ( void )
{
    max =70000000;
    fosc=20000000;

    for(msel=1;msel<=32;msel++)
    {
        for(psel=1;psel<=8;psel<<=1)
        {
            fcco=(fosc*msel*psel*2);
            if(fcco<156000000) continue;
            if(fcco>320000000) continue;
            cclk=(fosc*msel);
            if(cclk>max) continue;
            printf("P %u M %2u FCCO %9u CCLK %9u\n",psel,msel,fcco,cclk);
        }
    }
    return(0);
}
