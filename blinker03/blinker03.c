
extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );
extern void dummy ( unsigned int );

#define PINSEL0 0xE002C000
#define IO0SET  0xE0028004
#define IO0DIR  0xE0028008
#define IO0CLR  0xE002800C

#define LEDBIT  15

void notmain ( void )
{
    unsigned int ra;

    //zero bits 30 and 31 (connect pin to GPIO)
    PUT32(PINSEL0,GET32(PINSEL0)&(~(3<<(LEDBIT<<1))));
    //make the pin an output
    PUT32(IO0DIR,GET32(IO0DIR)|(1<<LEDBIT));
    while(1)
    {
        PUT32(IO0SET,(1<<LEDBIT));
        for(ra=0x40000;ra;ra--) dummy(ra);
        PUT32(IO0CLR,(1<<LEDBIT));
        for(ra=0x40000;ra;ra--) dummy(ra);
    }
}
