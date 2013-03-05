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
	.file "stop.S"
	.text

#include "kernel.h"

	.globl k_abort;
	.globl k_reboot;
	.globl k_freeze;
	.globl k_breakpoint;

k_abort:
	.code32
	/* switch to realmode */
	call	k_switch_to_real_mode
	.code16

	jmp		k_halt_system_16

k_reboot:
	.code32
	movl	%esp, %ebp
	/* switch to realmode */
	call	k_switch_to_real_mode
	.code16

	movl	4(%bp), %eax
	jmp		k_do_reboot_16

k_freeze:
	.code32
	nop
	nop
	jmp		k_freeze

k_breakpoint:
	movl	$0x5, %eax
	movl	$0x6, %ebx
	movl	$0x7, %ecx
	int		$0x3
	ret

