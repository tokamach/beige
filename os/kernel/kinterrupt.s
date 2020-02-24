	/* interrupts handlers with no err code */
	.macro	no_err_code_interrupt_handler num
	.globl interrupt_handler_\num
interrupt_handler_\num:
	pushw 0
	pushw \num
	jmp common_interrupt_handler
	.endm

	/* interrupt handlers with an err code */
	.macro err_code_interrupt_handler num
	.globl interrupt_handler_\num
interrupt_handler_\num:
	pushw \num
	jmp common_interrupt_handler
	.endm
	
	/* the shared interrupt handler that deals with registers */
	.extern forward_interrupt
common_interrupt_handler:
	pushal
 	
	mov %ds, %ax
	pushl %eax

	mov 0x10, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	
	call forward_interrupt
	
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	
	popal
	add 8, %esp
	sti
	iret

	
	/* Calling the macro, i.e. setting up interrupt handlers */
	/* Protected mode Exceptions */
	no_err_code_interrupt_handler  0  # Div by 0
	no_err_code_interrupt_handler  1  # Debug exception
	no_err_code_interrupt_handler  2  # Non-maskable interrupt
	no_err_code_interrupt_handler  3  # Breakpoint exception
	no_err_code_interrupt_handler  4  # "Into detected overflow"
	no_err_code_interrupt_handler  5  # Out of bounds exception
	no_err_code_interrupt_handler  6  # Invalid opcode
	no_err_code_interrupt_handler  7  # No Coprocessor
	err_code_interrupt_handler     8  # Double fault
	no_err_code_interrupt_handler  9  # Coprocessor seg overrun
	err_code_interrupt_handler    10  # Bad TSS
	err_code_interrupt_handler    11  # Segment not present
	err_code_interrupt_handler    12  # Stack fault
	err_code_interrupt_handler    13  # GPF
	err_code_interrupt_handler    14  # Page Fault
	no_err_code_interrupt_handler 15  # Unkown interrupt fault
	no_err_code_interrupt_handler 16  # Coprocessor fault
	err_code_interrupt_handler    17  # Alignment check exception
	no_err_code_interrupt_handler 18  # Machine check exception
	no_err_code_interrupt_handler 19  # SIMD floating point exception

	/* IRQs */
	no_err_code_interrupt_handler 32  # keyboard?
	no_err_code_interrupt_handler 33  # keyboard?
	no_err_code_interrupt_handler 34
	no_err_code_interrupt_handler 35
	no_err_code_interrupt_handler 36
	no_err_code_interrupt_handler 37
	no_err_code_interrupt_handler 38
	no_err_code_interrupt_handler 39
	no_err_code_interrupt_handler 40
	no_err_code_interrupt_handler 41
	no_err_code_interrupt_handler 42
	no_err_code_interrupt_handler 43
	no_err_code_interrupt_handler 44
	no_err_code_interrupt_handler 45
	no_err_code_interrupt_handler 46
	no_err_code_interrupt_handler 47
	

	/* Loading IDT */
	.globl load_idt
	.globl enable_interrupts
	.globl disable_interrupts

	/* stack: 4(%esp) IDT target addr
	            %esp return addr
	*/
load_idt:
	mov 4(%esp), %eax
	lidt (%eax)

enable_interrupts:
	sti
	ret
	
disable_interrupts:
	cli
	ret
