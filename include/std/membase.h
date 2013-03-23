/* membase.h -- definitions for memory copy functions.  Generic C version.
 Copyright (C) 1991, 1992, 1993 Free Software Foundation, Inc.
 Contributed by Torbjorn Granlund (tege@sics.se).

 The GNU C Library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Library General Public License as
 published by the Free Software Foundation; either version 2 of the
 License, or (at your option) any later version.

 The GNU C Library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Library General Public License for more details.

 You should have received a copy of the GNU Library General Public
 License along with the GNU C Library; see the file COPYING.LIB.  If
 not, write to the Free Software Foundation, Inc., 675 Mass Ave,
 Cambridge, MA 02139, USA.  */

/* The strategy of the memory functions is:

 1. Copy bytes until the destination pointer is aligned.

 2. Copy words in unrolled loops.  If the source and destination
 are not aligned in the same way, use word memory operations,
 but shift and merge two read words before writing.

 3. Copy the few remaining bytes.

 This is fast on processors that have at least 10 registers for
 allocation by GCC, and that can access memory at reg+const in one
 instruction.

 I made an "exhaustive" test of this memmove when I wrote it,
 exhaustive in the sense that I tried all alignment and length
 combinations, with and without overlap.  */

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

#ifndef MEMBASE_H_
#define MEMBASE_H_

#include <types.h>

/* memory manipulate functions */
int
k_memcmp(ptr_t s1, ptr_t s2, size_t len);
ptr_t
k_memchr(const ptr_t s, int c, size_t n);
ptr_t
k_memcpy(ptr_t dstpp, const ptr_t srcpp, size_t len);
ptr_t
k_memmem(const ptr_t haystack, const size_t haystack_len, const ptr_t needle,
		const size_t needle_len);
ptr_t
k_memmove(ptr_t dest, const ptr_t src, size_t len);
ptr_t
k_memset(ptr_t dstpp, int c, size_t len);

#endif /* MEMBASE_H_ */

