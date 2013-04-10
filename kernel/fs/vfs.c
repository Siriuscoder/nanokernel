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

#include <fs/vfs.h>
#include <std/strings.h>
#include <std/membase.h>
#include <mem.h>

static fsnode_t vfs_root;

bool k_vfs_init()
{
	k_memset(&vfs_root, 0, sizeof(fsnode_t));

	tree_link_init(&vfs_root.link);
	vfs_root.flags = FILE_IS_FOLDER;
	vfs_root.node.name[0] = '/';
	/* add default folders */
	k_vfs_mkdir("/", "mnt");
	k_vfs_mkdir("/", "dev");
	k_vfs_mkdir("/", "tmp");

	return true;
}

static bool find_file_node_in_list(struct list_link *link, void *arg)
{
	struct tree_link *cur = MEMBERCAST(struct tree_link, link, listLink);
	fsnode_t *curNode = MEMBERCAST(fsnode_t, cur, link);

	if(k_strcmp(curNode->node.name, (char *)arg) == 0)
		return true;

	return false;
}

fsnode_t *k_vfs_find_node_by_full_path(const char *path)
{
	char tmp[FILE_FULL_PATH];
	char *ps = tmp;
	struct tree_link *searchTreeNode = &vfs_root.link;
	struct list_link *searchLink;
	/* copy path to tmp buffer first */
	k_strncpy(tmp, path, FILE_FULL_PATH);
	/* add '/' to end for all */
	k_strncat(tmp, "/", FILE_FULL_PATH);
	while(true)
	{
		char *s = k_strchr(ps, '/');	
		if(s == NULL)
			break;
		/* cat path string to first '/' */		
		*s = 0;
		/* if s is empty to next token */
		if(k_strlen(ps) == 0)
			goto to_next;
		
		searchLink = list_find_arg(&searchTreeNode->children, find_file_node_in_list, ps);
		/* if no return - exit */
		if(!searchLink)
			return NULL;

		/* setup new tree node */
		searchTreeNode = MEMBERCAST(struct tree_link, searchLink, listLink);

to_next:		
		/* setup ps to next path token */
		ps = s+1;
	}

	return MEMBERCAST(fsnode_t, searchTreeNode, link);
}

file_t *k_vfs_open_file(file_t *file, const char *path, uint32_t mode)
{
	fsnode_t *fsNode;
	if((fsNode = k_vfs_find_node_by_full_path(path)) == NULL)
		return NULL;
	/* if found node is folder return */
	if(fsNode->flags & FILE_IS_FOLDER)
		return NULL;

	file->open = fsNode->node.open;
	file->close = fsNode->node.close;
	file->write = fsNode->node.write;
	file->read = fsNode->node.read;
	file->seek = fsNode->node.seek;
	file->flush = fsNode->node.flush;
	k_strncpy(file->fullPath, path, FILE_FULL_PATH);
	k_strncpy(file->name, fsNode->node.name, FILE_NAME_LEN);
	/* add new open file to list */
	list_link_init(&file->l);
	list_add_last_link(&file->l, &fsNode->openFiles);

	return file;
}

file_t *k_vfs_close_file(file_t *file)
{
 /* unlink and flush file */
	list_unlink_link(&file->l);
	list_link_init(&file->l);
	if(file->flush)
		file->flush(file);

	return file;
}

bool k_vfs_mknode(const char *path, const node_t *node, uint32_t flags)
{
    fsnode_t *fsNode, *dstNode;
    char fullName[FILE_FULL_PATH] = { '\0' }; 
    
	fsNode = k_malloc(sizeof(fsnode_t));
	if(!fsNode)
		goto failed_exit;
    
	k_memset(fsNode, 0, sizeof(fsnode_t));
	tree_link_init(&fsNode->link);
	list_init(&fsNode->openFiles);
	fsNode->node = *node;
	fsNode->flags = flags;
	if((dstNode = k_vfs_find_node_by_full_path(path)) == NULL)
		goto failed;
    
	/* it mast a directory */
	if(!(dstNode->flags & FILE_IS_FOLDER))
		goto failed;
    /* check if file already exist */
	k_strncat(fullName, path, FILE_FULL_PATH);
	k_strncat(fullName, node->name, FILE_FULL_PATH);
	if(k_vfs_find_node_by_full_path(fullName))
		goto failed;
    
    /* add to vfs tree */
	tree_add_link(&dstNode->link, &fsNode->link);
	return true;
    
failed:
	k_free(fsNode);
failed_exit:
	return false;
}

bool k_vfs_rmnode(const char *path)
{
	fsnode_t *fsNode;
	if((fsNode = k_vfs_find_node_by_full_path(path)) == NULL)
		return false;
	
	/* if folder not empty - return false */
	/* TODO: remove sub-catalogs later */
	if(!list_is_empty(&fsNode->link.children))
		return false;
	tree_unlink_link(&fsNode->link);
	k_free(fsNode);
	
	return true;
}

bool k_vfs_mkdir(const char *path, const char *name)
{
	node_t dirNode;
	k_memset(&dirNode, 0, sizeof(dirNode));
	k_strcpy(dirNode.name, name);
	
	return k_vfs_mknode(path, &dirNode, FILE_IS_FOLDER);
}

bool k_vfs_rmdir(const char *path)
{
	return k_vfs_rmnode(path);
}

bool k_vfs_mvnode(const char *path, const char *newPath)
{
	fsnode_t *fsNode, *dstNode;	    
	if((fsNode = k_vfs_find_node_by_full_path(path)) == NULL)
		return false;

	/* go to destination  node */
	if((dstNode = k_vfs_find_node_by_full_path(newPath)) == NULL)
		return false;
	
	/* if exist - return */
	if(list_find_arg(&dstNode->link.children, 
		find_file_node_in_list, fsNode->node.name))
		return false;
	
	tree_move_link(&dstNode->link, &fsNode->link);
	return true;
}




