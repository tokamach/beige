ENTRY(_start)

SECTIONS
{
	. = 1M;

	/* multiboot header */
	.text BLOCK(4K) : ALIGN(4K)
	{
		*(.multiboot)
		*(.text)
	}

	/* read only data */
	.rodata BLOCK(4K) : ALIGN(4K)
	{
		*(.rodata)
	}

	/* read-write data */
	.data BLOCK(4K) : ALIGN (4K)
	{
		*(.data)
	}

	.bss BLOCK(4K) : ALIGN (4K)
	{
		*(COMMON)
		*(.bss)
	}
}
