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

#ifndef GRUB_KERN_HEADER
#define GRUB_KERN_HEADER	1


#define BOOL_LOADER_BODY_SIZE			0x1FE
/* boot drive index (0x00 for floppy disk A)*/
#define BOOT_DRIVE_INDEX			0x00
/* MBR signature */
#define MAIN_BOOT_RECORD_SIGNATURE		0xAA55
/* boot loader start address */
#define BOOT_LOADER_BIOS_START			0x7C00
/* kernel size in sectors (60 sectors) 60x512=30 kB */
#define BOOT_KERNEL_SIZE_IN_SECT		60
/* kernel start sector */
#define BOOT_KERNEL_START_SECTOR		0x1
/* default sector size 512 bytes */
#define DISK_SECT_SIZE			 	512
/* kernel load address 0000h:7e00h */
#define KERNEL_LOAD_ADDR			0x7e00
#define FLOPPY_SECTORS_PER_TRACK		18
#define FLOPPY_HEADS_NUM			2


/* Apple assembler requires local labels to start with a capital L */
#define LOCAL(sym)				L_ ## sym
/* Add an underscore to a C symbol in assembler code if needed. */
#if HAVE_ASM_USCORE
# define EXT_C(sym)				_ ## sym
#else
# define EXT_C(sym)				sym
#endif

#define REAL_ADR(x)				x+BOOT_LOADER_BIOS_START

#endif
