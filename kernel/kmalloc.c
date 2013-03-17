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
#include "memory.h"
#include "mempool.h"

static MemPool_t memPool;

bool k_heap_init()
{
	k_memset(&memPool, 0, sizeof(MemPool_t));
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

}



