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

#ifndef CPUINFO_HEADER
#define CPUINFO_HEADER	1

#include "kernel.h"

typedef struct __cpuinfo
{
	char brandString[50];
	char vendorID[15];
	byte extendedFamily;
	byte extendedModel;
	byte processorType;
	byte familyCode;
	byte modelNumber;
	byte steppingID;
	byte apicPresence;
	byte msrSupported;
	byte procCount;
} cpuinfo_t;

int32_t k_query_cpu_info(cpuinfo_t *cpuinfo);

/* check presence of CPUID instruction */
int32_t k_check_cpuid();
/* CPUID exec */
void k_get_cpu_info(uint32_t mode, uint32_t *eax, uint32_t *ebx,
	uint32_t *ecx, uint32_t *edx);

void k_cpuinfo_print(cpuinfo_t *cpuinfo);

#endif
