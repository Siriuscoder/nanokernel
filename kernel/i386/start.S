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

#include "kernel.h"

	.file	"start.S"

.globl _start, start;
.globl k_phisical_memory_map_size;
.globl k_phisical_memory_map;
.globl k_do_reboot_16;
.globl k_halt_system_16;

	.text

_start:
start:
/* Tell GAS to generate 16-bit instructions so that this code works 
	in real mode. */
	.code16
/* OK, we starting kernel code at 07e0h:0000h */
/* common registers initialize */
	xorl	%eax, %eax
	movl	%eax, %ebx
	movl	%eax, %ecx
	movl	%eax, %edx

	/* normalize code segment */
	ljmp	$KERNEL_REALMODE_CODE_SEG, $common_segments_init
	nop
	nop

common_segments_init:
/* turn off interrupts */
	cli
/* setup new stack */
	movw	$KERNEL_REALMODE_STACK_SEG, %ax
	movw	%ax, %ss
	movl	$KERNEL_REALMODE_STACK_ADDR, %esp
	movl	%esp, %ebp
/* setup data segment */
	movw	$KERNEL_REALMODE_DATA_SEG, %ax
	movw	%ax, %ds
/* set fs and gs to zero */
	xorw	%ax, %ax
	movw	%ax, %fs
	movw	%ax, %es
	movw	%ax, %gs	

/* turn on interrupts */
	sti

start_system_initialization:

	call	k_disable_cursor
	call 	k_reset_disk_system
	call 	k_get_memory_info

/* switch to protected mode */
	call 	k_switch_to_protected_mode
	.code32

/* jump to kernel main C code and newer return */
	call 	k_main

k_get_memory_info:
	.code16
/*
INT 15h, AX=E820h - Query System Address Map

Input:
	EAX	Function Code	E820h
	EBX	Continuation	Contains the "continuation value" to get the
				next run of physical memory.  This is the
				value returned by a previous call to this
				routine.  If this is the first call, EBX
				must contain zero.
	ES:DI	Buffer Pointer	Pointer to an  Address Range Descriptor
				structure which the BIOS is to fill in.
	ECX	Buffer Size	The length in bytes of the structure passed
				to the BIOS.  The BIOS will fill in at most
				ECX bytes of the structure or however much
				of the structure the BIOS implements.  The
				minimum size which must be supported by both
				the BIOS and the caller is 20 bytes.  Future
				implementations may extend this structure.
	EDX	Signature	'SMAP' -  Used by the BIOS to verify the
				caller is requesting the system map
				information to be returned in ES:DI.
Output:
	CF	Carry Flag	Non-Carry - indicates no error
	EAX	Signature	'SMAP' - Signature to verify correct BIOS
				revision.
	ES:DI	Buffer Pointer	Returned Address Range Descriptor pointer.
				Same value as on input.
	ECX	Buffer Size	Number of bytes returned by the BIOS in the
				address range descriptor.  The minimum size
				structure returned by the BIOS is 20 bytes.
	EBX	Continuation	Contains the continuation value to get the
				next address descriptor.  The actual
				significance of the continuation value is up
				to the discretion of the BIOS.  The caller
				must pass the continuation value unchanged
				as input to the next iteration of the E820
				call in order to get the next Address Range
				Descriptor.  A return value of zero means that
				this is the last descriptor.  Note that the
				BIOS indicate that the last valid descriptor
				has been returned by either returning a zero
				as the continuation value, or by returning
				carry.
*/
#ifdef SYSTEM_ADDRESS_MAP_ENTRY
	xorl	%ebx, %ebx
	movl 	$0xE820, %eax
	movw	%bx, %es /* 0x0 to es */
	movw	$SYSTEM_ADDRESS_MAP_ENTRY, %di
	movl	$20, %ecx /* read 20 bytes per call */
	movl	$0x534D4150, %edx /* 'SMAP' magic */
	/* Query System Address Map first call */
	int 	$0x15
	/* check result */
	jc 	get_bios_memory_map_failed

	cmpl	$0x534D4150, %eax
	jne	get_bios_memory_map_failed

	testl	%ebx, %ebx
	je	get_bios_memory_map_failed
	incw	(k_phisical_memory_map_size)

get_memory_map_loop:
	movl 	$0xE820, %eax
	addw	%cx, %di
	movl	$20, %ecx
	int 	$0x15

	/* check result */
	jc 	get_bios_memory_map_failed
	
	incw	(k_phisical_memory_map_size)
	testl	%ebx, %ebx
	je	get_memory_map_done

	jmp	get_memory_map_loop

get_memory_map_done:
#endif
	ret

k_reset_disk_system:
	.code16
/* INT 13h AH=00h: Reset Disk Drive */
	movb	$BOOT_DRIVE_INDEX, %dl
	movb 	$0x00, %ah
	int 	$0x13

	ret

k_disable_cursor:
	.code16
/* 	
	Set text-mode cursor shape AH=01h
	CH = Scan Row Start, CL = Scan Row End

	Normally a character cell has 8 scan lines, 0-7. So, CX=0607h is a normal underline cursor, 
	CX=0007h is a full-block cursor. If bit 5 of CH is set, that often means "Hide cursor". 
	So CX=2607h is an invisible cursor. Some video cards have 16 scan lines, 00h-0Fh.
	Some video cards don't use bit 5 of CH. With these, make Start>End (e.g. CX=0706h)
*/
/* save cursor position to cx */
	movb	$0x3, %ah
	xorb	%bh, %bh
	int     $0x10

	movb	$0x1, %ah
	movb	$0x20, %ch
	int     $0x10

	ret

/* set ax to 0x1234 for warm boot, and 0x0000 to cold boot */
k_do_reboot_16:
	.code16
	movw	$0x0472, %di
	movw	%ax, (%di)
	ljmp	$0xf000, $0xfff0

k_halt_system_16:
	.code16
	cli
hlt_end:
  	hlt
	jmp 	hlt_end

get_bios_memory_map_failed:
	.code16
	leaw 	get_bios_memory_map_failed_mes, %si
	call 	k_rm_message
/* stop processor */
	jmp 	k_halt_system_16

k_rm_message:
	.code16
	movw	$0x0001, %bx
	movb	$0xe, %ah
	jmp 	message_lods
display_char:
	int	$0x10		/* display a byte */
message_lods:
	lodsb
	cmpb	$0, %al
	/* if not end of string, jmp to display */
	jne	display_char
	ret

	.data
k_phisical_memory_map_size:
	.short	0x0
k_phisical_memory_map:
	.long	SYSTEM_ADDRESS_MAP_ENTRY

	.section .rodata
get_bios_memory_map_failed_mes:
	.asciz "Get BIOS memory map failed..\n"

