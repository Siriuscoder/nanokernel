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
	fsnode_t *devNode, *tmpNode, *mntNode;
	devNode = k_malloc(sizeof(fsnode_t));
	tmpNode = k_malloc(sizeof(fsnode_t));
	mntNode = k_malloc(sizeof(fsnode_t));

    k_memset(devNode, 0, sizeof(fsnode_t));
    k_memset(tmpNode, 0, sizeof(fsnode_t));
    k_memset(mntNode, 0, sizeof(fsnode_t));
    
    devNode->flags |= FILE_IS_FOLDER;
    k_strcpy(devNode->node.name, "dev/");
    tree_link_init(&devNode->link);
    list_init(&devNode->openFiles);
    
    tmpNode->flags |= FILE_IS_FOLDER;
    k_strcpy(tmpNode->node.name, "tmp/");
    tree_link_init(&tmpNode->link);
    list_init(&tmpNode->openFiles);    

    mntNode->flags |= FILE_IS_FOLDER;
    k_strcpy(mntNode->node.name, "mnt/");
    tree_link_init(&mntNode->link);
    list_init(&mntNode->openFiles);
    /* add default nodes to vfs tree */
    tree_add_link(&vfs_root.link, &devNode->link);
    tree_add_link(&vfs_root.link, &tmpNode->link);
    tree_add_link(&vfs_root.link, &mntNode->link);

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
	k_strcpy(tmp, path);
	while(true)
	{
		char *s = k_strchr(ps, '/');
		if(s == NULL)
			break;
		if(k_strlen(s) == 0)
			return NULL;

		*s = 0;
		searchLink = list_find_arg(&searchTreeNode->children, find_file_node_in_list, s);
		/* if no return - exit */
		if(!searchLink)
			return NULL;

		/* setup new tree node */
		searchTreeNode = MEMBERCAST(struct tree_link, searchLink, listLink);
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
	k_strcpy(file->fullPath, path);
	k_strcpy(file->name, fsNode->node.name);
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
    
    /* check if file already exist */
    k_strcat(fullName, path);
    k_strcat(fullName, node->name);
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
	return false;
}




