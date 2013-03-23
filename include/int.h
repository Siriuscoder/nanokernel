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

#include <kernel.h>
#include <irq.h>

#define IDT_INTEL_EXCEPTION_COUNT		32
#define IDT_COUNT_MAX					(IDT_INTEL_EXCEPTION_COUNT+IDT_IRQ_VECTORS_COUNT)

#define IDT_INTERRUPT_GATE_FLAG			0x8E
#define IDT_TRAP_GATE_FLAG				0x8F
#define IDT_TASK_GATE_FLAG				0x85

#define IDT_GATE_DPL_0					0x00
#define IDT_GATE_DPL_1					0x20
#define IDT_GATE_DPL_2					0x40
#define IDT_GATE_DPL_3					0x60

#define EXC_DIVIDE_ERROR				0x00
#define EXC_NMI							0x02
#define EXC_BREAKPOINT					0x03
#define EXC_OVERFLOW					0x04
#define EXC_BOUND_RANGE_EXCEEDED		0x05
#define EXC_INVALID_OPCODE				0x06
#define EXC_DEVICE_NOT_AVAILABLE		0x07
#define EXC_DOUBLE_FAULT				0x08
#define EXC_INVALID_TSS					0x0A
#define EXC_SEGMENT_NOT_PRESENT			0x0B
#define EXC_STACK_SEGMENT_FAULT			0x0C
#define EXC_GENERAL_PROTECTION			0x0D
#define EXC_PAGE_FAULT					0x0E
#define EXC_FPU_ERROR					0x10
#define EXC_ALIGMENT_CHECK				0x11
#define EXC_MACHINE_CHECK				0x12
#define EXC_FLOATING_POINT				0x13

#define INT_NO_PARAM					0x00
#define INT_WITH_PARAM					0x01
#define INT_IRQ							0x02

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
void k_idt_set_int_gate(byte intNum, const ptr_t handler, uint16_t codeSelector, byte DPL);
void k_idt_set_task_gate(byte intNum, uint16_t tssSelector, byte DPL);
void k_idt_set_trap_gate(byte intNum, const ptr_t handler, uint16_t codeSelector, byte DPL);

bool k_idt_init();
bool k_interrupts_init();

#endif
#endif
