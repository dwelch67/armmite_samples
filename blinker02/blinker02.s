
/*blinker02.s*/
.code 32

.globl _start
_start:
    b reset
    b hang
    b hang
    b hang
    b hang
    b hang
    b hang
    b hang
    b hang
    b hang
    b hang
    b hang
    b hang
    b hang
    b hang
    b hang

hang:   b hang

reset:
    ldr r0,=thumb_start
    bx r0  /* branch and switch from ARM to thumb mode */

/* ------- ARM ABOVE, THUMB BELOW ------- */

.thumb

.thumb_func
thumb_start:
    ldr r0,=0x40002000
    mov sp,r0
    ldr r0,=0xE0028000

    ldr r1,=0x00008000 /* bit 15 */

    /* make P0.15 an output, 0xE0028008 */
    ldr r2,[r0,#0x08]
    orr r2,r1
    str r2,[r0,#0x08]
    /* blink led */

loop:
    str r1,[r0,#0x04] /* IO0SET 0xE0028004 */
    ldr r2,=0x40000
loop0:
    sub r2,r2,#1 /* gas should really allow the functionally correct subs here */
    bne loop0

    str r1,[r0,#0x0C] /* IO0CLR 0xE002800C */
    ldr r2,=0x40000
loop1:
    sub r2,#1
    bne loop1

    b loop
