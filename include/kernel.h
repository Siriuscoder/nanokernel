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

#ifndef KERNEL_HEADER
#define KERNEL_HEADER	1

#include "loader.h"

#ifndef __GAS__
#include "types.h"
#endif

/* realmode segments */
#define KERNEL_REALMODE_STACK_SEG		0x0
#define KERNEL_REALMODE_CODE_SEG		0x0
#define KERNEL_REALMODE_DATA_SEG		0x0
/* realmode address */
#define KERNEL_REALMODE_STACK_ADDR		(KERNEL_LOAD_SEGMENT*0x10)
/* realmode stack size */
#define KERNEL_REALMODE_STACK_SIZE		(KERNEL_REALMODE_STACK_ADDR-0x70)
#define KERNEL_REALMODE_CODE_ADDR		0x0
#define KERNEL_REALMODE_DATA_ADDR		KERNEL_REALMODE_CODE_ADDR

/* protected mode segments */
#define KERNEL_PROTECTEDMODE_STACK_SEG		0x18
#define KERNEL_PROTECTEDMODE_CODE_SEG		0x8
#define KERNEL_PROTECTEDMODE_DATA_SEG		0x10
#define KERNEL_PROTECTEDMODE_STACK_ADDR		KERNEL_REALMODE_STACK_ADDR


#if !defined(__GAS__)

/*
Type 1: Usable (normal) RAM
Type 2: Reserved - unusable
Type 3: ACPI reclaimable memory
Type 4: ACPI NVS memory
Type 5: Area containing bad memory
*/

#define MEMORY_USE_NORMAL			1
#define MEMORY_USE_RESERVED			2
#define MEMORY_USE_ACPI_RECLAIMABLE		3
#define MEMORY_USE_ACPI_NVS			4
#define MEMORY_USE_BAD_MEMORY_AREA		5

#pragma pack(push,4)
typedef struct phisical_address_map_entity
{
	uint64_t base; // base address QWORD
	uint64_t length; // length QWORD
	uint16_t type; // entry Type
} phisical_address_map_entity_t;
#pragma pack(pop)

#endif

#endif

