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

#ifndef CPU_H
#define	CPU_H

/* stop/interrupt funct */
void k_abort();
void k_reboot(int how);
void k_freeze();
void k_breakpoint();
/* crash functions for debug only */
void k_ss_crash();
void k_gp_crash();
void k_opcode_crash();
void k_idle_wait();


#endif	/* CPU_H */

