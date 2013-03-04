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

#ifndef IRQ_HEADER
#define IRQ_HEADER	1

#define IDT_IRQ_VECTORS_COUNT		16

/* Default location in the IDT at which we program the PICs */
#define IRQ_MASTER_BASE             0x20
#define IRQ_SLAVE_BASE          	0x28

#define IRQ_MASTER_TIMER			0x00
#define IRQ_MASTER_KEYBOARD			0x01
#define IRQ_MASTER_CASCADE			0x02
#define IRQ_MASTER_COM_2_4			0x03
#define IRQ_MASTER_COM_1_3			0x04
#define IRQ_MASTER_LPT2				0x05
#define IRQ_MASTER_FDC				0x06
#define IRQ_MASTER_LPT1				0x07

#define IRQ_SLAVE_RTC				0x08
#define IRQ_SLAVE_IRQ2_REDIRECT		0x09
#define IRQ_SLAVE_RESERVED_02		0x0A
#define IRQ_SLAVE_RESERVED_03		0x0B
#define IRQ_SLAVE_RESERVED_04		0x0C
#define IRQ_SLAVE_COPROC			0x0D
#define IRQ_SLAVE_HDC				0x0E
#define IRQ_SLAVE_RESERVED_07		0x0F

#define IRQ_MAKEINT_MASTER(x)		(x+IRQ_MASTER_BASE)
#define IRQ_MAKEINT_SLAVE(x)		(x-8+IRQ_SLAVE_BASE)

#ifndef __GAS__


#endif

#endif

