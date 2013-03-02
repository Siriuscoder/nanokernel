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

#ifndef INT_HEADER
#define INT_HEADER	1

#include "kernel.h"
#include "irq.h"

#define IDT_INTEL_EXCEPTION_COUNT		32
#define IDT_COUNT_MAX					(IDT_INTEL_EXCEPTION_COUNT+IDT_IRQ_VECTORS_COUNT)

#define IDT_INTERRUPT_GATE_FLAG			0x8E
#define IDT_TRAP_GATE_FLAG				0x8F
#define IDT_TASK_GATE_FLAG				0x85

#ifndef __GAS__

/* Defines an IDT entry */
#pragma pack(push, 1)
typedef struct idt_entry
{
    uint16_t base_lo;
    uint16_t sel;
    byte always0;
    byte flags;
    uint16_t base_hi;
} idtEntry_t;
#pragma pack(pop)

/* async interrupts enable */
void k_iasync_enable();
/* async interrupts disable */
void k_iasync_disable();

/* set gate */
void k_idt_set_int_gate(byte intNum, uint32_t handler, uint16_t codeSelector, byte DPL);
void k_idt_set_task_gate(byte intNum, uint16_t tssSelector, byte DPL);
void k_idt_set_trap_gate(byte intNum, uint32_t handler, uint16_t codeSelector, byte DPL);

#endif
#endif
