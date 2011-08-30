
//-------------------------------------------------------------------
//-------------------------------------------------------------------

extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );

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

#define U0ACR       0xE000C020
#define U0LCR       0xE000C00C
#define U0DLL       0xE000C000
#define U0DLM       0xE000C004
#define U0IER       0xE000C004
#define U0FDR       0xE000C028
#define U0FCR       0xE000C008
#define U0TER       0xE000C030
#define U0LSR       0xE000C014
#define U0THR       0xE000C000
#define U0RBR       0xE000C000

//-------------------------------------------------------------------
void uart_init ( void )
{
    unsigned int ra;

    ra=GET32(PINSEL0);
    ra&=(~0xF);
    ra|=0x5;
    PUT32(PINSEL0,ra);
//60000000 Hz PCLK 38400 baud
//dl 0x47 mul 0x08 div 0x03 baud 38412 diff 12
    PUT32(U0ACR,0x00); //no autobaud
    PUT32(U0LCR,0x83); //dlab=1; N81
    PUT32(U0DLL,0x47); //dl = 0x0047
    PUT32(U0DLM,0x00); //dl = 0x0047
    PUT32(U0IER,0x00); //no interrupts
    PUT32(U0LCR,0x03); //dlab=0; N81
    PUT32(U0IER,0x00); //no interrupts
    PUT32(U0FDR,(0x8<<4)|(0x3<<0)); //mul 0x08, div 0x03
    PUT32(U0FCR,(1<<2)|(1<<1)|(1<<0)); //enable and reset fifos
    PUT32(U0TER,(1<<7)); //transmit enable
}
//-------------------------------------------------------------------
void uart_putc ( unsigned int x )
{
    while (( GET32(U0LSR) & (1<<5)) == 0) continue;
    PUT32(U0THR,x);
}
//-------------------------------------------------------------------
unsigned int uart_getc ( void )
{
    while (( GET32(U0LSR) & (1<<0)) == 0) continue;
    return(GET32(U0RBR));
}
//-------------------------------------------------------------------
void hexstring ( unsigned int d, unsigned int cr )
{
    //unsigned int ra;
    unsigned int rb;
    unsigned int rc;

    rb=32;
    while(1)
    {
        rb-=4;
        rc=(d>>rb)&0xF;
        if(rc>9) rc+=0x37; else rc+=0x30;
        uart_putc(rc);
        if(rb==0) break;
    }
    if(cr)
    {
        uart_putc(0x0D);
        uart_putc(0x0A);
    }
    else
    {
        uart_putc(0x20);
    }
}
//-------------------------------------------------------------------
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
//-------------------------------------------------------------------
void notmain ( void )
{
    unsigned int lastcount;
    unsigned int nowcount;
    unsigned int timeout;

    pll_init(); //run clock at 60MHz.

    PUT32(APBDIV,1); //run peripherals at 60MHz

    uart_init();
    hexstring(0x12345678,1);

    //zero bits 30 and 31 (connect pin to GPIO)
    PUT32(PINSEL0,GET32(PINSEL0)&(~(3<<(LEDBIT<<1))));
    //make the pin an output
    PUT32(IO0DIR,GET32(IO0DIR)|(1<<LEDBIT));
    PUT32(T0CR,2);     //reset timer
    PUT32(T0CR,0);     //reset timer
    PUT32(T0PR,(1000000-1)); //divide by 1 million
    PUT32(T0CR,1);     //enable timer
    timeout=60*3; //10 seconds
    lastcount=GET32(T0TC);
    while(1)
    {
        PUT32(IO0SET,(1<<LEDBIT));
        //not too fast if you are just jamming these in
        PUT32(U0THR,0x31);
        while(1)
        {
            nowcount=GET32(T0TC);
            nowcount-=lastcount; //upcounter
            if(nowcount>=timeout) break;
        }
        lastcount+=timeout;
        PUT32(IO0CLR,(1<<LEDBIT));
        //not too fast if you are just jamming these in
        PUT32(U0THR,0x30);
        while(1)
        {
            nowcount=GET32(T0TC);
            nowcount-=lastcount; //upcounter
            if(nowcount>=timeout) break;
        }
        lastcount+=timeout;
    }
}
//-------------------------------------------------------------------
//-------------------------------------------------------------------
