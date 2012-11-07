/*  This file is part of simple bool loader.
    Project NanoKernel (for study purposes only)
    Copyright (C) 2012  Sirius (Vdov Nikita)

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

#include "kern.h"

	.file	"boot.S"

	.text

	/* Tell GAS to generate 16-bit instructions so that this code works
	   in real mode. */
	.code16

.globl _boot, boot;

_boot:
boot:
	/*
	 *  boot is loaded at 0x7c00 and is jumped to with CS:IP 0:0x7c00
	 */
	/* common registers initialize */
	xorw	%ax, %ax
	movw	%ax, %bx
	movw	%ax, %cx
	movw	%ax, %dx

	jmp	real_start
	nop

DAP:
	/* size of DAP = 16 = 10h */
	.byte	0x10
	/* unused, should be zero */
	.byte	0x00
	/* number of sectors to be read, (some Phoenix BIOSes are limited to a maximum of 127 sectors) */
	.short	BOOT_KERNEL_SIZE_IN_SECT
	/* segment:offset pointer to the memory buffer to which sectors will be transferred 
	(note that x86 is little-endian: if declaring the segment and offset separately, 
	the offset must be declared before the segment) 
	*/
	.short  0x0
	.short	KERNEL_LOAD_ADDR
	/* absolute number of the start of the sectors to be read (1st sector of drive has number 0) */
	.quad 	1
CHS_parameters:
	/* number of the heads */
	.byte 	FLOPPY_HEADS_NUM
	/* number of sectors per track */	
	.byte	FLOPPY_SECTORS_PER_TRACK
	
real_start:

	/* turn off interrupts */
	cli

	/* setup stack */
	/* stack located in lower memory, 
	/* below boot address we have 0x7c00-0x70 = 30 kb */
	/* set stack segment = 0*/
	movw	%ax, %ss
	/* so, set stack pointer to 0x7c00 */
	movw	$BOOT_LOADER_BIOS_START, %sp
	movw	%sp, %bp
	/* set data segment = 0 */
	movw	%ax, %ds

	/* turn on interrupts */
	sti

	/* set boot drive */
	movb	$BOOT_DRIVE_INDEX, %dl
	/* save drive reference first thing! */
	pushw	%dx

	/* check if LBA is supported */
	/* BIOS INT 13h AH=41h: Check Extensions Present */
	movb	$0x41, %ah
	movw	$0x55aa, %bx
	int	$0x13

	popw	%dx

	/* check result */
	jc	lba_not_supported
	cmpw	$0xaa55, %bx
	jne	lba_not_supported
	/* drive control using DAP structure? */ 
	andw	$1, %cx
	jz	lba_not_supported

lba_mode:
/*
 * BIOS call "INT 0x13 Function 0x42" to read sectors from disk into memory
 *	Call with	%ah = 0x42
 *			%dl = drive number
 *			%ds:%si = segment:offset of disk address packet
 *	Return:
 *			%al = 0x0 on success; err code on failure
 */

	movb	$0x42, %ah
	movw	$REAL_ADR(DAP), %si
	int	$0x13
	jnc	boot_next

CHS_mode:
	push	%dx
	movw	$REAL_ADR(CHS_parameters), %si
	/*
	 *  Determine the hard disk geometry from the BIOS!
	 *  INT 13h AH=08h: Read Drive Parameters
	 */
	movb	$0x08, %ah
	int	$0x13
	/* read disk parameters failed, use standart floppy parameters */
	jc	CHS_read

	/* expand disk parameters */
	movw	%cx, %ax
	andw	$0x3f, %ax
	/* save num of sectors per track */
	movb	%al, 1(%si)
	movzbw  %dh, %ax
	incw	%ax
	/* save number of heads */
	movb	%al, (%si)

CHS_read:
	/* restore drive index */
	popw	%dx
	/* calc sect in cylinder */
	movb	1(%si), %al
	mulb	(%si)
	/* save it in stack */
	movw	%sp, %bp
	pushw	%ax
	/* sector counter */
	movw	$BOOT_KERNEL_START_SECTOR, %ax
	pushw	%ax
	/* block address */
	movw	$KERNEL_LOAD_ADDR, %bx

CHS_read_sector:
	// calc next CHS coordinates
	// calc next sector no
	movw	-4(%bp), %ax
	divb	1(%si)
	movzbw	%ah, %cx
	// calc head no
	movb	$0x0, %ah
	divb	(%si)
	movb	%ah, %dh
	pushw	%dx
	// calc cylinder no
	xorw	%dx, %dx
	movw	-4(%bp), %ax
	divw	-2(%bp)
	/* restore dx */
	popw	%dx
/*
	CX =       ---CH--- ---CL---
	cylinder : 76543210 98
	sector   :            543210

	low bits of cylinder start
*/
	movb	%al, %ch
	/* high bits of cylinder start */
	xorb	%al, %al
	shrw	$2, %ax
	orb	%al, %cl
/*
 * BIOS call "INT 0x13 Function 0x2" to read sectors from disk into memory
 *	Call with	%ah = 0x2
 *			%al = number of sectors
 *			%ch = cylinder
 *			%cl = sector (bits 6-7 are high bits of "cylinder")
 *			%dh = head
 *			%dl = drive (0x80 for hard disk, 0x0 for floppy disk)
 *			%es:%bx = segment:offset of buffer
 *	Return:
 *			%al = 0x0 on success; err code on failure
 */
	movw	$0x0201, %ax	
	int	$0x13

	jc	error_disk_read

	/* to next sector */
	incw	-4(%bp)
	/* to next memory block */
	addw	$DISK_SECT_SIZE, %bx
	cmpw	$(BOOT_KERNEL_SIZE_IN_SECT+BOOT_KERNEL_START_SECTOR), -4(%bp)
	jl	CHS_read_sector

	/* restore stack */
	movw	%bp, %sp
	jmp 	boot_next

lba_not_supported:
	pushw	%si
	movw 	$REAL_ADR(lba_not_supported_err_mes), %si
	call 	message
	popw	%si
	jmp 	CHS_mode

error_disk_read:
	movw 	$REAL_ADR(disk_read_error_mes), %si
	call 	message
	jmp 	stop_boot

	/*
	 * Use BIOS "int 10H Function 0Eh" to write character in teletype mode
	 *	%ah = 0xe	%al = character
	 *	%bh = page	%bl = foreground color (graphics modes)
	 */
message:
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

stop_boot:
	jmp 	stop_boot
boot_next:
	movw 	$REAL_ADR(success_mes), %si
	call 	message
	ljmp	$0, $KERNEL_LOAD_ADDR

lba_not_supported_err_mes:
	.asciz "LBA not supported, trying CHS mode\n"
disk_read_error_mes:
	.asciz "Disk read error, boot failed..\n"
success_mes:
	.asciz "Boot OK\n"

	. = _boot + BOOL_LOADER_BODY_SIZE
	.short	MAIN_BOOT_RECORD_SIGNATURE




