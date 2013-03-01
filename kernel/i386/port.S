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

	.file	"iobase.S"


	.globl	k_io_wait;
	.globl	k_io_port_outb;
	.globl	k_io_port_inb;

	.text
	.code32

k_io_wait:

	jmp	to_next_jmp
to_next_jmp:
	jmp	to_ret
to_ret:
	ret

/* in word port */
/* in byte data */
k_io_port_outb:
	
	movw	4(%esp), %dx
	movb	6(%esp), %al
	outb	%al, %dx
	ret

/* in word port */
k_io_port_inb:

	movw	4(%esp), %dx
	inb	%dx, %al
	ret



