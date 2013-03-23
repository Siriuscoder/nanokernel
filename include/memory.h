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
