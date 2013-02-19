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
#include "console.h"
#include "cpuinfo.h"

/* console init structure */
const consoleEntry_t consoleEntry = {
	(ptr_t)0xB8000,
	80,
	25
};

/* cpu info structure */
static cpuinfo_t cpuInfo;

int k_main()
{
	if(k_console_init(&consoleEntry) != 0)
		return EXIT_PANIC;

	k_console_write("Boot process done.. Starting the kernel\n");
	k_console_write("Console init OK..\n");
	k_console_write("Kernel version: ");
	k_console_write(k_version_full_string);
	k_console_putc('\n');

	k_console_write("Query cpu info.. ");
	if(k_query_cpu_info(&cpuInfo))
	{
		k_console_write("Supported\n");
		k_cpuinfo_print(&cpuInfo);
	}
	else
		k_console_write("Not supported\n");

	k_console_write("Halt kernel now..");
	return EXIT_CPU_HALT;
}

