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
 *
 * @date 08.10.11
 * @author Avdyukhin Dmitry
 */

#ifndef TREE_H_
#define TREE_H_

#include <utils/list.h>
#include <utils/membercast.h>

struct tree_link
{
	/** Parent node in the tree. */
	struct tree_link *par;

	/** Children are represented as a list of nodes. */
	struct list children;

	/** List link inside of list of children. */
	struct list_link listLink;
};

/**
 * Type of functions, returning true or false for given tree_link.
 */
typedef int
(*tree_link_predicate_t)(struct tree_link *link);

/**
 * Type of functions, returning true or false for given tree_link with some argument.
 */
typedef int
(*tree_link_arg_predicate_t)(struct tree_link *link, void *arg);

/*
 * Initialize tree link.
 * @param tree_link Link to initialize.
 */
struct tree_link *
tree_link_init(struct tree_link *link);

/**
 * Add element to tree by adding new link into list of children of another node.
 *   Added tree_link must be initialized before and have no parent.
 *   Added element will be insert in the end of list of children.
 * @param parent Parent of new node.
 * @param link Added element
 */
void
tree_add_link(struct tree_link *parent, struct tree_link *link);

/**
 * Unlink specified tree_link from its parent and bind it to new parent.
 * @param parent Parent of new node.
 * @param link Added element
 */
void
tree_move_link(struct tree_link *parent, struct tree_link *link);

/**
 * Separates node from its parent.
 *   This parent can not exist.
 * @param link Unlinked element
 * @return
 * 	true, if element was in the tree before deletion.
 */
int
tree_unlink_link(struct tree_link *link);

/**
 * Find element of subtree, for what specified predicate is true.
 *
 * @param tree Node, subtree of what (including itself) is tested.
 * @param predicate Predicate, with what nodes are tested.
 *
 * @return Node, for what predicate is true, or NULL, if it doesn't exist.
 */
struct tree_link *
tree_find(struct tree_link *tree, tree_link_predicate_t predicate);

#endif /* TREE_H_ */
