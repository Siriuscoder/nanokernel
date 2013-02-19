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

#ifndef IO_HEADER
#define IO_HEADER	1

#include "kernel.h"

#ifndef __GAS__
/* Forces the CPU to wait for an I/O operation to complete. 
only use this when there's nothing like a status register 
or an IRQ to tell you the info has been received. */
void k_io_wait();

/* Sends a byte (or word or dword) on a I/O location. 
Traditional names are 'outb', 'outw' and 'outl' respectively. 
The "a" modifier enforces 'val' to be placed in the eax register 
before the asm command is issued and "Nd" allows for one-byte 
constant values to be assembled as constants, freeing the edx 
register for other cases. */
void k_io_port_outb(uint16_t port, byte data);

/* Receives a byte (or word or dword) from an I/O location.
Traditional names are 'inb', 'inw' and 'inl' respectively.
*/
byte k_io_port_inb(uint16_t port);

#endif

#endif
