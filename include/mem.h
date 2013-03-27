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

#ifndef MEMORY_H_
#define MEMORY_H_

#include <types.h>

#define KERNEL_HEAP_ADDRESS				0x200000
#define KERNEL_HEAP_PAGES				(256 * 20) // 20MB

#define HIPART(x)						(uint32_t)((x) >> 32)
#define LOPART(x)						(uint32_t)(x)

/*
Type 1: Usable (normal) RAM
Type 2: Reserved - unusable
Type 3: ACPI reclaimable memory
Type 4: ACPI NVS memory
Type 5: Area containing bad memory
*/

#define MEMORY_USE_NORMAL				1
#define MEMORY_USE_RESERVED				2
#define MEMORY_USE_ACPI_RECLAIMABLE		3
#define MEMORY_USE_ACPI_NVS				4
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

typedef ptr_t (*malloc_func)(size_t);
typedef void (*free_func)(ptr_t);
typedef ptr_t (*realloc_func)(ptr_t, size_t);

typedef struct mallocInfo
{
	malloc_func malloc;
	free_func free;
	realloc_func realloc;
} mallocInfo_t;

bool k_heap_init();

ptr_t
k_malloc(size_t bytes);

void
k_free(ptr_t block);

ptr_t
k_realloc(ptr_t block, size_t bytes);

void
k_memory_dump();

#endif /* MEMORY_H_ */
