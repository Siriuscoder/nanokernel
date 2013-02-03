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

/* console init structure */
const consoleEntry_t consoleEntry = {
	(ptr_t)0xB8000,
	80,
	25
};

int kmain()
{
	if(console_init(&consoleEntry) != 0)
		return EXIT_PANIC;

	console_write("Boot process done.. Starting the kernel\n");
	console_write("Console init OK..\n");
	console_write("Kernel version: ");
	console_write(k_version_full_string);
	console_putc('\n');

	console_write("Halt kernel now..");
	return EXIT_CPU_HALT;
}

