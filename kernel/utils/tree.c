/*
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

#include <utils/tree.h>

struct tree_link *
tree_link_init(struct tree_link *link)
{
	list_init(&link->children);
	list_link_init(&link->listLink);
	link->par = NULL;
	return link;
}

void
tree_add_link(struct tree_link *parent, struct tree_link *link)
{
	list_add_last_link(&link->listLink, &parent->children);
	link->par = parent;
}

void
tree_move_link(struct tree_link *parent, struct tree_link *link)
{
	if (parent != link->par)
	{
		tree_unlink_link(link);
		tree_add_link(parent, link);
	}
}

int
tree_unlink_link(struct tree_link *link)
{
	if (link->par != NULL)
	{
		list_unlink_link(&link->listLink);
		link->par = NULL;
		return 1;
	}
	return 0;
}

struct tree_link *
tree_find_arg(struct tree_link *tree, tree_link_arg_predicate_t predicate, void *arg)
{
	struct list_link *iter, *enditer;
	struct tree_link *item;

	if(list_is_empty(&tree->children))
		return NULL;

	iter = list_first_link(&tree->children);
	enditer = list_last_link(&tree->children);

	for(; iter != enditer; iter = list_next(iter))
	{
		item = MEMBERCAST(struct tree_link, iter, listLink);
		if (predicate(item, arg))
		{
			return item;
		}

		if((item = tree_find(item, predicate)) != NULL)
			return item;
	}

	return NULL;
}

void
tree_foreach(struct tree_link *tree, tree_link_predicate_t predicate)
{
	tree_find_arg(tree, predicate, NULL);
}

