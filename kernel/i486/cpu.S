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

	.file	"cpu.S"


	.globl	k_check_cpuid;
	.globl	k_get_cpu_info;

	.text
	.code32

/*
The Intel486 family and subsequent Intel processors provide a
straightforward method for determining whether the processor's
internal architecture is able to execute the CPUID instruction.
This method uses the ID flag in bit 21 of the EFLAGS register. If
software can change the value of this flag, the CPUID instruction
is executable
*/
k_check_cpuid:

	pushfl
	popl 	%eax
	movl 	%eax, %ebx
	xorl 	$0x200000, %eax
	pushl 	%eax
	popfl
	pushfl
	popl 	%eax
	xorl 	%eax, %ebx
	jne 	cpuid_supported
/* return zero if not supported */
	xorl	%eax, %eax

cpuid_supported:
	ret



