
extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );
extern void dummy ( unsigned int );


#define PLLCON  0xE01FC080
#define PLLCFG  0xE01FC084
#define PLLSTAT 0xE01FC088
#define PLLFEED 0xE01FC08C

#define MAMCR  0xE01FC000
#define MAMTIM 0xE01FC004

#define PINSEL0 0xE002C000
#define IO0SET  0xE0028004
#define IO0DIR  0xE0028008
#define IO0CLR  0xE002800C

#define LEDBIT  15

#define T0CR    0xE0004004
#define T0TC    0xE0004008
#define T0PR    0xE000400C

#define APBDIV 0xE01FC100

void pll_init(void)
{
    //P 2 M  3 FCCO 240000000 CCLK  60000000
    PUT32(PLLCFG,((2-1)<<5)|((3-1)<<0));
    PUT32(PLLCON,0x01);
    PUT32(PLLFEED,0xAA);
    PUT32(PLLFEED,0x55);
    while((GET32(PLLSTAT)&(1<<10))==0) continue;
    PUT32(PLLCON,0x03);
    PUT32(PLLFEED,0xAA);
    PUT32(PLLFEED,0x55);
    //assuming 40 to 60 MHz
    PUT32(MAMCR,0);
    PUT32(MAMTIM,3);
    //PUT32(MAMCR,2); //errata says mode 2 may fail
    PUT32(MAMCR,1);   //errata says mode 2 may fail
}

void notmain ( void )
{
    unsigned int lastcount;
    unsigned int nowcount;
    unsigned int timeout;

    pll_init(); //run clock at 60MHz.

    PUT32(APBDIV,1); //run timer at 60MHz

    //zero bits 30 and 31 (connect pin to GPIO)
    PUT32(PINSEL0,GET32(PINSEL0)&(~(3<<(LEDBIT<<1))));
    //make the pin an output
    PUT32(IO0DIR,GET32(IO0DIR)|(1<<LEDBIT));
    PUT32(T0CR,2);     //reset timer
    PUT32(T0CR,0);     //reset timer
    PUT32(T0PR,(1000000-1)); //divide by 1 million
    PUT32(T0CR,1);     //enable timer
    timeout=60*10; //10 seconds
    lastcount=GET32(T0TC);
    while(1)
    {
        PUT32(IO0SET,(1<<LEDBIT));
        while(1)
        {
            nowcount=GET32(T0TC);
            nowcount-=lastcount; //upcounter
            if(nowcount>=timeout) break;
        }
        lastcount+=timeout;
        PUT32(IO0CLR,(1<<LEDBIT));
        while(1)
        {
            nowcount=GET32(T0TC);
            nowcount-=lastcount; //upcounter
            if(nowcount>=timeout) break;
        }
        lastcount+=timeout;
    }
}
