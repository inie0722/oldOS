CC = gcc
LD = ld
D = -ffreestanding -fno-asynchronous-unwind-tables -fno-pic -nostdlib -O3 -Wall -I ./include

SRC = src

BOOT_OBJ = boot.o loader.o bios.o fat12.o loader_kernel.o
boot.bin: $(BOOT_OBJ)
	$(LD) -T ./src/boot/boot.lds $(BOOT_OBJ) -o boot.elf
	objcopy -O binary boot.elf $@
	dd if=$@ of=/home/inie0722/boot.img bs=512 count=8 conv=notrunc

boot.o: ./src/boot/boot.s
	$(CC) $(D) -c $< -o $@

loader.o: ./src/boot/loader.s
	$(CC) $(D) -c $< -o $@

loader_kernel.o: ./src/boot/loader_kernel.c
	$(CC) $(D) -c $< -o $@

bios.o: ./src/bios/bios.c
	$(CC) $(D) -c $< -o $@

fat12.o: ./src/fat12/fat12.c
	$(CC) $(D) -c $< -o $@

KERNEL_OBJ = kernel.o head.o bios.o
kernel.bin: $(KERNEL_OBJ)
	$(LD) -T ./src/kernel/kernel.lds $(KERNEL_OBJ) -o kernel.elf
	objcopy -O binary kernel.elf $@

kernel.o: ./src/kernel/kernel.c
	$(CC) $(D) -c $< -o $@

head.o: ./src/kernel/head.s
	$(CC) $(D) -c $< -o $@

.PHONY : clean
ALL_OBJ = $(BOOT_OBJ) $(KERNEL_OBJ)
clear:
	-rm $(ALL_OBJ)