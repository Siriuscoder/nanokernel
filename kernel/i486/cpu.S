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
	.globl	k_iasync_enable;
	.globl	k_iasync_disable;
	.globl	k_abort;
	.globl	k_reboot;
	.globl	k_freeze;
	.globl	k_breakpoint;
	.globl	k_ss_crash;
	.globl	k_gp_crash;
	.globl	k_opcode_crash;
	.globl	k_idle_wait;

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
	movl	8(%esp), %edi
	movl	%edx, (%edi)
	movl	12(%esp), %edi
	movl	%eax, (%edi)
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
	
k_iasync_enable:
	sti
	ret
	
k_iasync_disable:
	cli
	ret

k_abort:
	call	k_iasync_disable
hlt_loop:
	hlt
/* But if a non maskable interrupt is received, halt again */
	jmp		hlt_loop

k_reboot:
	jmp		do_reboot
/*
There are several methods for rebooting, including
1. ACPI Reset command
2. load a 0-sized IDT and issue an interrupt (that'll triple fault and reset)
3. Use the 8042 keyboard controller to pulse the CPU's RESET pin

I chose method 2..

set IDT null decriptor first 
*/ 
null_idt_descr:
	.word	0x0				/* limit */
	.long	null_idt_descr	/* addr */
do_reboot:

	lidt	null_idt_descr
/* rase debug interrupt, and good bye */
	int		$0x2

k_freeze:
	nop
	nop
	jmp		k_freeze

k_breakpoint:
	int		$0x3
	ret

k_ss_crash:
	movl	$0xD, %eax
	movl	$0xE, %ebx
	movl	$0xA, %ecx
	movl	$0xD, %edx
	movw	%ax, %ss

k_gp_crash:
	movl	$0xD, %eax
	movl	$0xE, %ebx
	movl	$0xA, %ecx
	movl	$0xD, %edx
	movw	%ax, %ds

k_opcode_crash:
	movl	$0xD, %eax
	movl	$0xE, %ebx
	movl	$0xA, %ecx
	movl	$0xD, %edx
	.long	0xffffffff

k_idle_wait:
	hlt
	ret