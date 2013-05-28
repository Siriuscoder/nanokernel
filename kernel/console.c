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

#include <console.h>
#include <std/membase.h>

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
	char curAttrib;
	consoleEntity_t *firstEntity;
	consoleEntry_t entry;
} consIntern;

static void k_clear_line(uint32_t lineno)
{
	int i = 0;
	consoleEntity_t *p = consIntern.firstEntity +
		(lineno * consIntern.entry.xLen);

	for(; i < consIntern.entry.xLen; i++, p++)
	{
		p->symbol = ' ';
		p->attrib = WHITE_ON_DARK_ATTR;
	}
}


static size_t k_console_newline()
{
	size_t curLine;

	curLine = consIntern.pos / consIntern.entry.xLen;
	/* if it is last line in console buffer shift to up */
	if(curLine+1 >= consIntern.entry.yLen)
	{
		/* copy from second line to begin */
		k_memcpy(consIntern.entry.memEntry,
			consIntern.entry.memEntry + consIntern.lineSize,
			consIntern.buffSize - consIntern.lineSize);
		/* clear current line */
		k_clear_line(curLine);
	}
	else
	{
		curLine++;
	}

	consIntern.pos = consIntern.entry.xLen * curLine;
	return consIntern.pos;
}

static void k_set_symbol_in_current_pos(char symbol, char attr)
{
	consoleEntity_t *entity = (consIntern.firstEntity + consIntern.pos);
	entity->symbol = symbol;
	entity->attrib = attr;
}

bool k_console_init(const consoleEntry_t *entry)
{
	k_memcpy(&consIntern.entry, (ptr_t)entry, sizeof(consoleEntry_t));
	consIntern.pos = 0;
	consIntern.lineSize = entry->xLen * sizeof(consoleEntity_t);
	consIntern.buffSize = consIntern.lineSize * entry->yLen;
	consIntern.consoleMaxSize = entry->xLen * entry->yLen;
	consIntern.firstEntity = (consoleEntity_t *)consIntern.entry.memEntry;
	consIntern.curAttrib = WHITE_ON_DARK_ATTR;

	return true;
}

bool k_console_clean()
{
	int i = 0;

	for(; i < consIntern.entry.yLen; i++)
		k_clear_line(i);

	return true;
}

size_t k_console_write(char *message)
{
	char *base = message;
	while ((*message) != 0)
	{
		k_console_putc(*message);
		message++;
	}

	return (size_t)(message - base);
}

int k_console_putc(char c)
{
    /* clean up cursor */
	k_set_symbol_in_current_pos(0, WHITE_ON_DARK_ATTR);
	
	if(c == '\n')
	{
		k_console_newline();
		return 1;
	}

	if(c == '\t')
	{
		int tabCounter = 0;
		while(tabCounter < TAB_WHITE_SPACES)
		{
			k_console_putc(' ');
			tabCounter++;
		}
		
		return 1;
	}

	k_set_symbol_in_current_pos(c, consIntern.curAttrib);

	consIntern.pos++;
	if(consIntern.pos+1 >= consIntern.consoleMaxSize)
		k_console_newline();

	/* paint my cursor */
	k_set_symbol_in_current_pos('_', WHITE_ON_DARK_ATTR);

	return 1;
}

int k_console_set_cursor_pos(size_t x, size_t y)
{
	if(x > consIntern.entry.xLen ||
		y > consIntern.entry.yLen)
		return -1;

	consIntern.pos = (consIntern.entry.xLen * y) + consIntern.entry.xLen;
	return 0;
}

void k_console_get_cursor_pos(size_t *x, size_t *y)
{
	*y = consIntern.pos / consIntern.entry.xLen;
	*x = consIntern.pos % consIntern.entry.xLen;
}


int k_console_seek(size_t pos)
{
	if(pos >= consIntern.consoleMaxSize)
		return -1;

	consIntern.pos = pos;
	return 0;
}

size_t k_console_get_pos()
{
	return consIntern.pos;
}

void k_console_clean_front()
{
	if(consIntern.pos > 0)
	{
		k_set_symbol_in_current_pos(0, WHITE_ON_DARK_ATTR);
		consIntern.pos--;
		
		/* paint my cursor */
		k_set_symbol_in_current_pos('_', WHITE_ON_DARK_ATTR);
	}
}

void k_console_set_color(char backCol, char symCol)
{
 /* set low 4 bits for symbol color */
	consIntern.curAttrib = (symCol & 0xf);
 /* set high 3 bits for background color, 4-ht bit always 0 */
	consIntern.curAttrib |= (backCol << 4);
}

