	.macro	no_error_code_interrupt_handler num
	global interrupt_handler_\num
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
	pushad
	
	;; TODO: hlt and validate reg in qemu
	call interrupt_handler

	popad
	add 8, %esp
	sti
	iret
