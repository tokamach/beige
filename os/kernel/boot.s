	/* multiboot consts */
	.set ALIGN,   1<<0
	.set MEMINFO, 1<<1
	.set FLAGS, ALIGN | MEMINFO
	.set MAGIC, 0x1BADB002
	.set CHECKSUM, -(MAGIC + FLAGS)

	/* multiboot header */
	.section .multiboot
	.align 4
	.long MAGIC	
	.long FLAGS
	.long CHECKSUM

	/* stack setup */
	.section .bss
	.align 16
stack_bottom:
	.skip 16384  /* 16 KiB */
stack_top:

	/* _start is specified as the entry point by the linker
	   the bootlader will load at point */
	.section .text
	.global _start
	.type _start, @function
_start:
	/* setup stack */
	mov $stack_top, %esp

	/* TODO: initalize GDT, floating point, etc */

	/* enter kernel */
	call kernel_main

	/* loop */
	cli
1:	hlt
	jmp 1b
	

	/* set _start size to here (.) minus _start
	   useful for debugging */
	.size _start, . - _start
