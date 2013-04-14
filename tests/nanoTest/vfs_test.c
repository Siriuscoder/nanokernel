
#include <fs/vfs.h>
#include <fs/file.h>
#include <std/print.h>

#include "vfs_test.h"

static void *file_open_1(const char *path, uint32_t mode, void *file)
{
	k_print("File1 %s opened!\n", path);
	return file;
}

static void *file_open_2(const char *path, uint32_t mode, void *file)
{
	k_print("File2 %s opened!\n", path);
	return file;
}

static bool file_close_1(void *file)
{
	file_t *myfile = (file_t *)file;
	k_print("file_close_1 %s, %d\n", myfile->name, myfile->fd);
	return true;
}

static bool file_close_2(void *file)
{
	file_t *myfile = (file_t *)file;
	k_print("file_close_2 %s, %d\n", myfile->name, myfile->fd);
	return true;	
}

static long file_write_1(const ptr_t buf, size_t size, size_t count, void *file)
{
	file_t *myfile = (file_t *)file;
	k_print("file_write_1 %s, %d\n", myfile->name, myfile->fd);
	return true;
}

static void dir_iterator(const char *name, uint32_t flags, void *arg)
{
	k_print("-> %s %s\n", name, flags & FILE_IS_FOLDER ? "->" : "");
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
	
	/* add default folders */
	if(!k_vfs_mkdir("/", "mnt"))
		return false;
	if(!k_vfs_mkdir("/", "dev"))
		return false;
	if(!k_vfs_mkdir("/", "tmp"))
		return false;
	
	if(!k_vfs_mknode("/dev/", &fsNodeOut1, 0x0))
		return false;
	if(!k_vfs_mknode("/dev/", &fsNodeOut2, 0x0))
		return false;
	
	if(!k_vfs_mkdir("/dev", "net"))
		return false;
	if(!k_vfs_mkdir("/dev", "disk"))
		return false;
	
	k_strcpy(fsNodeOut1.name, "net1");
	k_strcpy(fsNodeOut2.name, "net2");
	
	if(!k_vfs_mknode("/dev/net", &fsNodeOut1, 0x0))
		return false;
	if(!k_vfs_mknode("/dev/net", &fsNodeOut2, 0x0))
		return false;
	
	k_strcpy(fsNodeOut1.name, "hdd1");
	if(!k_vfs_mknode("/dev/", &fsNodeOut1, 0x0))
		return false;
	
	k_print("/dev/=====\n");
	if(!k_vfs_iterate_directory("/dev", dir_iterator, NULL))
		return false;
	k_print("/dev/net/=====\n");
	if(!k_vfs_iterate_directory("/dev/net", dir_iterator, NULL))
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
		
	if(k_vfs_mknode("/dev/", &testNode, 0x0))
		return false;
	if(k_vfs_mkdir("/dev", "net"))
		return false;
	if(k_vfs_mkdir("/dev/net/", "net1"))
		return false;
		
	return true;
}

static bool rm_mv_some_vfs_nodes()
{
	k_print("/dev/=====\n");
	if(!k_vfs_iterate_directory("/dev/", dir_iterator, NULL))
		return false;
	k_print("/dev/disk=====\n");
	if(!k_vfs_iterate_directory("/dev/disk", dir_iterator, NULL))
		return false;
	
	if(!k_vfs_mvnode("/dev/hdd1", "/dev/disk"))
		return false;
	
	k_print("/dev/=====\n");
	if(!k_vfs_iterate_directory("/dev/", dir_iterator, NULL))
		return false;
	k_print("/dev/disk=====\n");
	if(!k_vfs_iterate_directory("/dev/disk", dir_iterator, NULL))
		return false;
	
	if(k_vfs_mvnode("/dev/disk/hdd1", "/dev/disk/folder/"))
		return false;

	if(k_vfs_rmnode("/dev/disk/folder"))
		return false;
	
	if(!k_vfs_rmnode("/dev/disk/hdd1"))
		return false;
	if(!k_vfs_rmdir("/dev/disk/"))
		return false;
	k_print("/dev/=====\n");
	if(!k_vfs_iterate_directory("/dev/", dir_iterator, NULL))
		return false;
}

static bool files_working_test()
{
	uint32_t fd1, fd2;
	
	fd1 = k_fopen("/dev/disk/", FILE_OPEN_IN_VFS | FILE_OUT);
	if(fd1)
		return false;
	
	fd1 = k_fopen("/dev/net/", FILE_OPEN_IN_VFS | FILE_OUT);
	if(fd1)
		return false;
	
	fd1 = k_fopen("/dev/stdout1", FILE_OPEN_IN_VFS | FILE_OUT);
	if(!fd1)
		return false;
	fd2 = k_fopen("/dev/net/net2", FILE_OPEN_IN_VFS | FILE_OUT);
	if(!fd2)
		return false;
	
	k_fwrite(fd1, "test1", 5, 1);
	k_fwrite(fd2, "test2", 5, 1);
	
	if(!k_fclose(fd1))
		return false;
	if(!k_fclose(fd2))
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
	if(!rm_mv_some_vfs_nodes())
		return false;
	if(!files_working_test())
		return false;
	
    return true;
}

