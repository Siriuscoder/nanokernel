
#include <fs/vfs.h>
#include <fs/file.h>

#include "vfs_test.h"

static void *file_open_1(const char *path, uint32_t mode, void *file)
{
	
}

static void *file_open_2(const char *path, uint32_t mode, void *file)
{
	
}

static bool file_close_1(void *file)
{
	
}

static bool file_close_2(void *file)
{
	
}

static long file_write_1(const ptr_t buf, size_t size, size_t count, void *file)
{
	
}

static bool setup_vfs()
{
	node_t fsNodeOut1 = {
		"stdout1",
		file_open_1, 
		file_close_1, 
		NULL,
		file_write_1,
		NULL,
		NULL
	};
	
	node_t fsNodeOut2 = {
		"stdout2",
		file_open_2, 
		file_close_2, 
		NULL,
		file_write_1,
		NULL,
		NULL
	};
	
	if(!k_vfs_mknode("/dev/", &fsNodeOut1, 0x0))
		return false;
	if(!k_vfs_mknode("/dev/", &fsNodeOut2, 0x0))
		return false;
	
	return true;
}

static bool valid_vfs()
{
		node_t testNode = {
		"stdout1",
		file_open_1, 
		file_close_1, 
		NULL,
		file_write_1,
		NULL,
		NULL
	};
		
	if(!k_vfs_mknode("/dev/", &testNode, 0x0))
		return true;
		
	return false;
}

bool run_vfs_test()
{
	if(!k_vfs_init())
		return false;
	if(!setup_vfs())
		return false;
	if(!valid_vfs())
		return false;
	
    return true;
}

