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

#ifndef KEYBOARD_H
#define	KEYBOARD_H

#include <types.h>

#define KEY_PESSED			1
#define KEY_RELEASED		0

#define KEY_ESC				1
#define KEY_BACKSPACE		2
#define KEY_LEFT_CTRL		3
#define KEY_RIGHT_CTRL		4
#define KEY_LEFT_SHIFT		5
#define KEY_RIGHT_SHIFT		6
#define KEY_ALT				7
#define KEY_F1				8
#define KEY_F2				9
#define KEY_F3				10
#define KEY_F4				11
#define KEY_F5				12
#define KEY_F6				13
#define KEY_F7				14
#define KEY_F8				15
#define KEY_F9				16
#define KEY_F10				17
#define KEY_CAPS_LOCK		18
#define KEY_SCROLL_LOCK		19
#define KEY_NUM_LOCK		20
#define KEY_TAB				21
#define KEY_ENTER			22
#define KEY_SPACE			23
#define KEY_HOME			24
#define KEY_PAGE_UP			25
#define KEY_PAGE_DOWN		26
#define KEY_END				27
#define KEY_UP_ARROW		28
#define KEY_DOWN_ARROW		29
#define KEY_LEFT_ARROW		30
#define KEY_RIGHT_ARROW		31
#define KEY_INSERT			32
#define KEY_DELETE			33

#define KEY_MAX				50

/* LEDs keys */
#define LED_NUMLOCK_ENABLE					0x02
#define LED_CAPSLOCK_ENABLE					0x04
#define LED_SCROLLLOCK_ENABLE				0x01

typedef void
(*LED_controll_t)(uint8_t flag);

void k_set_keyboard_state_key(uint32_t key, uint8_t state);
uint8_t k_get_keyboard_state_key(uint32_t key);
void k_init_keyboard(void);
void k_wait_keyboard(void);
void k_notify_keyboard(void);
void k_set_keyboard_LED_controll(LED_controll_t func);
void k_keyboard_LED_controll(uint8_t flag);

#endif	/* KEYBOARD_H */

