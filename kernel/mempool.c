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
#include "std/membase.h"
#include "mempool.h"

static const byte BitTable[256] = { 0, 1, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 5,
		5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 6, 6,
		6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 7,
		7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
		7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
		7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8,
		8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
		8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
		8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
		8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
		8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, };

static void *
SystemAlloc(MemPool_t *pool, size_t size)
{
	return NULL;
}

static uint32_t
get_next_power_of_two(size_t k)
{
	--k;

	if (k < 256)
		return BitTable[k];

	if (k < 256 * 256)
		return BitTable[k >> 8] + 8;

	if (k < 256 * 256 * 256)
		return BitTable[k >> 16] + 16;

	return BitTable[k >> 24] + 24;
}

static void
add_block_to_list(MemPool_t *pool, void *p_memory, byte listNum)
{
	struct BlockHeader_t *pHeader = (struct BlockHeader_t *) p_memory; //non initialized BlockHeader_t
	pHeader->next = pool->freeSlicesLists[listNum];
	pool->freeSlicesLists[listNum] = pHeader;
}

static struct BlockHeader_t *
detach_first_block(MemPool_t *pool, byte listNum)
{
	struct BlockHeader_t *pHeader = pool->freeSlicesLists[listNum];
	pool->freeSlicesLists[listNum] = pool->freeSlicesLists[listNum]->next;
	pHeader->listNum = listNum;
	//pHeader->is_malloced = 0;
	return pHeader;
}

static void
split_block_and_add_to_lists(MemPool_t *pool, void *pRawMemory, byte firstListNum, byte memoryLogSize)
{
	void *p_memory = (char*) pRawMemory + (1U << (memoryLogSize - 1)); //set pointer to second half
	byte i = memoryLogSize - 1;
	for (; i >= firstListNum; --i)
	{
		add_block_to_list(pool, p_memory, i);
		p_memory = (char*) p_memory - (1U << (i - 1)); //leave other half for next iteration
	}

	add_block_to_list(pool, pRawMemory, firstListNum);
}

static bool
find_new_block_for_list(MemPool_t *pool, byte listNum)
{
	if (listNum >= MEMORY_SLICES_MAX_COUNT)
		return false;

	uint32_t i = listNum + 1;
	for (; i < MEMORY_SLICES_MAX_COUNT; ++i)
	{
		if (pool->freeSlicesLists[i])
			break;
	}

	void *result;

	if (i != MEMORY_SLICES_MAX_COUNT)
	{ //we have found bigger block in other list.
		result = detach_first_block(pool, i);
		split_block_and_add_to_lists(pool, result, listNum, i);
	}
	else
	{ //we need to get new block from OS.
		size_t size_of_new_os_block_log =
				(PHYSICAL_MEMORY_CAPTURE_VOLUME_LOG > listNum) ?
						PHYSICAL_MEMORY_CAPTURE_VOLUME_LOG : listNum;

		result = SystemAlloc(pool, 1U << size_of_new_os_block_log);
		if (!result)
			return false;
		split_block_and_add_to_lists(pool, result, listNum, size_of_new_os_block_log);
	}

	return true;
}

void *
k_pool_select(MemPool_t *pool, size_t bytes)
{
	if(!pool)
		return NULL;

	const byte listNum = get_next_power_of_two(bytes + sizeof(struct BlockHeader_t));

	if (!pool->freeSlicesLists[listNum])
	{
		if (!find_new_block_for_list(pool, listNum))
		{

			return NULL;
		}
	}

	pool->numAllocatedBlocks[listNum]++;
	pool->numBlockAllocations[listNum]++;
	void * res = 1 + detach_first_block(pool, listNum);

	return res;
}

void
k_pool_release(MemPool_t *pool, ptr_t block)
{
	if (!block || !pool)
		return;

	struct BlockHeader_t *pHeader = (struct BlockHeader_t *) block - 1;

	byte listNum = pHeader->listNum;
	add_block_to_list(pool, pHeader, listNum);
	pool->numAllocatedBlocks[listNum]--;
}

void *
k_pool_replace(MemPool_t *pool, ptr_t block, size_t bytes)
{

	struct BlockHeader_t *pHeader = (struct BlockHeader_t *) block - 1;

	const byte newListNum = get_next_power_of_two(bytes + sizeof(struct BlockHeader_t));
	if (pHeader->listNum != newListNum)
	{
		size_t oldSize = 1U << pHeader->listNum;
		void *newp = k_pool_select(pool, bytes);
		if (bytes <= oldSize)
			k_memcpy(newp, block, bytes);
		else
			k_memcpy(newp, block, oldSize);
		k_pool_release(pool, block);

		return newp;
	}
	else
	{
		return block;
	}

}

size_t
k_pool_block_size(ptr_t block)
{
	struct BlockHeader_t *pHeader = (struct BlockHeader_t *) block - 1;

	size_t res = 1U << pHeader->listNum;
	return res;

}


