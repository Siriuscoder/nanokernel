OUTPUT_FORMAT("binary")
ENTRY(start)
SECTIONS
{
	.text16 0xE000 : 
	{
    	start.o (.text)
    	protmode.o (.text)
	}

	.data16 :
	{
		start.o (.data)
    	protmode.o (.data)
	}

	.rodata16 :
	{
		start.o (.rodata)
    	protmode.o (.rodata)
	}

	.text :
	{
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
