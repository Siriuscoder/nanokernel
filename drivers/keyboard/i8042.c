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

#include <driver.h>
#include <codepage.h>
#include <stdin.h>
#include <int.h>
#include <pic.h>
#include <io.h>
#include <std/print.h>
#include <keyboard.h>


#define  I8042_MODE_XLATE					0x40
#define  I8042_MODE_FPORT_DISABLE			0x10
#define  I8042_MODE_SPORT_DISABLE			0x20
#define  I8042_FPORT_IRQ_ENABLE				0x01
#define  I8042_SPORT_IRQ_ENABLE				0x02

#define  I8042_CMD_PORT			0x64
#define	 I8042_STATUS_PORT		0x64
#define  I8042_DATA_PORT		0x60

/*
 * Status Register
 * The Status Register contains various flags that indicate the state of the PS/2 
 * controller. The meanings for each bit are:
 * Bit	 Meaning
 * 0	 Output buffer status (0 = empty, 1 = full)
 * must be set before attempting to read data from IO port 0x60)
 * 1	 Input buffer status (0 = empty, 1 = full)
 * (must be clear before attempting to write data to IO port 0x60 or IO port 0x64)
 * 2	 System Flag
 * Meant to be cleared on reset and set by firmware (via. PS/2 Controller Configuration Byte) if the system passes self tests (POST)
 * 3	 Command/data (0 = data written to input buffer is data for PS/2 device, 1 = data written to input buffer is data for PS/2 controller command)
 * 4	 Unknown (chipset specific)
 * May be "keyboard lock" (more likely unused on modern systems)
 * 5	 Unknown (chipset specific)
 * May be "receive time-out" or "second PS/2 port output buffer full"
 * 6	 Time-out error (0 = no error, 1 = time-out error)
 * 7	 Parity error (0 = no error, 1 = parity error)
 */
#define STATUS_OUTPUT_BUFFER_EMPTY			0x00
#define STATUS_OUTPUT_BUFFER_FILL			0x01
#define STATUS_INPUT_BUFFER_EMPTY			0x00
#define STATUS_INPUT_BUFFER_FILL			0x02
/* Controller Configuration Byte read command */
#define COMMAND_READ_CONFIG_BYTE			0x20
/* Controller Configuration Byte write command */
#define COMMAND_WRITE_CONFIG_BYTE			0x60
/* Test PS/2 Controller */
#define COMMAND_SELF_TEST					0xAA
/* Test results */
#define I8042_TEST_PASSED					0x55
#define I8042_TEST_FAILED					0xFC
/* enabling devices */
#define COMMAND_FPORT_ENABLE				0xAE
#define COMMAND_SPORT_ENABLE				0xA8

#define LED_SET_COMMAND						0xED

static bool extendedKeyboardFlag = false;

static void I8042_wait_read(void) 
{
	while(0 == (k_io_port_inb(I8042_STATUS_PORT) & STATUS_OUTPUT_BUFFER_FILL))
		k_io_wait();
}

static void I8042_wait_write(void) 
{
	while(0 != (k_io_port_inb(I8042_STATUS_PORT) & STATUS_INPUT_BUFFER_FILL))
		k_io_wait();
}

static void flush_keyboard_buffer(void)
{
	/* while out buffer not empty */
	while(k_io_port_inb(I8042_STATUS_PORT) & STATUS_OUTPUT_BUFFER_FILL)
	{
		/* read data */
		k_io_port_inb(I8042_DATA_PORT);
	}
}

static void I8042_send_command(uint8_t command)
{
	I8042_wait_write();
	k_io_port_outb(I8042_CMD_PORT, command);
}

static void keyboard_send_command(uint8_t command)
{
	I8042_wait_write();
	k_io_port_outb(I8042_DATA_PORT, command);
}

static uint8_t I8042_send_command_and_read(uint8_t command)
{
	I8042_send_command(command);
	I8042_wait_read();
	return k_io_port_inb(I8042_DATA_PORT);
}

static void I8042_send_command_and_write(uint8_t command, uint8_t byte)
{
	I8042_send_command(command);
	I8042_wait_write();
	k_io_port_outb(I8042_DATA_PORT, byte);
}

static void keyboard_led_ctrl(uint8_t ledByte)
{
	keyboard_send_command(LED_SET_COMMAND);
	I8042_wait_write();
	k_io_port_outb(I8042_DATA_PORT, ledByte);
}

static bool init_I8042_controller(void)
{
	uint8_t resByte;
	/* If 0x64 returns 0xff, then we have no keyboard controller */
	if (k_io_port_inb(I8042_STATUS_PORT) == 0xFF)
		return false;
	
	/* step 1: flush output buffer */
	flush_keyboard_buffer();
	
	/* step 2: read Controller Configuration Byte */
	resByte = I8042_send_command_and_read(COMMAND_READ_CONFIG_BYTE);
	/* step 3: configure controller */
	/* Turn on scancode translate mode so that we can
	 use the scancode set 1 tables */
	resByte |= I8042_MODE_XLATE;
	resByte |= I8042_FPORT_IRQ_ENABLE;
	resByte |= I8042_SPORT_IRQ_ENABLE;
	resByte |= I8042_MODE_FPORT_DISABLE;
	resByte |= I8042_MODE_SPORT_DISABLE;
	/* now write config byte */
	I8042_send_command_and_write(COMMAND_WRITE_CONFIG_BYTE, resByte);
	
	/* step 4: testing controller */
	if(I8042_send_command_and_read(COMMAND_SELF_TEST) != I8042_TEST_PASSED)
		return false;
	
	/* step 5: configure again */
	I8042_send_command_and_write(COMMAND_WRITE_CONFIG_BYTE, resByte);
	
	/* step 6: enabling ports */
	I8042_send_command(COMMAND_FPORT_ENABLE);
	I8042_send_command(COMMAND_SPORT_ENABLE);
	/* OK! init complete! */
	
	/* Lets play with LED =) */
	/* enable all - its mead what keyboard init OK! */

	keyboard_led_ctrl(
			LED_NUMLOCK_ENABLE | 
			LED_CAPSLOCK_ENABLE | 
			LED_SCROLLLOCK_ENABLE);
	
	return true;
}

static void enter_non_ascii_symbol(byte index)
{
	switch(scan_code_map_1[index][0])
	{
	case KEY_ENTER:
		enter_ascii_symbol('\n');
		break;
	case KEY_SPACE:
		enter_ascii_symbol(' ');
		break;
	case KEY_TAB:
		enter_ascii_symbol('\t');
		break;
	}
}

static void keyboard_irq_handler(const intParams_t *params)
{
	/* read scancode */
	byte scancode = k_io_port_inb(I8042_DATA_PORT);
	byte indexcode = scancode;
	
	if(scancode == KEYBOARD_EXTENDED_STATE)
	{
		extendedKeyboardFlag = true;
		return;
	}
	/* key released */
	if(scancode & KEYBOARD_RELEASED_STATE)
	{
		/* normalize scancode */
		indexcode = scancode & 0x7f;
	}
	
	if(indexcode >= (sizeof(scan_code_map_1)/4))
		return;
	/* non char buttons */
	if(extendedKeyboardFlag || scan_code_map_1[indexcode][3])
	{		
		k_set_keyboard_state_key(scan_code_map_1[indexcode]
			[extendedKeyboardFlag ? 2 : 0], 
			scancode & KEYBOARD_RELEASED_STATE ? KEY_RELEASED : KEY_PESSED);
		
		if(!(scancode & KEYBOARD_RELEASED_STATE))
		{
			enter_non_ascii_symbol(indexcode);
		}
		
		extendedKeyboardFlag = false;
	}
	/* char buttons */
	else if(!(scancode & KEYBOARD_RELEASED_STATE))
	{
		byte symbol = scan_code_map_1[indexcode][
				(k_get_keyboard_state_key(KEY_LEFT_SHIFT) == KEY_PESSED ||
				k_get_keyboard_state_key(KEY_RIGHT_SHIFT) == KEY_PESSED) ? 1 : 0];

		enter_ascii_symbol(symbol);
	}
	
	k_notify_keyboard();
}

static bool init_keyboard(size_t argc, char **args)
{
	/* init ps/2 keyboard/mouse controller */
	if(!init_I8042_controller())
		return false;
	init_stdin();
	
	k_set_keyboard_LED_controll(keyboard_led_ctrl);
	/* disable async interrupts */
	k_iasync_disable();
	/* register interrupt handler */
	k_attach_interrupt_handler(IRQ_MAKEINT_MASTER(IRQ_MASTER_KEYBOARD), keyboard_irq_handler);
	k_pic_enable_irq_line(IRQ_MASTER_KEYBOARD);
	/* enable async intrerrupts */
	k_iasync_enable();

	return true;
}

DECLARE_DRIVER(keyboardDrv, "keyboard", "siriusmicro", init_keyboard, NULL);


