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

#ifndef __CONSOLE_H
#define __CONSOLE_H

/* is this not a kernel includes, add for easy */
#include <kernel.h>

#define TAB_WHITE_SPACES	4

#define CONCOLE_SYMBOL_COLOR_BLACK			0x0
#define CONCOLE_SYMBOL_COLOR_BLUE			0x1
#define CONCOLE_SYMBOL_COLOR_GREEN			0x2
#define CONCOLE_SYMBOL_COLOR_CYAN			0x3
#define CONCOLE_SYMBOL_COLOR_RED			0x4
#define CONCOLE_SYMBOL_COLOR_MAGENTA		0x5
#define CONCOLE_SYMBOL_COLOR_BROWN			0x6
#define CONCOLE_SYMBOL_COLOR_LIGHTGREY		0x7
#define CONCOLE_SYMBOL_COLOR_DARKGREY		0x8
#define CONCOLE_SYMBOL_COLOR_LIGHTBLUE		0x9
#define CONCOLE_SYMBOL_COLOR_LIGHTCYAN		0xA
#define CONCOLE_SYMBOL_COLOR_LIGHTRED		0xB
#define CONCOLE_SYMBOL_COLOR_LIGHTMAGENTA	0xC
#define CONCOLE_SYMBOL_COLOR_LIGHTBROWN		0xD
#define CONCOLE_SYMBOL_COLOR_WHITE			0xE

#define CONCOLE_BACKGROUND_BLACK			0x0
#define CONCOLE_BACKGROUND_BLUE				0x1
#define CONCOLE_BACKGROUND_GREEN			0x2
#define CONCOLE_BACKGROUND_CYAN				0x3
#define CONCOLE_BACKGROUND_RED				0x4
#define CONCOLE_BACKGROUND_MAGENTA			0x5
#define CONCOLE_BACKGROUND_BROWN			0x6
#define CONCOLE_BACKGROUND_LIGHTGREY		0x7

typedef struct 
{
	ptr_t memEntry;
	size_t xLen;
	size_t yLen;
} consoleEntry_t;

bool k_console_init(const consoleEntry_t *entry);

bool k_console_clean();

int k_console_set_cursor_pos(size_t x, size_t y);

void k_console_get_cursor_pos(size_t *x, size_t *y);

int k_console_seek(size_t pos);

size_t k_console_get_pos();

int k_console_putc(char c);

void k_console_clean_front();

size_t k_console_write(char *message);

void k_console_set_color(char backCol, char symCol);

#endif
