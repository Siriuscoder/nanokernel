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

#include "console.h"

#define WHITE_ON_DARK_ATTR 0x07 // white on black text

typedef struct
{
	char symbol;
	char attrib;
} consoleEntity_t;

static struct 
{ 
	size_t pos;
	size_t buffSize;
	size_t lineSize;
	size_t consoleMaxSize;
	consoleEntity_t *firstEntity;
	consoleEntry_t entry;
} consIntern;

static size_t console_newline()
{
	size_t curLine;
	size_t newLine;

	curLine = consIntern.pos / consIntern.entry.xLen;
	/* if it is last line in console buffer shift to up */
	if((curLine+1) >= consIntern.entry.yLen)
	{
		newLine = curLine;
		/* copy from second line to begin */
		kmemcpy(consIntern.entry.memEntry,
			consIntern.entry.memEntry + consIntern.lineSize,
			consIntern.buffSize - consIntern.lineSize);
	}
	else
	{
		newLine = curLine+1;
	}

	consIntern.pos = consIntern.entry.xLen * newLine;
	return consIntern.pos;
}

int console_init(const consoleEntry_t *entry)
{
	kmemcpy(&consIntern.entry, (ptr_t)entry, sizeof(consoleEntry_t));
	consIntern.pos = 0;
	consIntern.lineSize = entry->xLen * sizeof(consoleEntity_t);
	consIntern.buffSize = consIntern.lineSize * entry->yLen;
	consIntern.consoleMaxSize = entry->xLen * entry->yLen;
	consIntern.firstEntity = (consoleEntity_t *)consIntern.entry.memEntry;

	return console_clean();
}

int console_clean()
{
	int i = 0;

	consoleEntity_t *p = consIntern.firstEntity;
	for(; i < consIntern.consoleMaxSize; ++i, ++p)
	{
		p->symbol = ' ';
		p->attrib = WHITE_ON_DARK_ATTR;
	}

	return 0;
}

size_t console_write(char *message)
{
	char *base = message;
	while ((*message) != 0)
	{
		console_putc(*message);
		message++;
	}

	return (size_t)(message - base);
}

int console_putc(char c)
{
	size_t newPos;
	if(c == '\n')
	{
		console_newline();
		return 1;
	}

	if(c == '\t')
	{
		int tabCounter = 0;
		while(tabCounter < TAB_WHITE_SPACES)
		{
			console_putc(' ');
			tabCounter++;
		}
	}

	(consIntern.firstEntity + consIntern.pos)->symbol = c;

	newPos = consIntern.pos+1;
	if(newPos >= consIntern.consoleMaxSize)
		newPos = console_newline();
	
	consIntern.pos = newPos;
	return 1;
}

int console_set_cursor_pos(size_t x, size_t y)
{
	if(x > consIntern.entry.xLen ||
		y > consIntern.entry.yLen)
		return -1;

	consIntern.pos = (consIntern.entry.xLen * y) + consIntern.entry.xLen;
	return 0;
}


int console_seek(size_t pos)
{
	if(pos >= consIntern.consoleMaxSize)
		return -1;

	consIntern.pos = pos;
	return 0;
}

