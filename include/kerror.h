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

#ifndef ERROR_H_
#define ERROR_H_

#include <kernel.h>

#define INIT_FAILED				0xE000
#define PIC_INIT_FAILED			0xE001
#define INT_INIT_FAILED			0xE002
#define CON_INIT_FAILED			0xE003
#define DRV_INIT_FAILED			0xE004
#define SCR_INIT_FAILED			0xE005
#define CPU_EXCEPTION			0xE006
#define OUT_OF_MEMORY			0xE007
#define HEAP_INIT_FAILED		0xE007

#pragma pack(push, 1)
typedef struct
{
	int16_t		ss;
	int16_t		cs;
	int16_t		gs;
	int16_t		fs;
	int16_t		es;
	int16_t		ds;
	int32_t 	edi;
	int32_t 	esi;
	int32_t 	ebp;
	int32_t		esp;
	int32_t		ebx;
	int32_t 	edx;
	int32_t 	ecx;
	int32_t 	eax;
} regs_t;
#pragma pack(pop)

void k_panic1(int errCode);
void k_panic3(int errCode, int addCode, const char *str);
void k_panic4(int errCode, int addCode, const char *str,
		const regs_t *regs, uint32_t address);

#endif /* ERROR_H_ */
