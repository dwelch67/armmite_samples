
#ARMGNU=arm-thumb-elf
#ARMGNU=arm-none-linux-gnueabi
ARMGNU?=arm-none-eabi

AOPS = --warn --fatal-warnings
COPS = -mthumb -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding 

all : blinker03.bin

blinker03.bin : memmap startup.o blinker03.o
	$(ARMGNU)-ld -T memmap startup.o blinker03.o -o blinker03.elf
	$(ARMGNU)-objdump -D blinker03.elf > blinker03.list
	$(ARMGNU)-objcopy blinker03.elf blinker03.bin -O binary

startup.o : startup.s
	$(ARMGNU)-as $(AOPS) startup.s -o startup.o

blinker03.o : blinker03.c
	$(ARMGNU)-gcc $(COPS) -c blinker03.c -o blinker03.o

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list

