CC = gcc
LD = ld
D = -ffreestanding -fno-asynchronous-unwind-tables -fno-pic -nostdlib -Wall

DLS = kernel.lds

obj = kernel.o

build:

	$(CC) $(D) -c kernel.c -o kernel.o
	
	$(LD) -T $(DLS) $(obj) -o kernel.bin
	
	objcopy -O binary kernel.bin kernel.bin

	-rm $(obj)

clear:
	-rm $(obj)