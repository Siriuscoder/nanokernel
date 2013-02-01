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

	.file	"version.S"

.globl k_major_version;
.globl k_minor_version;
.globl k_build_version;
.globl k_version_full_string;

	.section .rodata

k_major_version:
	.byte 	MAJOR_VERSION
k_minor_version:
	.byte	MINOR_VERSION
k_build_version:
	.byte	BUILD_VERSION
k_version_full_string:
	.asciz 	KERNEL_VERSION_FULL_STRING(KERNEL_VERSION_FULL)

