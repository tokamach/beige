	.globl load_gdt

	.set K_CODE_SEG, 0x08
	.set K_DATA_SEG, 0x10

	/* stack: 4(%esp) gdt_ptr
	 	  %esp    return address */
load_gdt:
	mov 4(%esp), %eax
	lgdt (%eax)

	# 0x08 is kernel code segment
	ljmp $K_CODE_SEG, $reload_segreg

reload_segreg:
	mov $K_DATA_SEG, %ax		# Kernel Data segment
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %ss
	mov %ax, %fs
	mov %ax, %gs
	ret
	
