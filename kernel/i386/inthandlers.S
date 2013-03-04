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

	.file "inthandlers.S"

#include "int.h"

	.globl k_idt;
	.globl k_load_idt_descriptor;
	.globl k_DE_handler;
	.globl k_NMI_handler;
	.globl k_BP_handler;
	.globl k_OF_handler;
	.globl k_BR_handler;
	.globl k_UD_handler;
	.globl k_NM_handler;
	.globl k_DF_handler;
	.globl k_TS_handler;
	.globl k_NP_handler;
	.globl k_SS_handler;
	.globl k_GP_handler;
	.globl k_PF_handler;
	.globl k_MF_handler;
	.globl k_AC_handler;
	.globl k_MC_handler;
	.globl k_XM_handler;
	.globl k_stub_handler;

	.globl k_irq_master_00;
	.globl k_irq_master_01;
	.globl k_irq_master_02;
	.globl k_irq_master_03;
	.globl k_irq_master_04;
	.globl k_irq_master_05;
	.globl k_irq_master_06;
	.globl k_irq_master_07;

	.globl k_irq_slave_00;
	.globl k_irq_slave_01;
	.globl k_irq_slave_02;
	.globl k_irq_slave_03;
	.globl k_irq_slave_04;
	.globl k_irq_slave_05;
	.globl k_irq_slave_06;
	.globl k_irq_slave_07;




	.data
/* force 8-byte alignment */
	.p2align	3

k_idt:
	.space		(8 * IDT_COUNT_MAX)

/* force 4-byte alignment */
	.p2align	2

k_idt_descriptor:
	.word	((IDT_COUNT_MAX*8)-1)	/* limit */
	.long	k_idt					/* addr */


	.text
	.code32

k_load_idt_descriptor:
	movw	$KERNEL_REALMODE_DATA_SEG, %ax
	movw	%ax, %ds
	lidt	k_idt_descriptor
	ret

/* Intel exceptions handlers
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
	17 			#AC 		Alignment Check 				Fault 		Yes(Zero)   Any data reference in memory
	18 			#MC 		Machine Check 					Abort 		No 			Error codes (if any) and source are model dependent.
	19 			#XM 		SIMD Floating-Point Exception 	Fault 		No 			SSE/SSE2/SSE3 floating-point instructions
	20-31 			— 		Intel reserved. Do not use.
	32-255			— 		User Defined (Nonreserved)
 *
 *

	If the handler procedure is going to be executed at a numerically lower privilege
	level, a stack switch occurs. When the stack switch occurs:

		a. The segment selector and stack pointer for the stack to be used by the
	handler are obtained from the TSS for the currently executing task. On this
	new stack, the processor pushes the stack segment selector and stack
	pointer of the interrupted procedure.

		b. The processor then saves the current state of the EFLAGS, CS, and EIP
	registers on the new stack (see Figures 6-4).

		c. If an exception causes an error code to be saved, it is pushed on the new
	stack after the EIP value.

	If the handler procedure is going to be executed at the same privilege level as the
	interrupted procedure:

		a. The processor saves the current state of the EFLAGS, CS, and EIP registers
	on the current stack (see Figures 6-4).

		b. If an exception causes an error code to be saved, it is pushed on the current
	stack after the EIP value.

	Figure 6-4. Stack Usage on Transfers to Interrupt and Exception-Handling Routines

	Stack Usage with No
	Privilege-Level Change

	|------|
	|EFLAGS|
	|CS----|
	|EIP---|
	|ErrorC|

	Stack Usage with
	Privilege-Level Change
	|------|
	|SS----|
	|ESP---|
	|EFLAGS|
	|CS----|
	|EIP---|
	|ErrorC|

*/
k_switch_to_handler:
	/* push all regs */
	pusha
	pushw	%ds
	pushw	%es
	pushw	%fs
	pushw	%gs

	/* get int handler call method */
	movl	-8(%ebp), %eax
	cmpl	$INT_IRQ, %eax
	je		call_irq
	cmpl	$INT_WITH_PARAM, %eax
	je		with_code

	/* no code variant case */
	/* save eip */
	pushl	4(%ebp)
	/* save exc no */
	pushl	-4(%ebp)
	call 	k_handle_exception_no_code
	/* restore stack */
	addl	$0x8, %esp
	jmp 	fini_int

call_irq:
	/* IRQ variant case */
	/* save eip */
	pushl	4(%ebp)
	/* save exc no */
	pushl	-4(%ebp)
	call 	k_handle_irq
	/* restore stack */
	addl	$0x8, %esp
	jmp 	fini_int

with_code:
	/* with code variant case */
	/* save eip */
	pushl	8(%ebp)
	/* save err code in ax */
	pushl	4(%ebp)
	/* save exc no */
	pushl	-4(%ebp)
	call 	k_handle_exception_with_code
	/* restore stack */
	addl	$0xc, %esp

fini_int:
	popw	%gs
	popw	%fs
	popw	%es
	popw	%ds
	popa
	addl	$0x8, %esp
	leave

	iret

k_stub_handler:
	iret

k_DE_handler:
	/* no err code */
	pushl	%ebp
	movl	%esp, %ebp
	pushl	$EXC_DIVIDE_ERROR
	pushl	$INT_NO_PARAM
	jmp 	k_switch_to_handler

k_NMI_handler:
	/* no err code */
	pushl	%ebp
	movl	%esp, %ebp
	pushl	$EXC_NMI
	pushl	$INT_NO_PARAM
	jmp 	k_switch_to_handler

k_BP_handler:
	/* no err code */
	pushl	%ebp
	movl	%esp, %ebp
	pushl	$EXC_BREAKPOINT
	pushl	$INT_NO_PARAM
	jmp 	k_switch_to_handler

k_OF_handler:
	/* no err code */
	pushl	%ebp
	movl	%esp, %ebp
	pushl	$EXC_OVERFLOW
	pushl	$INT_NO_PARAM
	jmp 	k_switch_to_handler

k_BR_handler:
	/* no err code */
	pushl	%ebp
	movl	%esp, %ebp
	pushl	$EXC_BOUND_RANGE_EXCEEDED
	pushl	$INT_NO_PARAM
	jmp 	k_switch_to_handler

k_UD_handler:
	/* no err code */
	pushl	%ebp
	movl	%esp, %ebp
	pushl	$EXC_INVALID_OPCODE
	pushl	$INT_NO_PARAM
	jmp 	k_switch_to_handler

k_NM_handler:
	/* no err code */
	pushl	%ebp
	movl	%esp, %ebp
	pushl	$EXC_DEVICE_NOT_AVAILABLE
	pushl	$INT_NO_PARAM
	jmp 	k_switch_to_handler

k_DF_handler:
	/* with err code */
	pushl	%ebp
	movl	%esp, %ebp
	pushl	$EXC_DOUBLE_FAULT
	pushl	$INT_WITH_PARAM
	jmp 	k_switch_to_handler


k_TS_handler:
	/* with err code */
	pushl	%ebp
	movl	%esp, %ebp
	pushl	$EXC_INVALID_TSS
	pushl	$INT_WITH_PARAM
	jmp 	k_switch_to_handler

k_NP_handler:
	/* with err code */
	pushl	%ebp
	movl	%esp, %ebp
	pushl	$EXC_SEGMENT_NOT_PRESENT
	pushl	$INT_WITH_PARAM
	jmp 	k_switch_to_handler

k_SS_handler:
	/* with err code */
	pushl	%ebp
	movl	%esp, %ebp
	pushl	$EXC_STACK_SEGMENT_FAULT
	pushl	$INT_WITH_PARAM
	jmp 	k_switch_to_handler

k_GP_handler:
	/* with err code */
	pushl	%ebp
	movl	%esp, %ebp
	pushl	$EXC_GENERAL_PROTECTION
	pushl	$INT_WITH_PARAM
	jmp 	k_switch_to_handler

k_PF_handler:
	/* with err code */
	pushl	%ebp
	movl	%esp, %ebp
	pushl	$EXC_PAGE_FAULT
	pushl	$INT_WITH_PARAM
	jmp 	k_switch_to_handler

k_MF_handler:
	/* no err code */
	pushl	%ebp
	movl	%esp, %ebp
	pushl	$EXC_FPU_ERROR
	pushl	$INT_NO_PARAM
	jmp 	k_switch_to_handler

k_AC_handler:
	/* with err code */
	pushl	%ebp
	movl	%esp, %ebp
	pushl	$EXC_ALIGMENT_CHECK
	pushl	$INT_WITH_PARAM
	jmp 	k_switch_to_handler

k_MC_handler:
	/* no err code */
	pushl	%ebp
	movl	%esp, %ebp
	pushl	$EXC_MACHINE_CHECK
	pushl	$INT_NO_PARAM
	jmp 	k_switch_to_handler

k_XM_handler:
	/* no err code */
	pushl	%ebp
	movl	%esp, %ebp
	pushl	$EXC_FLOATING_POINT
	pushl	$INT_NO_PARAM
	jmp 	k_switch_to_handler

///////////////// IRQ /////////////////////////

k_irq_master_00:
	/* no err code */
	pushl	%ebp
	movl	%esp, %ebp
	pushl	$IRQ_MASTER_TIMER
	pushl	$INT_IRQ
	jmp 	k_switch_to_handler

k_irq_master_01:
	/* no err code */
	pushl	%ebp
	movl	%esp, %ebp
	pushl	$IRQ_MASTER_KEYBOARD
	pushl	$INT_IRQ
	jmp 	k_switch_to_handler

k_irq_master_02:
	/* no err code */
	pushl	%ebp
	movl	%esp, %ebp
	pushl	$IRQ_MASTER_CASCADE
	pushl	$INT_IRQ
	jmp 	k_switch_to_handler

k_irq_master_03:
	/* no err code */
	pushl	%ebp
	movl	%esp, %ebp
	pushl	$IRQ_MASTER_COM_2_4
	pushl	$INT_IRQ
	jmp 	k_switch_to_handler

k_irq_master_04:
	/* no err code */
	pushl	%ebp
	movl	%esp, %ebp
	pushl	$IRQ_MASTER_COM_1_3
	pushl	$INT_IRQ
	jmp 	k_switch_to_handler

k_irq_master_05:
	/* no err code */
	pushl	%ebp
	movl	%esp, %ebp
	pushl	$IRQ_MASTER_LPT2
	pushl	$INT_IRQ
	jmp 	k_switch_to_handler

k_irq_master_06:
	/* no err code */
	pushl	%ebp
	movl	%esp, %ebp
	pushl	$IRQ_MASTER_FDC
	pushl	$INT_IRQ
	jmp 	k_switch_to_handler

k_irq_master_07:
	/* no err code */
	pushl	%ebp
	movl	%esp, %ebp
	pushl	$IRQ_MASTER_LPT1
	pushl	$INT_IRQ
	jmp 	k_switch_to_handler

k_irq_slave_00:
	/* no err code */
	pushl	%ebp
	movl	%esp, %ebp
	pushl	$IRQ_SLAVE_RTC
	pushl	$INT_IRQ
	jmp 	k_switch_to_handler

k_irq_slave_01:
	/* no err code */
	pushl	%ebp
	movl	%esp, %ebp
	pushl	$IRQ_SLAVE_IRQ2_REDIRECT
	pushl	$INT_IRQ
	jmp 	k_switch_to_handler

k_irq_slave_02:
	/* no err code */
	pushl	%ebp
	movl	%esp, %ebp
	pushl	$IRQ_SLAVE_RESERVED_02
	pushl	$INT_IRQ
	jmp 	k_switch_to_handler

k_irq_slave_03:
	/* no err code */
	pushl	%ebp
	movl	%esp, %ebp
	pushl	$IRQ_SLAVE_RESERVED_03
	pushl	$INT_IRQ
	jmp 	k_switch_to_handler

k_irq_slave_04:
	/* no err code */
	pushl	%ebp
	movl	%esp, %ebp
	pushl	$IRQ_SLAVE_RESERVED_04
	pushl	$INT_IRQ
	jmp 	k_switch_to_handler

k_irq_slave_05:
	/* no err code */
	pushl	%ebp
	movl	%esp, %ebp
	pushl	$IRQ_SLAVE_COPROC
	pushl	$INT_IRQ
	jmp 	k_switch_to_handler

k_irq_slave_06:
	/* no err code */
	pushl	%ebp
	movl	%esp, %ebp
	pushl	$IRQ_SLAVE_HDC
	pushl	$INT_IRQ
	jmp 	k_switch_to_handler

k_irq_slave_07:
	/* no err code */
	pushl	%ebp
	movl	%esp, %ebp
	pushl	$IRQ_SLAVE_RESERVED_07
	pushl	$INT_IRQ
	jmp 	k_switch_to_handler
