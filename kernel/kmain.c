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

#include "screen.h"
#include "cpuinfo.h"
#include "int.h"
#include "kerror.h"
#include "std/print.h"



void k_main()
{
	if(!k_init_screen())
		k_panic1(SCR_INIT_FAILED);

	k_print("Boot process done.. Starting the kernel\n");
	k_print("Console init OK..\n");
	k_print("Kernel version: %s\n", k_version_full_string);

	k_refresh_cpu_info();
	k_cpuinfo_print(k_get_cpuinfo());

	if(!k_interrupts_init())
		k_panic1(INT_INIT_FAILED);


	k_print("Halt kernel now..");
	k_abort();
}

