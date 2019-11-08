#include <asm/regdef.h>

.text
.extern main
.extern trace_clear
.globl _start
.set noreorder

_start:
	la sp, __stack
	la t0, _bss_start
	la t1, _bss_end

_clear_bss_loop:
	sw zero, 0(t0)
	bne t0, t1, _clear_bss_loop
	addi t0, t0, 4

	jal trace_clear
	nop

	jal main
	nop

_end:
	nop
	nop
	nop
	nop
	wait

	b _end
	nop
