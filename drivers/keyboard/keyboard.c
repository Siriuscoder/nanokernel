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
#include <int.h>
#include <pic.h>
#include <io.h>
#include <std/print.h>

static void keyboard_irq_handler(const intParams_t *params)
{
	k_print("k_handle_irq line %d\n", params->intNum);
	/* read scancode */
	k_io_port_inb(0x60);
}

static bool init_keyboard(size_t argc, char **args)
{
	k_iasync_disable();

	k_attach_interrupt_handler(IRQ_MAKEINT_MASTER(IRQ_MASTER_KEYBOARD), keyboard_irq_handler);
	k_pic_enable_irq_line(IRQ_MASTER_KEYBOARD);
	k_iasync_enable();

	return true;
}

DECLARE_DRIVER(keyboardDrv, "keyboard", "siriusmicro", init_keyboard, NULL);


