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

#include "int.h"
#include "std/membase.h"

/*
 * INTERRUPT DESCRIPTOR TABLE (IDT)
 *
 * The interrupt descriptor table (IDT) associates each exception or interrupt vector
 * with a gate descriptor for the procedure or task used to service the associated exception
 * or interrupt. Like the GDT and LDTs, the IDT is an array of 8-byte descriptors (in
 * protected mode). Unlike the GDT, the first entry of the IDT may contain a descriptor.
 * To form an index into the IDT, the processor scales the exception or interrupt vector
 * by eight (the number of bytes in a gate descriptor). Because there are only 256 interrupt
 * or exception vectors, the IDT need not contain more than 256 descriptors. It can
 * contain fewer than 256 descriptors, because descriptors are required only for the
 * interrupt and exception vectors that may occur. All empty descriptor slots in the IDT
 * should have the present flag for the descriptor set to 0.
 * The base addresses of the IDT should be aligned on an 8-byte boundary to maximize
 * performance of cache line fills. The limit value is expressed in bytes and is added to
 * the base address to get the address of the last valid byte. A limit value of 0 results in
 * exactly 1 valid byte. Because IDT entries are always eight bytes long, the limit should
 * always be one less than an integral multiple of eight (that is, 8N – 1).
 * The IDT may reside anywhere in the linear address space. As shown in Figure 6-1,
 * the processor locates the IDT using the IDTR register. This register holds both a
 * 32-bit base address and 16-bit limit for the IDT.
 * The LIDT (load IDT register) and SIDT (store IDT register) instructions load and store
 * the contents of the IDTR register, respectively. The LIDT instruction loads the IDTR
 * register with the base address and limit held in a memory operand. This instruction
 * can be executed only when the CPL is 0. It normally is used by the initialization code
 * of an operating system when creating an IDT. An operating system also may use it to
 * change from one IDT to another. The SIDT instruction copies the base and limit value
 * stored in IDTR to memory. This instruction can be executed at any privilege level.
 * If a vector references a descriptor beyond the limit of the IDT, a general-protection
 * exception (#GP) is generated.
 *
 *
 * Table 6-1. Protected-Mode Exceptions and Interrupts
 *
 *	Vector No.	Mnemonic	Description						Type		Error		Code Source
	0 			#DE 		Divide Error 					Fault 		No 			DIV and IDIV instructions.
	1 			#DB 		RESERVED 						Fault/Trap	No 			For Intel use only.
	2 			— 			NMI Interrupt 					Interrupt 	No 			Nonmaskable external interrupt.
	3 			#BP 		Breakpoint 						Trap 		No 			INT 3 instruction.
	4 			#OF 		Overflow 						Trap 		No 			INTO instruction.
	5 			#BR 		BOUND Range Exceeded 			Fault 		No 			BOUND instruction.
	6 			#UD 		Invalid Opcode 					Fault 		No 			UD2 instruction or reserved opcode.1
	7 			#NM 		Device Not Available 			Fault 		No 			Floating-point or WAIT/FWAIT instruction.
	8 			#DF 		Double Fault 					Abort 		Yes(zero)	Any instruction that can generate an exception, an NMI,or an INTR.
	9 														Fault 		No 			Coprocessor Segment Overrun (reserved)
	10 			#TS 		Invalid TSS 					Fault 		Yes 		Task switch or TSS access.
	11 			#NP 		Segment Not Present 			Fault 		Yes 		Loading segment registers or accessing system segments.
	12 			#SS 		Stack-Segment Fault 			Fault 		Yes 		Stack operations and SS register loads.
	13 			#GP 		General Protection 				Fault 		Yes 		Any memory reference and other protection checks.
	14 			#PF 		Page Fault 						Fault 		Yes 		Any memory reference.
	15 			— 			(Intel reserved. Do not use.)
	16 			#MF 		x87 FPU Floating-Point Error	Fault 		No 			x87 FPU floating-point or WAIT/FWAIT instruction.
	18 			#MC 		Machine Check 					Abort 		No 			Error codes (if any) and source are model dependent.
	19 			#XM 		SIMD Floating-Point Exception 	Fault 		No 			SSE/SSE2/SSE3 floating-point instructions
	20-31 			— 		Intel reserved. Do not use.
	32-255			— 		User Defined (Nonreserved)
 *
 */

extern void k_load_idt_descriptor();
extern idtEntry_t k_idt[];

static void setup_idt_descriptor(byte intNum, idtEntry_t *desc)
{
	k_memcpy(&k_idt[intNum], desc, sizeof(idtEntry_t));
}

void k_idt_set_int_gate(byte intNum, uint32_t handler, uint16_t codeSelector, byte DPL)
{
	idtEntry_t gate;

	gate.always0 = 0;
	gate.base_lo = handler & 0xffff;
	gate.base_hi = (handler >> 16) & 0xffff;

	gate.sel = codeSelector;
	gate.flags = IDT_INTERRUPT_GATE_FLAG;
	gate.flags |= (DPL << 5);

	setup_idt_descriptor(intNum, &gate);
}

void k_idt_set_task_gate(byte intNum, uint16_t tssSelector, byte DPL)
{
	idtEntry_t gate;

	gate.always0 = 0;
	gate.base_lo = 0;
	gate.base_hi = 0;

	gate.sel = tssSelector;
	gate.flags = IDT_TASK_GATE_FLAG;
	gate.flags |= (DPL << 5);

	setup_idt_descriptor(intNum, &gate);
}

void k_idt_set_trap_gate(byte intNum, uint32_t handler, uint16_t codeSelector, byte DPL)
{
	idtEntry_t gate;

	gate.always0 = 0;
	gate.base_lo = handler & 0xffff;
	gate.base_hi = (handler >> 16) & 0xffff;

	gate.sel = codeSelector;
	gate.flags = IDT_TRAP_GATE_FLAG;
	gate.flags |= (DPL << 5);

	setup_idt_descriptor(intNum, &gate);
}



