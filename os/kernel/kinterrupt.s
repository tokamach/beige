/*
	.macro	no_error_code_interrupt_handler num
	.globl interrupt_handler_\num
interrupt_handler_\num:
	pushw 0
	pushw \num
	jmp common_interrupt_handler
	.endm

	.macro error_code_interrupt_handler num
	global interrupt_handler_\num
interrupt_handler_\num:
	pushw \num
	jmp common_interrupt_handler
	.endm
	

common_interrupt_handler:
	pushal
	
	#TODO: hlt and validate reg in qemu
	call interrupt_handler

	popal
	add 8, %esp
	sti
	iret
*/
	.globl load_idt
	.globl enable_interrupts
	.globl disable_interrupts

load_idt:
	mov 4(%esp), %eax
	lidt (%eax)

enable_interrupts:
	sti
	ret
	
disable_interrupts:
	cli
	ret
