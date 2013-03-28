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

#include <pic.h>
#include <io.h>
#include <int.h>

#define PIC1            0x20   /* IO base address for master PIC */
#define PIC2            0xA0   /* IO base address for slave PIC */
#define PIC1_COMMAND    PIC1
#define PIC1_DATA       (PIC1 + 1)
#define PIC2_COMMAND    PIC2
#define PIC2_DATA       (PIC2 + 1)

/* ICW1 (Initialization Control Word) */
#define ICW_TEMPLATE          0x10
#define LEVL_TRIGGER          0x08
#define EDGE_TRIGGER          0x00
#define ADDR_INTRVL4          0x04
#define ADDR_INTRVL8          0x00
#define SINGLE__MODE          0x02
#define CASCADE_MODE          0x00
#define ICW4__NEEDED          0x01
#define NO_ICW4_NEED          0x00

/* ICW3 */
#define SLAVE_ON_IR0          0x01
#define SLAVE_ON_IR1          0x02
#define SLAVE_ON_IR2          0x04
#define SLAVE_ON_IR3          0x08
#define SLAVE_ON_IR4          0x10
#define SLAVE_ON_IR5          0x20
#define SLAVE_ON_IR6          0x40
#define SLAVE_ON_IR7          0x80

#define I_AM_SLAVE_0          0x00
#define I_AM_SLAVE_1          0x01
#define I_AM_SLAVE_2          0x02
#define I_AM_SLAVE_3          0x03
#define I_AM_SLAVE_4          0x04
#define I_AM_SLAVE_5          0x05
#define I_AM_SLAVE_6          0x06
#define I_AM_SLAVE_7          0x07

/* ICW4 */
#define SNF_MODE_ENA          0x10
#define SNF_MODE_DIS          0x00
#define BUFFERD_MODE          0x08
#define NONBUFD_MODE          0x00
#define AUTO_EOI_MOD          0x02
#define NRML_EOI_MOD          0x00
#define I8086_EMM_MOD         0x01
#define SET_MCS_MODE          0x00

/* OCW1 (Operation Control Word) */
#define PICM_MASK             0xFF
#define PICS_MASK             0xFF

/* OCW2 */
#define NON_SPEC_EOI          0x20
#define SPECIFIC_EOI          0x60
#define ROT_NON_SPEC          0xa0
#define SET_ROT_AEOI          0x80
#define RSET_ROTAEOI          0x00
#define ROT_SPEC_EOI          0xe0
#define SET_PRIORITY          0xc0
#define NO_OPERATION          0x40
/* OCW2 - EOI */
#define SEND_EOI_IR0          0x00
#define SEND_EOI_IR1          0x01
#define SEND_EOI_IR2          0x02
#define SEND_EOI_IR3          0x03
#define SEND_EOI_IR4          0x04
#define SEND_EOI_IR5          0x05
#define SEND_EOI_IR6          0x06
#define SEND_EOI_IR7          0x07

/* OCW3 */
#define OCW_TEMPLATE          0x08
#define SPECIAL_MASK          0x40
#define MASK_MDE_SET          0x20
#define MASK_MDE_RST          0x00
#define POLL_COMMAND          0x04
#define NO_POLL_CMND          0x00
#define READ_NEXT_RD          0x02
#define READ_IR_ONRD          0x00
#define READ_IS_ONRD          0x01
#define READ_IRR	      0x0a    /* OCW3 irq ready next CMD read */
#define READ_ISR              0x0b    /* OCW3 irq service next CMD read */

/**
 * Standard PIC initialization values
 */
#define PIC1_ICW1       (ICW_TEMPLATE | EDGE_TRIGGER | ADDR_INTRVL8 | CASCADE_MODE | ICW4__NEEDED)
#define PIC1_ICW3       (SLAVE_ON_IR2)
#define PIC1_ICW4       (SNF_MODE_DIS | NONBUFD_MODE | NRML_EOI_MOD | I8086_EMM_MOD)

#define PIC2_ICW1       (ICW_TEMPLATE | EDGE_TRIGGER | ADDR_INTRVL8 | CASCADE_MODE | ICW4__NEEDED)
#define PIC2_ICW3       (I_AM_SLAVE_2)
#define PIC2_ICW4       (SNF_MODE_DIS | NONBUFD_MODE | NRML_EOI_MOD | I8086_EMM_MOD)

static void pic_remap(uint16_t masterOffset, uint16_t slaveOffset)
{
/*
	When you enter protected mode (or even before hand, if you're not using GRUB) the first 
	command you will need to give the two PICs is the initialise command (code 0x11). This
	command makes the PIC wait for 3 extra "initialisation words" on the data port. 
	These bytes give the PIC:
	 - Its vector offset. (ICW2)
	 - Tell it how it is wired to master/slaves. (ICW3)
	 - Gives additional information about the environment. (ICW4)
*/
	/* Initialize the master */
	k_io_port_outb(PIC1_COMMAND, PIC1_ICW1);
	k_io_wait();
	k_io_port_outb(PIC1_DATA, masterOffset);
	k_io_wait();
	k_io_port_outb(PIC1_DATA, PIC1_ICW3);
	k_io_wait();
	k_io_port_outb(PIC1_DATA, PIC1_ICW4);
	k_io_wait();

	/* Initialize the slave */
	k_io_port_outb(PIC2_COMMAND, PIC2_ICW1);
	k_io_wait();
	k_io_port_outb(PIC2_DATA, slaveOffset);
	k_io_wait();
	k_io_port_outb(PIC2_DATA, PIC2_ICW3);
	k_io_wait();
	k_io_port_outb(PIC2_DATA, PIC2_ICW4);
	k_io_wait();

	k_io_port_outb(PIC1_COMMAND, NON_SPEC_EOI);
	k_io_port_outb(PIC2_COMMAND, NON_SPEC_EOI);
}
	
/* Helper func */
static uint16_t pic_get_irq_reg(int32_t ocw3)
{
	uint8_t hr, lr;
    	/* OCW3 to PIC CMD to get the register values.  PIC2 is chained, and
     	* represents IRQs 8-15.  PIC1 is IRQs 0-7, with 2 being the chain */
	k_io_port_outb(PIC1_COMMAND, ocw3);
	k_io_port_outb(PIC2_COMMAND, ocw3);

	lr = k_io_port_inb(PIC1_COMMAND);
	hr = k_io_port_inb(PIC2_COMMAND);
    	return (hr << 8) | lr;
}

void k_pic_disable()
{
	k_io_port_outb(PIC1_DATA, PICM_MASK);
	k_io_port_outb(PIC2_DATA, PICS_MASK);
}

void k_apic_disable()
{
	uint32_t hw, lw;
	k_get_msr(IA32_APIC_BASE_MSR, &hw, &lw);

	hw = 0;
	lw &= IA32_APIC_BASE_MSR_DISABLE;
	k_set_msr(IA32_APIC_BASE_MSR, hw, lw);
}

uint16_t k_pic_get_irq_mask()
{
	byte master;
	byte slave;

	master = k_io_port_inb(PIC1_DATA);
	slave = k_io_port_inb(PIC2_DATA);

	return (slave << 8) | master;
}

void k_pic_set_irq_mask(uint16_t mask)
{
	byte master = mask & 0xff;
	byte slave = (mask >> 8) & 0xff;

	k_io_port_outb(PIC1_DATA, master);
	k_io_port_outb(PIC2_DATA, slave);
}

bool k_pic_init()
{
	/* save old irq masks */
	//IRQ_master_mask = k_io_port_inb(PIC1_DATA);
	//IRQ_slave_mask = k_io_port_inb(PIC2_DATA);
	//k_iasync_disable();
	/* Normally, IRQs 0 to 7 are mapped to entries 8 to 15. This
	*  is a problem in protected mode, because IDT entry 8 is a
	*  Double Fault! Without remapping, every time IRQ0 fires,
	*  you get a Double Fault Exception, which is NOT actually
	*  what's happening. We send commands to the Programmable
	*  Interrupt Controller (PICs - also called the 8259's) in
	*  order to make IRQ0 to 15 be remapped to IDT entries 32 to
	*  47 */
	pic_remap(IRQ_MASTER_BASE, IRQ_SLAVE_BASE);

	/* mask all */
	k_pic_set_irq_mask(/*0xfffd*/0xffff);
	//pic_restore_mask();
	//k_iasync_enable();
	return true;
}

/* mask IRQ vectors */
void k_pic_disable_irq_line(byte irq)
{
    uint16_t port;
    uint8_t value;
 
    if(irq < 8)
        port = PIC1_DATA;
	else
	{
		port = PIC2_DATA;
        irq -= 8;
    }

	value = k_io_port_inb(port) | (1 << irq);
	k_io_port_outb(port, value);
}

void k_pic_enable_irq_line(byte irq)
{
    uint16_t port;
    uint8_t value;
 
    if(irq < 8)
        port = PIC1_DATA;
	else
	{
		port = PIC2_DATA;
        irq -= 8;
    }

	value = k_io_port_inb(port) & ~(1 << irq);
	k_io_port_outb(port, value);
}

/* Returns the combined value of the cascaded PICs irq request register */
uint16_t k_pic_get_irr(void)
{
	return pic_get_irq_reg(READ_IRR);
}
	
 
/* Returns the combined value of the cascaded PICs in-service register */
uint16_t k_pic_get_isr(void)
{
	return pic_get_irq_reg(READ_ISR);
}

void k_pic_eoi(byte irq)
{
	if(irq >= 8)
		k_io_port_outb(PIC2_COMMAND, NON_SPEC_EOI);
 
	k_io_port_outb(PIC1_COMMAND, NON_SPEC_EOI);
}


