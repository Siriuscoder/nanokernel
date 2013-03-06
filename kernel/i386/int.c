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
#include "std/print.h"
#include "pic.h"
#include "io.h"
#include "cpuinfo.h"
#include "kerror.h"


#define DECLARE_INTERRUPT_POINTER(x)	extern void x(void);
#define INTERRUPT_POINTER(x)			((ptr_t)x)

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

const char *k_exception_descr[21] = {
		"Divide Error",
		"RESERVED",
		"NMI Interrupt",
		"Breakpoint",
		"Overflow",
		"BOUND Range Exceeded",
		"Invalid Opcode",
		"Device Not Available",
		"Double Fault",
		"Coprocessor Segment Overrun",
		"Invalid TSS",
		"Segment Not Present",
		"Stack-Segment Fault",
		"General Protection",
		"Page Fault",
		"RESERVED",
		"x87 FPU Floating-Point Error",
		"Alignment Check",
		"Machine Check",
		"SIMD Floating-Point Exception",
		0x0
};

DECLARE_INTERRUPT_POINTER(k_stub_handler);

DECLARE_INTERRUPT_POINTER(k_DE_handler);
DECLARE_INTERRUPT_POINTER(k_NMI_handler);
DECLARE_INTERRUPT_POINTER(k_BP_handler);
DECLARE_INTERRUPT_POINTER(k_OF_handler);
DECLARE_INTERRUPT_POINTER(k_BR_handler);
DECLARE_INTERRUPT_POINTER(k_UD_handler);
DECLARE_INTERRUPT_POINTER(k_NM_handler);
DECLARE_INTERRUPT_POINTER(k_DF_handler);
DECLARE_INTERRUPT_POINTER(k_TS_handler);
DECLARE_INTERRUPT_POINTER(k_NP_handler);
DECLARE_INTERRUPT_POINTER(k_SS_handler);
DECLARE_INTERRUPT_POINTER(k_GP_handler);
DECLARE_INTERRUPT_POINTER(k_PF_handler);
DECLARE_INTERRUPT_POINTER(k_MF_handler);
DECLARE_INTERRUPT_POINTER(k_AC_handler);
DECLARE_INTERRUPT_POINTER(k_MC_handler);
DECLARE_INTERRUPT_POINTER(k_XM_handler);

DECLARE_INTERRUPT_POINTER(k_irq_master_00);
DECLARE_INTERRUPT_POINTER(k_irq_master_01);
DECLARE_INTERRUPT_POINTER(k_irq_master_02);
DECLARE_INTERRUPT_POINTER(k_irq_master_03);
DECLARE_INTERRUPT_POINTER(k_irq_master_04);
DECLARE_INTERRUPT_POINTER(k_irq_master_05);
DECLARE_INTERRUPT_POINTER(k_irq_master_06);
DECLARE_INTERRUPT_POINTER(k_irq_master_07);

DECLARE_INTERRUPT_POINTER(k_irq_slave_00);
DECLARE_INTERRUPT_POINTER(k_irq_slave_01);
DECLARE_INTERRUPT_POINTER(k_irq_slave_02);
DECLARE_INTERRUPT_POINTER(k_irq_slave_03);
DECLARE_INTERRUPT_POINTER(k_irq_slave_04);
DECLARE_INTERRUPT_POINTER(k_irq_slave_05);
DECLARE_INTERRUPT_POINTER(k_irq_slave_06);
DECLARE_INTERRUPT_POINTER(k_irq_slave_07);

static void setup_idt_descriptor(byte intNum, idtEntry_t *desc)
{
	k_memcpy(&k_idt[intNum], desc, sizeof(idtEntry_t));
}

void k_idt_set_int_gate(byte intNum, const ptr_t handler, uint16_t codeSelector, byte DPL)
{
	idtEntry_t gate;
	long int handlerPtr = (long int)handler;

	gate.always0 = 0;
	gate.base_lo = handlerPtr & 0xffff;
	gate.base_hi = (handlerPtr >> 16) & 0xffff;

	gate.sel = codeSelector;
	gate.flags = IDT_INTERRUPT_GATE_FLAG;
	gate.flags |= DPL;

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
	gate.flags |= DPL;

	setup_idt_descriptor(intNum, &gate);
}

void k_idt_set_trap_gate(byte intNum, const ptr_t handler, uint16_t codeSelector, byte DPL)
{
	idtEntry_t gate;
	long int handlerPtr = (long int)handler;

	gate.always0 = 0;
	gate.base_lo = handlerPtr & 0xffff;
	gate.base_hi = (handlerPtr >> 16) & 0xffff;

	gate.sel = codeSelector;
	gate.flags = IDT_TRAP_GATE_FLAG;
	gate.flags |= DPL;

	setup_idt_descriptor(intNum, &gate);
}

bool k_idt_init()
{
	/* clean idt first */
	k_memset(&k_idt[0], 0, sizeof(idtEntry_t) * IDT_COUNT_MAX);

	/* setup exceptions handlers gates */
	k_idt_set_int_gate(EXC_DIVIDE_ERROR, INTERRUPT_POINTER(k_DE_handler),
			KERNEL_PROTECTEDMODE_CODE_SEG, IDT_GATE_DPL_0);
/*
	k_idt_set_int_gate(1, INTERRUPT_POINTER(k_stub_handler),
			KERNEL_PROTECTEDMODE_CODE_SEG, IDT_GATE_DPL_0);
*/
	k_idt_set_int_gate(EXC_NMI, INTERRUPT_POINTER(k_NMI_handler),
			KERNEL_PROTECTEDMODE_CODE_SEG, IDT_GATE_DPL_0);

	k_idt_set_trap_gate(EXC_BREAKPOINT, INTERRUPT_POINTER(k_BP_handler),
			KERNEL_PROTECTEDMODE_CODE_SEG, IDT_GATE_DPL_0);

	k_idt_set_trap_gate(EXC_OVERFLOW, INTERRUPT_POINTER(k_OF_handler),
			KERNEL_PROTECTEDMODE_CODE_SEG, IDT_GATE_DPL_0);

	k_idt_set_int_gate(EXC_BOUND_RANGE_EXCEEDED, INTERRUPT_POINTER(k_BR_handler),
			KERNEL_PROTECTEDMODE_CODE_SEG, IDT_GATE_DPL_0);

	k_idt_set_int_gate(EXC_INVALID_OPCODE, INTERRUPT_POINTER(k_UD_handler),
			KERNEL_PROTECTEDMODE_CODE_SEG, IDT_GATE_DPL_0);

	k_idt_set_int_gate(EXC_DEVICE_NOT_AVAILABLE, INTERRUPT_POINTER(k_NM_handler),
			KERNEL_PROTECTEDMODE_CODE_SEG, IDT_GATE_DPL_0);

	k_idt_set_int_gate(EXC_DOUBLE_FAULT, INTERRUPT_POINTER(k_DF_handler),
			KERNEL_PROTECTEDMODE_CODE_SEG, IDT_GATE_DPL_0);
/*
	k_idt_set_int_gate(9, INTERRUPT_POINTER(k_stub_handler),
			KERNEL_PROTECTEDMODE_CODE_SEG, IDT_GATE_DPL_0);
*/
	k_idt_set_int_gate( EXC_INVALID_TSS, INTERRUPT_POINTER(k_TS_handler),
			KERNEL_PROTECTEDMODE_CODE_SEG, IDT_GATE_DPL_0);

	k_idt_set_int_gate(EXC_SEGMENT_NOT_PRESENT, INTERRUPT_POINTER(k_NP_handler),
			KERNEL_PROTECTEDMODE_CODE_SEG, IDT_GATE_DPL_0);

	k_idt_set_int_gate(EXC_STACK_SEGMENT_FAULT, INTERRUPT_POINTER(k_SS_handler),
			KERNEL_PROTECTEDMODE_CODE_SEG, IDT_GATE_DPL_0);

	k_idt_set_int_gate(EXC_GENERAL_PROTECTION, INTERRUPT_POINTER(k_GP_handler),
			KERNEL_PROTECTEDMODE_CODE_SEG, IDT_GATE_DPL_0);

	k_idt_set_int_gate(EXC_PAGE_FAULT, INTERRUPT_POINTER(k_PF_handler),
			KERNEL_PROTECTEDMODE_CODE_SEG, IDT_GATE_DPL_0);
/*
	k_idt_set_int_gate(15, INTERRUPT_POINTER(k_stub_handler),
			KERNEL_PROTECTEDMODE_CODE_SEG, IDT_GATE_DPL_0);
*/
	k_idt_set_int_gate(EXC_FPU_ERROR, INTERRUPT_POINTER(k_MF_handler),
			KERNEL_PROTECTEDMODE_CODE_SEG, IDT_GATE_DPL_0);
	k_idt_set_int_gate(EXC_ALIGMENT_CHECK, INTERRUPT_POINTER(k_AC_handler),
			KERNEL_PROTECTEDMODE_CODE_SEG, IDT_GATE_DPL_0);
	k_idt_set_int_gate(EXC_MACHINE_CHECK, INTERRUPT_POINTER(k_MC_handler),
			KERNEL_PROTECTEDMODE_CODE_SEG, IDT_GATE_DPL_0);
	k_idt_set_int_gate(EXC_FLOATING_POINT, INTERRUPT_POINTER(k_XM_handler),
			KERNEL_PROTECTEDMODE_CODE_SEG, IDT_GATE_DPL_0);

	k_idt_set_int_gate(20, INTERRUPT_POINTER(k_stub_handler),
			KERNEL_PROTECTEDMODE_CODE_SEG, IDT_GATE_DPL_0);
	k_idt_set_int_gate(21, INTERRUPT_POINTER(k_stub_handler),
			KERNEL_PROTECTEDMODE_CODE_SEG, IDT_GATE_DPL_0);
	k_idt_set_int_gate(22, INTERRUPT_POINTER(k_stub_handler),
			KERNEL_PROTECTEDMODE_CODE_SEG, IDT_GATE_DPL_0);
	k_idt_set_int_gate(23, INTERRUPT_POINTER(k_stub_handler),
			KERNEL_PROTECTEDMODE_CODE_SEG, IDT_GATE_DPL_0);
	k_idt_set_int_gate(24, INTERRUPT_POINTER(k_stub_handler),
			KERNEL_PROTECTEDMODE_CODE_SEG, IDT_GATE_DPL_0);
	k_idt_set_int_gate(25, INTERRUPT_POINTER(k_stub_handler),
			KERNEL_PROTECTEDMODE_CODE_SEG, IDT_GATE_DPL_0);
	k_idt_set_int_gate(26, INTERRUPT_POINTER(k_stub_handler),
			KERNEL_PROTECTEDMODE_CODE_SEG, IDT_GATE_DPL_0);
	k_idt_set_int_gate(27, INTERRUPT_POINTER(k_stub_handler),
			KERNEL_PROTECTEDMODE_CODE_SEG, IDT_GATE_DPL_0);
	k_idt_set_int_gate(28, INTERRUPT_POINTER(k_stub_handler),
			KERNEL_PROTECTEDMODE_CODE_SEG, IDT_GATE_DPL_0);
	k_idt_set_int_gate(29, INTERRUPT_POINTER(k_stub_handler),
			KERNEL_PROTECTEDMODE_CODE_SEG, IDT_GATE_DPL_0);
	k_idt_set_int_gate(30, INTERRUPT_POINTER(k_stub_handler),
			KERNEL_PROTECTEDMODE_CODE_SEG, IDT_GATE_DPL_0);
	k_idt_set_int_gate(31, INTERRUPT_POINTER(k_stub_handler),
			KERNEL_PROTECTEDMODE_CODE_SEG, IDT_GATE_DPL_0);


	/* setup IQR master handlers gates */
	k_idt_set_int_gate(IRQ_MAKEINT_MASTER(IRQ_MASTER_TIMER), INTERRUPT_POINTER(k_irq_master_00),
			KERNEL_PROTECTEDMODE_CODE_SEG, IDT_GATE_DPL_0);
	k_idt_set_int_gate(IRQ_MAKEINT_MASTER(IRQ_MASTER_KEYBOARD), INTERRUPT_POINTER(k_irq_master_01),
			KERNEL_PROTECTEDMODE_CODE_SEG, IDT_GATE_DPL_0);
	k_idt_set_int_gate(IRQ_MAKEINT_MASTER(IRQ_MASTER_CASCADE), INTERRUPT_POINTER(k_irq_master_02),
			KERNEL_PROTECTEDMODE_CODE_SEG, IDT_GATE_DPL_0);
	k_idt_set_int_gate(IRQ_MAKEINT_MASTER(IRQ_MASTER_COM_2_4), INTERRUPT_POINTER(k_irq_master_03),
			KERNEL_PROTECTEDMODE_CODE_SEG, IDT_GATE_DPL_0);
	k_idt_set_int_gate(IRQ_MAKEINT_MASTER(IRQ_MASTER_COM_1_3), INTERRUPT_POINTER(k_irq_master_04),
			KERNEL_PROTECTEDMODE_CODE_SEG, IDT_GATE_DPL_0);
	k_idt_set_int_gate(IRQ_MAKEINT_MASTER(IRQ_MASTER_LPT2), INTERRUPT_POINTER(k_irq_master_05),
			KERNEL_PROTECTEDMODE_CODE_SEG, IDT_GATE_DPL_0);
	k_idt_set_int_gate(IRQ_MAKEINT_MASTER(IRQ_MASTER_FDC), INTERRUPT_POINTER(k_irq_master_06),
			KERNEL_PROTECTEDMODE_CODE_SEG, IDT_GATE_DPL_0);
	k_idt_set_int_gate(IRQ_MAKEINT_MASTER(IRQ_MASTER_LPT1), INTERRUPT_POINTER(k_irq_master_07),
			KERNEL_PROTECTEDMODE_CODE_SEG, IDT_GATE_DPL_0);

	/* setup IQR slave handlers gates */
	k_idt_set_int_gate(IRQ_MAKEINT_SLAVE(IRQ_SLAVE_RTC), INTERRUPT_POINTER(k_irq_slave_00),
			KERNEL_PROTECTEDMODE_CODE_SEG, IDT_GATE_DPL_0);
	k_idt_set_int_gate(IRQ_MAKEINT_SLAVE(IRQ_SLAVE_IRQ2_REDIRECT), INTERRUPT_POINTER(k_irq_slave_01),
			KERNEL_PROTECTEDMODE_CODE_SEG, IDT_GATE_DPL_0);
	k_idt_set_int_gate(IRQ_MAKEINT_SLAVE(IRQ_SLAVE_RESERVED_02), INTERRUPT_POINTER(k_irq_slave_02),
			KERNEL_PROTECTEDMODE_CODE_SEG, IDT_GATE_DPL_0);
	k_idt_set_int_gate(IRQ_MAKEINT_SLAVE(IRQ_SLAVE_RESERVED_03), INTERRUPT_POINTER(k_irq_slave_03),
			KERNEL_PROTECTEDMODE_CODE_SEG, IDT_GATE_DPL_0);
	k_idt_set_int_gate(IRQ_MAKEINT_SLAVE(IRQ_SLAVE_RESERVED_04), INTERRUPT_POINTER(k_irq_slave_04),
			KERNEL_PROTECTEDMODE_CODE_SEG, IDT_GATE_DPL_0);
	k_idt_set_int_gate(IRQ_MAKEINT_SLAVE(IRQ_SLAVE_COPROC), INTERRUPT_POINTER(k_irq_slave_05),
			KERNEL_PROTECTEDMODE_CODE_SEG, IDT_GATE_DPL_0);
	k_idt_set_int_gate(IRQ_MAKEINT_SLAVE(IRQ_SLAVE_HDC), INTERRUPT_POINTER(k_irq_slave_06),
			KERNEL_PROTECTEDMODE_CODE_SEG, IDT_GATE_DPL_0);
	k_idt_set_int_gate(IRQ_MAKEINT_SLAVE(IRQ_SLAVE_RESERVED_07), INTERRUPT_POINTER(k_irq_slave_07),
			KERNEL_PROTECTEDMODE_CODE_SEG, IDT_GATE_DPL_0);

	k_load_idt_descriptor();

	return true;
}


void k_handle_exception(uint32_t except, int32_t code, uint32_t addr)
{
	regs_t *regs = (regs_t *)(&addr + 1);
	/* handle interrupt code */
	if(except != EXC_BREAKPOINT)
		k_panic4(CPU_EXCEPTION, except, k_exception_descr[except], regs, addr);
}

void k_handle_irq(uint32_t irq)
{
	/* handle interrupt code */
	k_print("k_handle_irq line %d\n", irq);
	if(irq == 1)
	{
		k_io_port_inb(0x60);
	}

	k_pic_eoi((byte)irq);
}

bool k_interrupts_init()
{
	/* disable external interrupts */
	k_iasync_disable();

	/* disable local APIC if presence */
	if(k_get_cpuinfo()->apicPresence)
	{
		if(k_get_cpuinfo()->msrSupported)
			k_apic_disable();
	}

	/* set idt routines */
	if(!k_idt_init())
		return false;

	/* init interrupt controler (i8259) */
	if(!k_pic_init())
		return false;

	/* enable external interrupts */
	k_iasync_enable();

	return true;
}





