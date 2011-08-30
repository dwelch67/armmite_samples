
extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );
extern void dummy ( unsigned int );

#define PINSEL0 0xE002C000
#define IO0SET  0xE0028004
#define IO0DIR  0xE0028008
#define IO0CLR  0xE002800C

#define LEDBIT  15

#define T0CR   0xE0004004
#define T0TC    0xE0004008
#define T0PR    0xE000400C

#define TIMEOUT (5000000*10)

#define APBDIV 0xE01FC100

void notmain ( void )
{
    unsigned int lastcount;
    unsigned int nowcount;

    //PUT32(APBDIV,1);

    //zero bits 30 and 31 (connect pin to GPIO)
    PUT32(PINSEL0,GET32(PINSEL0)&(~(3<<(LEDBIT<<1))));
    //make the pin an output
    PUT32(IO0DIR,GET32(IO0DIR)|(1<<LEDBIT));
    PUT32(T0CR,2);     //reset timer
    PUT32(T0CR,0);     //reset timer
    PUT32(T0PR,(1-1)); //divide by 1
    PUT32(T0CR,1);     //enable timer
    lastcount=GET32(T0TC);
    while(1)
    {
        PUT32(IO0SET,(1<<LEDBIT));
        while(1)
        {
            nowcount=GET32(T0TC);
            nowcount-=lastcount; //upcounter
            if(nowcount>=TIMEOUT) break;
        }
        lastcount+=TIMEOUT;
        PUT32(IO0CLR,(1<<LEDBIT));
        while(1)
        {
            nowcount=GET32(T0TC);
            nowcount-=lastcount; //upcounter
            if(nowcount>=TIMEOUT) break;
        }
        lastcount+=TIMEOUT;
    }
}
