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

#ifndef KERNEL_HEADER
#define KERNEL_HEADER	1

#include "loader.h"

/* realmode segments */
#define KERNEL_REALMODE_STACK_SEG		0x0
#define KERNEL_REALMODE_CODE_SEG		KERNEL_LOAD_SEGMENT
#define KERNEL_REALMODE_DATA_SEG		KERNEL_REALMODE_CODE_SEG
/* realmode address */
#define KERNEL_REALMODE_STACK_ADDR		(KERNEL_LOAD_SEGMENT*0x10)
/* realmode stack size */
#define KERNEL_REALMODE_STACK_SIZE		(KERNEL_REALMODE_STACK_ADDR-0x70)
#define KERNEL_REALMODE_CODE_ADDR		0x0
#define KERNEL_REALMODE_DATA_ADDR		KERNEL_REALMODE_CODE_ADDR

#endif
