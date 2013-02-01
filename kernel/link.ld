OUTPUT_FORMAT("binary")
ENTRY(start)
SECTIONS
{
	.text 0x7e00 : 
	{
    		start.o (.text)
		*(.text)
	}

	.data :
	{
		*(.data)
	}

	.bss :
	{
		*(.bss)
	}

	.rodata :
	{
		*(.rodata)
	}
}
