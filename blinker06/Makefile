
#ARMGNU=arm-thumb-elf
#ARMGNU=arm-none-linux-gnueabi
ARMGNU?=arm-none-eabi

AOPS = --warn --fatal-warnings
COPS = -mthumb -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding 



LOPS = -Wall -m32 -emit-llvm
LLCOPS = -march=thumb
OOPS = -std-compile-opts



all : blinker06.gcc.bin blinker06.clang.norm.bin blinker06.clang.opt.bin


startup.o : startup.s
	$(ARMGNU)-as $(AOPS) startup.s -o startup.o



blinker06.gcc.bin : memmap startup.o blinker06.gcc.o
	$(ARMGNU)-ld -T memmap startup.o blinker06.gcc.o -o blinker06.gcc.elf
	$(ARMGNU)-objdump -D blinker06.gcc.elf > blinker06.gcc.list
	$(ARMGNU)-objcopy blinker06.gcc.elf blinker06.gcc.bin -O binary

blinker06.gcc.o : blinker06.c
	$(ARMGNU)-gcc $(COPS) -c blinker06.c -o blinker06.gcc.o



blinker06.clang.bc : blinker06.c
	clang $(LOPS) -c blinker06.c -o blinker06.clang.bc

blinker06.clang.norm.bin : memmap startup.o blinker06.clang.bc
	llc $(LLCOPS) blinker06.clang.bc -o blinker06.clang.norm.s
	$(ARMGNU)-as blinker06.clang.norm.s -o blinker06.clang.norm.o
	$(ARMGNU)-ld -o blinker06.clang.norm.elf -T memmap startup.o blinker06.clang.norm.o
	$(ARMGNU)-objdump -D blinker06.clang.norm.elf > blinker06.clang.norm.list
	$(ARMGNU)-objcopy blinker06.clang.norm.elf blinker06.clang.norm.bin -O binary

blinker06.clang.opt.bin : memmap startup.o blinker06.clang.bc
	opt $(OOPS) blinker06.clang.bc -o blinker06.clang.opt.bc
	llc $(LLCOPS) blinker06.clang.opt.bc -o blinker06.clang.opt.s
	$(ARMGNU)-as blinker06.clang.opt.s -o blinker06.clang.opt.o
	$(ARMGNU)-ld -o blinker06.clang.opt.elf -T memmap startup.o blinker06.clang.opt.o
	$(ARMGNU)-objdump -D blinker06.clang.opt.elf > blinker06.clang.opt.list
	$(ARMGNU)-objcopy blinker06.clang.opt.elf blinker06.clang.opt.bin -O binary

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.bc
	rm -f *.norm.s
	rm -f *.opt.s



