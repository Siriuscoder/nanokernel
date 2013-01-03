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
	.text

.globl _start, start;
.globl k_reset_disk_system;
.globl k_do_reboot;
.globl k_halt_system;

_start:
start:
/* Tell GAS to generate 16-bit instructions so that this code works 
	in real mode. */
	.code16
/* OK, we starting kernel code at 07e0h:0000h */
/* common registers initialize */
	xorw	%ax, %ax
	movw	%ax, %bx
	movw	%ax, %cx
	movw	%ax, %dx

	/* do force ljump to 07e0h:common_segments_init */
	/* setup code segment and instruction pointer */
	ljmp	$KERNEL_REALMODE_CODE_SEG, common_segments_init
	nop

common_segments_init:
/* turn off interrupts */
	cli
/* setup new stack */
	movw	$KERNEL_REALMODE_STACK_SEG, %ss
	movw	$KERNEL_REALMODE_STACK_ADDR, %sp
	movw	%sp, %bp
/* setup data segment */
	movw	$KERNEL_REALMODE_DATA_SEG, %ds

/* turn on interrupts */
	sti

start_system_initialization:

	call 	k_reset_disk_system
	

k_reset_disk_system:
	.code16
/* INT 13h AH=00h: Reset Disk Drive */
	movb	$BOOT_DRIVE_INDEX, %dl
	movb 	$0x00, %ah
	int 	$0x13

	ret

/* set ax to 0x1234 for warm boot, and 0x0000 to cold boot */
k_do_reboot:
	.code16
	movw	$0x0472, %di
	movw	%ax, (%di)
	ljmp	$0xf000, $0xfff0

k_halt_system:
	.code16
	cli
  	hlt
	nop
	nop




	
