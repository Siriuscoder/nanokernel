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
	bool apicPresence;
	bool msrSupported;
	byte procCount;
	bool cpuInfoSupported;
} cpuinfo_t;

bool k_refresh_cpu_info();

void k_cpuinfo_print(cpuinfo_t *cpuinfo);

cpuinfo_t *k_get_cpuinfo();

#endif
