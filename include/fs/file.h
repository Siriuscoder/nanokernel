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

#ifndef FILE_H_
#define FILE_H_

#include <types.h>
#include <utils/list.h>

#define FILE_NAME_LEN		50
#define FILE_FULL_PATH		255
#define FD_ALLOCATE_BATCH	100

/* file flags */
#define FILE_IS_FOLDER		(1)

/* file open flags */
#define FILE_OPEN_IN_VFS	(1)

typedef void  *(*file_open_t)(const char *path, uint32_t mode, void *file);
typedef bool   (*file_close_t)(void *file);
typedef long (*file_read_t)(ptr_t buf, size_t size, size_t count, void *file);
typedef long (*file_write_t)(const ptr_t buf, size_t size, size_t count, void *file);
typedef long (*file_seek_t)(uint32_t offset, uint32_t whence, void *file);
typedef void (*file_flush_t)(void *file);

typedef struct
{
	char name[FILE_NAME_LEN];
	char fullPath[FILE_FULL_PATH];
	uint32_t flags;
	uint32_t fd;
	file_open_t open;
	file_close_t close;
	file_read_t read;
	file_write_t write;
	file_seek_t seek;
	file_flush_t flush;
	void *internal;
	struct list_link l;
} file_t;

bool k_finit();

uint32_t k_fopen(const char *path, uint32_t mode);
bool k_fclose(uint32_t fd);
long k_fread(uint32_t fd, ptr_t buf, size_t size, size_t count);
long k_fwrite(uint32_t fd, const ptr_t buf, size_t size, size_t count);
long k_fseek(uint32_t fd, uint32_t offset, uint32_t whence);
void k_fflush(uint32_t fd);

#endif /* FILE_H_ */
