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

#ifndef CODEPAGE_H
#define	CODEPAGE_H

#include <keyboard.h>

#define  KEYBOARD_EXTENDED_STATE			0xE0
#define  KEYBOARD_RELEASED_STATE			0x80

static const unsigned char scan_code_map_1[][4] = {
	{0,      0 },    /* 0 */
	{KEY_ESC,	KEY_ESC, 0, 1 },  /* 1 - ESC */
	{'1',	'!'},  /* 2 */
	{'2',	'@'},
	{'3',	'#'},
	{'4',	'$'},
	{'5',	'%'},
	{'6',	'^'},
	{'7',	'&'},
	{'8',	'*'},
	{'9',	'('},
	{'0',	')'},
	{'-',	'_'},
	{'=',	'+'},
	{KEY_BACKSPACE,	KEY_BACKSPACE, KEY_BACKSPACE, 1},  /* 14 - Backspace */
	{KEY_TAB, KEY_TAB, KEY_TAB, 1 },  /* 15 TAB*/
	{'q',	'Q'},
	{'w',	'W'},
	{'e',	'E'},
	{'r',	'R'},
	{'t',	'T'},
	{'y',	'Y'},
	{'u',	'U'},
	{'i',	'I'},
	{'o',	'O'},
	{'p',	'P'},
	{'[',	'{'},
	{']','}'},     /* 27 */
	{KEY_ENTER, KEY_ENTER, KEY_ENTER, 1},   /* 28 - Enter */
	{KEY_LEFT_CTRL,	KEY_LEFT_CTRL, KEY_LEFT_CTRL, 1 },  /* 29 - Ctrl */
	{'a',	'A'},  /* 30 */
	{'s',	'S'},
	{'d',	'D'},
	{'f',	'F'},
	{'g',	'G'},
	{'h',	'H'},
	{'j',	'J'},
	{'k',	'K'},
	{'l',	'L'},
	{';',	':'},
	{'\'',  '"'},  /* 40 */
	{'`',	'~'},  /* 41 */
	{KEY_LEFT_SHIFT, KEY_LEFT_SHIFT, KEY_LEFT_SHIFT, 1 },  /* 42 - Left Shift */
	{'\\',  '|'},  /* 43 */
	{'z',	'Z'},  /* 44 */
	{'x',	'X'},
	{'c',	'C'},
	{'v',	'V'},
	{'b',	'B'},
	{'n',	'N'},
	{'m',	'M'},
	{',',	'<'},
	{'.',	'>'},
	{'/',   '?'},  /* 53 */
	{KEY_RIGHT_SHIFT, KEY_RIGHT_SHIFT, KEY_RIGHT_SHIFT, 1 },  /* 54 - Right Shift */
	{0,		0},   /* 55 - Print Screen */
	{KEY_ALT, KEY_ALT, KEY_ALT, 1 },   /* 56 - Alt */
	{KEY_SPACE,  KEY_SPACE, KEY_SPACE, 1},   /* 57 - Space bar */
	{KEY_CAPS_LOCK, KEY_CAPS_LOCK, KEY_CAPS_LOCK, 1 },   /* 58 - Caps Lock */
	{KEY_F1, KEY_F1, KEY_F1, 1 },   /* 59 - F1 */
	{KEY_F2, KEY_F2, KEY_F2, 1 },   /* 60 - F2 */
	{KEY_F3, KEY_F3, KEY_F3, 1 },   /* 61 - F3 */
	{KEY_F4, KEY_F4, KEY_F4, 1 },   /* 62 - F4 */
	{KEY_F5, KEY_F5, KEY_F5, 1 },   /* 63 - F5 */
	{KEY_F6, KEY_F6, KEY_F6, 1 },   /* 64 - F6 */
	{KEY_F7, KEY_F7, KEY_F7, 1 },   /* 65 - F7 */
	{KEY_F8, KEY_F8, KEY_F8, 1 },   /* 66 - F8 */
	{KEY_F9, KEY_F9, KEY_F9, 1 },   /* 67 - F9 */
	{KEY_F10, KEY_F10, KEY_F10, 1 },   /* 68 - F10 */
	{KEY_NUM_LOCK, KEY_NUM_LOCK, KEY_NUM_LOCK, 1 },   /* 69 - Num Lock */
	{KEY_SCROLL_LOCK, KEY_SCROLL_LOCK, KEY_SCROLL_LOCK, 1 },   /* 70 - Scroll Lock */
	/* extended keyboard state */
	{'7',	'7', KEY_HOME},   /* 71 - Numeric keypad 7, Home */
	{'8',	'8', KEY_UP_ARROW},   /* 72 - Numeric keypad 8, Up Arrow */
	{'9',	'9', KEY_PAGE_UP},   /* 73 - Numeric keypad 9, Page up */
	{'-',	'-'},   /* 74 - Numeric keypad '-' */
	{'4',	'4', KEY_LEFT_ARROW},   /* 75 - Numeric keypad 4, Left Arrow*/
	{'5',	'5'},   /* 76 - Numeric keypad 5 */
	{'6',	'6', KEY_RIGHT_ARROW},   /* 77 - Numeric keypad 6, Right Arrow*/
	{'+',	'+'},   /* 78 - Numeric keypad '+' */
	{'1',	'1', KEY_END},   /* 79 - Numeric keypad 1, End*/
	{'2',	'2', KEY_DOWN_ARROW},   /* 80 - Numeric keypad 2, Down Arrow */
	{'3',	'3', KEY_PAGE_DOWN},   /* 81 - Numeric keypad 3, Page Down*/
	{'0',	'0', KEY_INSERT},   /* 82 - Numeric keypad 0, Insert*/
	{'.',	'.', KEY_DELETE}   /* 83 - Numeric keypad '.', Delete */
};

#endif	/* CODEPAGE_H */

