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
#include <fs/file.h>
#include <fs/vfs.h>
#include <console.h>

uint32_t	STDOUT;
uint32_t	STDERR;

static void* std_out_err_open(const char *path, uint32_t mode, void *file)
{
	/* write only */
	if(!(mode & FILE_OUT))
		return NULL;
	
	return file;
}

static bool std_out_err_close(void *file)
{
	return true;
}

static long std_out_err_write(const ptr_t buf, size_t size, size_t count, void *file)
{
	size_t fullSize = size * count, i = 0;
	for(; i < fullSize; i++)
		k_console_putc(((char *)buf)[i]);
	
	return fullSize;
}

bool k_stdout_init()
{
	node_t stdoutNode = {
		"stdout",
		std_out_err_open, 
		std_out_err_close, 
		NULL,
		std_out_err_write,
		NULL,
		NULL
	};
	
	node_t stderrNode = {
		"stderr",
		std_out_err_open, 
		std_out_err_close, 
		NULL,
		std_out_err_write,
		NULL,
		NULL
	};
		
	k_vfs_mkdir("/", "dev");
	if(!k_vfs_mknode("/dev/", &stdoutNode, 0x0))
		return false;
	if(!k_vfs_mknode("/dev/", &stderrNode, 0x0))
		return false;
	
	STDOUT = k_fopen("/dev/stdout", FILE_OPEN_IN_VFS | FILE_OUT);
	STDERR = k_fopen("/dev/stderr", FILE_OPEN_IN_VFS | FILE_OUT);
	
	return true;
}