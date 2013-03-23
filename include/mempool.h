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

#ifndef MEMPOOL_H_
#define MEMPOOL_H_

#include <types.h>

#define PHYSICAL_MEMORY_CAPTURE_VOLUME_LOG	20// 1 MB
#define PHYSICAL_MEMORY_CAPTURE_VOLUME		(1 << PHYSICAL_MEMORY_CAPTURE_VOLUME_LOG)
#define MEMORY_SLICES_MAX_COUNT				31

struct BlockHeader_t
{
	union
	{
		struct BlockHeader_t *next; //in free block, next free block
		struct //in allocated blocks.
		{
			//byte is_malloced; //was allocated by stdlib::malloc
			byte listNum; //if not, number of list in list of lists
		};
	};
};

typedef struct MemPool
{
	struct BlockHeader_t *freeSlicesLists[MEMORY_SLICES_MAX_COUNT];
	uint32_t numAllocatedBlocks[MEMORY_SLICES_MAX_COUNT];
	uint32_t numBlockAllocations[MEMORY_SLICES_MAX_COUNT];
	ptr_t	phisicalMemBegin;
	ptr_t	phisicalMemCurrent;
	size_t 	poolMaxSize;
} MemPool_t;

ptr_t
k_pool_select(MemPool_t *pool, size_t bytes);

void
k_pool_release(MemPool_t *pool, ptr_t block);

ptr_t
k_pool_replace(MemPool_t *pool, ptr_t block, size_t bytes);

size_t
k_pool_block_size(ptr_t block);



#endif /* MEMPOOL_H_ */
