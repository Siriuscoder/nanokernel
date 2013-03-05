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

/* version definitions */
#define SET_VERSION(major, minor, build)	((major & 0xff) | ((minor & 0xff) << 8) | ((build & 0xff) << 16))
#define GET_MAJOR_VERSION(vers)			(vers & 0xff)
#define GET_MINOR_VERSION(vers)			((vers >> 8) & 0xff)
#define GET_BUILD_VERSION(vers)			((vers >> 16) & 0xff)
/* version */
#define MAJOR_VERSION				1
#define MINOR_VERSION				0
#define BUILD_VERSION				0
#define BUILD_ARCHITECTURE			I386
/* version string */
#define KERNEL_VERSION_FULL_STRING(x)		TO_STRING(x)
#define KERNEL_VERSION_FULL			nanokernel v.MAJOR_VERSION.MINOR_VERSION.BUILD_VERSION \
	BUILD_ARCHITECTURE compiled at __DATE__ __TIME__

/* phisical memory map address */
#define SYSTEM_ADDRESS_MAP_ENTRY		0x1000

/* realmode segments */
#define KERNEL_REALMODE_STACK_SEG		0x0
#define KERNEL_REALMODE_CODE_SEG		0x0
#define KERNEL_REALMODE_DATA_SEG		0x0
/* realmode address */
#define KERNEL_REALMODE_STACK_ADDR		(KERNEL_LOAD_SEGMENT*0x10)
/* realmode stack size */
#define KERNEL_REALMODE_STACK_SIZE		(KERNEL_REALMODE_STACK_ADDR-SYSTEM_ADDRESS_MAP_ENTRY)
#define KERNEL_REALMODE_CODE_ADDR		0x0
#define KERNEL_REALMODE_DATA_ADDR		KERNEL_REALMODE_CODE_ADDR

/* protected mode segments */
#define KERNEL_PROTECTEDMODE_STACK_SEG		0x18
#define KERNEL_PROTECTEDMODE_CODE_SEG		0x8
#define KERNEL_PROTECTEDMODE_DATA_SEG		0x10
#define KERNEL_PSEUDO_REALMODE_DATA_SEG		0x28
#define KERNEL_PSEUDO_REALMODE_CODE_SEG		0x20
#define KERNEL_PROTECTEDMODE_STACK_ADDR		KERNEL_REALMODE_STACK_ADDR
#define KERNEL_PROTECTEDMODE_STACK_SIZE		KERNEL_REALMODE_STACK_SIZE


/* The flag for protected mode.  */
#define CPU_CR0_PE_ON		0x1
#define CPU_CR4_PAE_ON		0x00000020
#define CPU_CR4_PSE_ON		0x00000010
#define CPU_CR0_PAGING_ON	0x80000000
#define CPU_AMD64_MSR		0xc0000080
#define CPU_AMD64_MSR_ON	0x00000100

/* C code exit signals */
#define EXIT_COLD_BOOT		0x0
#define EXIT_WARM_BOOT		0x1234
#define EXIT_CPU_HALT		0x2
#define EXIT_PANIC		0xDEAD


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
	uint32_t type; // entry Type
} phisical_address_map_entity_t;
#pragma pack(pop)

/* phisical map structure pointers */
extern phisical_address_map_entity_t *k_phisical_memory_map;
extern uint16_t k_phisical_memory_map_size;

/* version info pointers from version.S */
extern uint32_t k_version_signature;
extern char k_version_full_string[];

/* stop/interrupt funct */
void k_abort();

void k_reboot(int how);

void k_freeze();

void k_breakpoint();

#endif

#define TO_STRING(x)				#x

#endif

