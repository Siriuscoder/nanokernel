/*  This file is part of simple kernel.
    Project NanoKernel (for study purposes only)
    Copyright (C) 2013  Sirius (Vdov Nikita)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "kernel.h"
#include "screen.h"
#include "cpuinfo.h"
#include "pic.h"
#include "int.h"

static bool init_interrupts()
{
	/* disable external interrupts */
	k_iasync_disable();

	k_console_write("Query cpu info.. ");
	if(k_refresh_cpu_info())
	{
		k_console_write("Supported\n");
		k_cpuinfo_print(k_get_cpuinfo());

		/* disable local APIC if presence */
		if(k_get_cpuinfo()->apicPresence)
		{
			if(k_get_cpuinfo()->msrSupported)
				k_apic_disable();
		}
	}
	else
		k_console_write("Not supported\n");

	/* set idt routines */
	if(!k_idt_init())
		return false;
	k_console_write("OK\n");
	/* init interrupt controler (i8259) */
	if(!k_pic_init())
		return false;
	k_console_write("OK\n");
	/* enable external interrupts */
	k_iasync_enable();
	for(;;);

	return true;
}

int k_main()
{
	if(!k_init_screen())
		return EXIT_PANIC;

	k_console_write("Boot process done.. Starting the kernel\n");
	k_console_write("Console init OK..\n");
	k_console_write("Kernel version: ");
	k_console_write(k_version_full_string);
	k_console_putc('\n');


	if(!init_interrupts())
		return EXIT_PANIC;

	k_console_write("Halt kernel now..");
	return EXIT_CPU_HALT;
}

