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

#include "cpuinfo.h"
#include "std/membase.h"


int32_t k_query_cpu_info(cpuinfo_t *cpuinfo)
{
	if(!cpuinfo)
		return 0;

	k_memset(cpuinfo, 0, sizeof(cpuinfo_t));

	if(!k_check_cpuid())
	{
		cpuinfo->apicPresence = 0;
		return 0;
	}

	

	return 1;
}

