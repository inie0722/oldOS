OUTPUT_FORMAT("elf64-x86-64", "elf64-x86-64", "elf64-x86-64")

OUTPUT_ARCH(i386:x86-64)


ENTRY(_start)

SECTIONS
{

	. = 0x7C00;
	.begin : {boot.o(.begin)}

	. = 0x7C00 + 510;
	.end : {boot.o (.end)}

	.= 0x7c00 + 512;

	.loader : {loader.o}
	.fat12 : {fat12.o}
	.bios : {bios.o}
}