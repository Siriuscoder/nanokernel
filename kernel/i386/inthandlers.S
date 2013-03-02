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

	.file "inthandlers.S"

#include "int.h"

	.globl k_idt;
	.globl k_load_idt_descriptor;

	.data
/* force 8-byte alignment */
	.p2align	3

k_idt:
	.space		(8 * IDT_COUNT_MAX)

/* force 4-byte alignment */
	.p2align	2

k_idt_descriptor:
	.word	(IDT_COUNT_MAX-1)		/* limit */
	.long	k_idt					/* addr */


	.text
	.code32

k_load_idt_descriptor:

	lidt	k_idt_descriptor
	ret





