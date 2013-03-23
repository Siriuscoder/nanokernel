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

#include <std/print.h>
#include <kerror.h>

static void k_print_regs(const regs_t *regs)
{
	if(!regs)
		return ;

	k_print("Registers: \n");
	k_print("EAX=0x%08x; EBX=0x%08x; ECX=0x%08x; EDX=0x%08x\n",
			regs->eax, regs->ebx, regs->ecx, regs->edx);
	k_print("ESP=0x%08x; DS=0x%08x; ES=0x%08x; FS=0x%08x; GS=0x%08x",
			regs->esp, regs->ds, regs->es, regs->fs, regs->gs);
}

void k_panic1(int errCode)
{
	k_panic3(errCode, 0, NULL);
}

void k_panic3(int errCode, int addCode, const char *str)
{
	k_panic4(errCode, addCode, str, NULL, 0);
}

void k_panic4(int errCode, int addCode, const char *str, const regs_t *regs, uint32_t address)
{
	k_print("*** Kernel Panic at 0x%08x\n", address);
	k_print("Error Code 0x%08x; Second Code %d : %s\n", errCode, addCode,
			(str == NULL) ? "none" : str);

	k_print_regs(regs);
	k_abort();
}



