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

#include <keyboard.h>
#include <mem.h>
#include <cpu.h>
#include <std/membase.h>

static uint32_t *keyboardState;
static bool keyboardWait;
static LED_controll_t led_controll_func;


bool k_init_keyboard(void)
{
	keyboardState = k_malloc(sizeof(uint32_t) * KEY_MAX);
	k_memset(keyboardState, 0, sizeof(uint32_t) * KEY_MAX);
	
	return true;
}

void k_set_keyboard_state_key(uint32_t key, uint8_t state)
{
	keyboardState[key] = state;
}

uint8_t k_get_keyboard_state_key(uint32_t key)
{
	return key < KEY_MAX ? keyboardState[key] : 0;
}

void k_wait_keyboard(void)
{
	while(keyboardWait)
		k_idle_wait();
	
	keyboardWait = true;
}

void k_notify_keyboard(void)
{
	keyboardWait = false;
}

void k_set_keyboard_LED_controll(LED_controll_t func)
{
	led_controll_func = func;
}

void k_keyboard_LED_controll(uint8_t flag)
{
	led_controll_func(flag);
}

