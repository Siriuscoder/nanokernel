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
#include <std/strings.h>
#include <std/membase.h>
#include <mem.h>

typedef struct
{
	file_t **fdarray;
	uint32_t fdlen;
	uint32_t fdcounter;
} fdt_t;

static fdt_t fdt;

static file_t *fdfind(uint32_t fd)
{
	if(fd >= fdt.fdlen)
		return NULL;

	return fdt.fdarray[fd];
}

static bool fdextrude()
{
	/* allocate first */
	if(fdt.fdarray == NULL)
	{
		fdt.fdarray = k_malloc(sizeof(ptr_t) * FD_ALLOCATE_BATCH);
		fdt.fdlen = FD_ALLOCATE_BATCH;
		return true;
	}

	/* reallocate if already allocated */
	fdt.fdarray = k_realloc(fdt.fdarray, sizeof(ptr_t) * (fdt.fdlen + FD_ALLOCATE_BATCH));
	if(fdt.fdarray == NULL)
		return false;

	fdt.fdlen += FD_ALLOCATE_BATCH;
	return true;
}

/* check fd array on empty elements */
static bool fdfind_first()
{
	int i = 1;
	for(; i < fdt.fdlen; i++)
	{
		if(fdt.fdarray[i] == NULL)
		{
			fdt.fdcounter = i;
			return true;
		}
	}

	return false;
}

static bool fdcheck()
{
	if(!fdfind_first())
	{
		if(fdextrude())
		{
			return fdcheck();
		}
		return false;
	}

	return true;
}

bool k_finit()
{
	fdt.fdarray = NULL;
	fdt.fdlen = 0;
	fdt.fdcounter = 0;
	return true;
}

uint32_t k_fopen(const char *path, uint32_t mode)
{
	file_t *file = NULL;
	file = k_malloc(sizeof(file_t));
	k_memset(file, 0, sizeof(file_t));

	if(!file)
		return 0;

	if(mode & FILE_OPEN_IN_VFS)
	{
		/* get file desc in vfs */
		file = k_vfs_open_file(file, path, mode);
	}

	if(!file->open)
		goto failed;

	if(!file->open(path, mode, file))
		goto failed;

	/* check next file descriptor is present */
	/* if not - fd array is full, and relocation failed */
	if(!fdcheck())
		goto failed;
	/* setup next file descriptor */
	fdt.fdarray[fdt.fdcounter] = file;
	return fdt.fdcounter;

failed:
	k_free(file);
	return 0;
}

bool k_fclose(uint32_t fd)
{
	file_t *file = NULL;
	if( (file = fdfind(fd)) != NULL)
	{
		if(file->close)
			file->close(file);
		k_vfs_close_file(file);
		/* empty fd desc */
		k_free(file);
		fdt.fdarray[fd] = NULL;

		return true;
	}

	return false;
}

long k_fread(uint32_t fd, ptr_t buf, size_t size, size_t count)
{
	file_t *file = NULL;
	if((file = fdfind(fd)) != NULL)
	{
		if(file->read)
			return file->read(buf, size, count, file);
	}

	return -1;
}

long k_fwrite(uint32_t fd, const ptr_t buf, size_t size, size_t count)
{
	file_t *file = NULL;
	if((file = fdfind(fd)) != NULL)
	{
		if(file->write)
			return file->write(buf, size, count, file);
	}

	return -1;
}

long k_fseek(uint32_t fd, uint32_t offset, uint32_t whence)
{
	file_t *file = NULL;
	if((file = fdfind(fd)) != NULL)
	{
		if(file->seek)
			return file->seek(offset, whence, file);
	}

	return -1;
}

void k_fflush(uint32_t fd)
{
	file_t *file = NULL;
	if((file = fdfind(fd)) != NULL)
		file->flush(file);
}
