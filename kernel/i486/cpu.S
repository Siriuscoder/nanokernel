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
	.globl	k_get_msr;
	.globl	k_set_msr;
	.globl	k_irq_enable;
	.globl	k_irq_disable;

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

/* ReaD from Model-Specific Register */
/* msr address */
/* out: uint32_t hw */
/* out: uint32_t lw */
k_get_msr:
	
	movl	4(%esp), %ecx
	rdmsr
	movl	8(%esp), %ebx
	movl	%edx, (%ebx)
	movl	12(%esp), %ebx
	movl	%eax, (%ebx)
	ret
/* WRite to Model-Specific Register */
/* msr address */
/* uint32_t hw */
/* uint32_t lw */
k_set_msr:

	movl	4(%esp), %ecx
	movl	8(%esp), %edx
	movl	12(%esp), %eax
	wrmsr
	ret

/* in uint32_t mode */
/* out uint32_t eax */
/* out uint32_t ebx */
/* out uint32_t ecx */
/* out uint32_t edx */
k_get_cpu_info:
	movl	4(%esp), %eax
	cpuid

	movl	8(%esp), %edi
	movl	%eax, (%edi)
	movl	12(%esp), %edi
	movl	%ebx, (%edi)
	movl	16(%esp), %edi
	movl	%ecx, (%edi)
	movl	20(%esp), %edi
	movl	%edx, (%edi)
	ret	
	
k_irq_enable:
	sti
	ret
	
k_irq_disable:
	cli
	ret

