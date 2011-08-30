
//-------------------------------------------------------------------
//-------------------------------------------------------------------

extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );

#define PINSEL0     0xE002C000
#define APBDIV      0xE01FC100
#define MAMCR       0xE01FC000
#define MAMTIM      0xE01FC004
#define PINSEL0     0xE002C000
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
//20000000 Hz PCLK 38400 baud
//dl 0x13 mul 0x07 div 0x05 baud 38377 diff 23
    PUT32(U0ACR,0x00); //no autobaud
    PUT32(U0LCR,0x83); //dlab=1; N81
    PUT32(U0DLL,0x13); //dl = 0x0013
    PUT32(U0DLM,0x00); //dl = 0x0013
    PUT32(U0IER,0x00); //no interrupts
    PUT32(U0LCR,0x03); //dlab=0; N81
    PUT32(U0IER,0x00); //no interrupts
    PUT32(U0FDR,(0x7<<4)|(0x5<<0)); //mul 0x07, div 0x05
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
void notmain ( void )
{
    PUT32(MAMCR,0);
    PUT32(MAMTIM,2); //20MHz
    PUT32(MAMCR,2);
    PUT32(APBDIV,1);

    uart_init();
    hexstring(0x12345678,1);
}
//-------------------------------------------------------------------
//-------------------------------------------------------------------
