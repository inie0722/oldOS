CC = clang
LD = ld
D = -ffreestanding -fno-asynchronous-unwind-tables -fno-pic -nostdlib -O3 -Wall

DLS = boot.lds

obj = boot.o loader.o bios.o fat12.o

build:

	$(CC) $(D) -c boot.s -o boot.o
	$(CC) $(D) -c loader.s -o loader.o
	$(CC) $(D) -c bios.c -o bios.o
	$(CC) $(D) -c fat12.c -o fat12.o
	
	$(LD) -T $(DLS) $(obj) -o boot.elf
	
	objcopy -O binary boot.elf boot.bin
	
	dd if=boot.bin of=/home/inie0722/boot.img bs=512 count=8 conv=notrunc

	-rm $(obj) boot.bin

clear:
	-rm $(obj)