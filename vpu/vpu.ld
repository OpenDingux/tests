OUTPUT_ARCH(mips)
ENTRY(_start)

MEMORY
{
	tcsm0 : ORIGIN = 0x132b0000 , LENGTH = 0x4000
	tcsm1 : ORIGIN = 0xf4000000 , LENGTH = 0xc000
}

SECTIONS
{
	.text.1 : { head.o(.text*) } > tcsm1
	.text.0 : { *(EXCLUDE_FILE (head.o) .text*) } > tcsm1

	. = ALIGN(0x1000);

	.data : {
		*(.rodata*) *(.data*) *(.scommon*) *(.reginfo*)
	} > tcsm1

	. = ALIGN(0x1000);

	.bss : {
		. = ALIGN(4);
		_bss_start = ABSOLUTE(.);
		*(.sbss*) *(.bss*)
		. = ALIGN(4);
		_bss_end = ABSOLUTE(.);
	} > tcsm1

	__stack = 0xf400c000;
}
