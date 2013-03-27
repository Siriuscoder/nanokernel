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

#include <std/membase.h>
#include <std/print.h>
#include <mem.h>
#include <mempool.h>

static MemPool_t memPool;

bool k_heap_init()
{
	k_memset(&memPool, 0, sizeof(MemPool_t));
	/* set kernel heap out to 2 Mb phisical memory space */
	memPool.phisicalMemPtr = (ptr_t)KERNEL_HEAP_ADDRESS;
	memPool.phisicalMemPtrMax = memPool.phisicalMemPtr + (KERNEL_HEAP_PAGES * 4096);

	return true;
}

ptr_t
k_malloc(size_t bytes)
{
	return k_pool_select(&memPool, bytes);
}

void
k_free(ptr_t block)
{
	k_pool_release(&memPool, block);
}

ptr_t
k_realloc(ptr_t block, size_t bytes)
{
	return k_pool_replace(&memPool, block, bytes);
}

void
k_memory_dump()
{
	int i;
	uint64_t memTotal = 0;
	k_print("=========== Phisical Memory Map ==========\n");
	for(i = 0; i < k_phisical_memory_map_size; i++)
	{
		k_print("0x%08x%08x - 0x%08x%08x %s\n",
			HIPART(k_phisical_memory_map[i].base),
			LOPART(k_phisical_memory_map[i].base),
			HIPART(k_phisical_memory_map[i].base + k_phisical_memory_map[i].length),
			LOPART(k_phisical_memory_map[i].base + k_phisical_memory_map[i].length),
			k_phisical_memory_map[i].type == MEMORY_USE_NORMAL ? "Use Normal" :
			(k_phisical_memory_map[i].type == MEMORY_USE_RESERVED ? "Reserved" :
			(k_phisical_memory_map[i].type == MEMORY_USE_ACPI_RECLAIMABLE ? "ACPI Reclaimable" :
			(k_phisical_memory_map[i].type == MEMORY_USE_ACPI_NVS ? "Use normal" : "ACPI NVS"))));

		if(k_phisical_memory_map[i].type == MEMORY_USE_NORMAL)
			memTotal += k_phisical_memory_map[i].length;
	}

	k_print("Usable memory total: %d bytes\n", (uint32_t)memTotal);
}



