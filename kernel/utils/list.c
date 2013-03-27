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
/** embox
 * @author Eldar Abusalimov
 */

#include <utils/list.h>

#define __list_check(expr) (expr)
#define __LIST_POISON       0x0f00
#define __LIST_LINK_POISON  0x0b0b

static inline void
__list_bind(struct list_link *prev, struct list_link *next)
{
	__list_check(next)->prev = prev;
	__list_check(prev)->next = next;
}

static inline bool
__list_link_alone(struct list_link *link)
{
	return link == __list_check(link)->next;
}

static inline void
__list_link_init(struct list_link *link)
{
	__list_bind(link, link);
}

static inline void
__list_insert_chain(struct list_link *first, struct list_link *last,
		struct list_link *prev, struct list_link *next)
{
	__list_bind(prev, first);
	__list_bind(last, next);
}

static inline void
__list_insert_link(struct list_link *link, struct list_link *prev,
		struct list_link *next)
{
	__list_insert_chain(link, link, prev, next);
}

struct list_link *
list_link_init(struct list_link *link)
{
	link->poison = __LIST_LINK_POISON;
	link->list = NULL;
	__list_link_init(link);

	return link;
}

struct list *
list_init(struct list *list)
{
	__list_link_init(&list->l);

	return list;
}

bool
list_alone_link(struct list_link *link)
{
	return __list_link_alone(link);
}

int
list_is_empty(struct list *list)
{
	return __list_link_alone(&list->l);
}

struct list_link *
list_first_link(struct list *list)
{
	struct list_link *l, *first;

	l = &list->l;
	first = l->next;
	return first != l ? first : NULL;
}

struct list_link *
list_last_link(struct list *list)
{
	struct list_link *l, *last;

	l = &list->l;
	last = l->prev;
	return last != l ? last : NULL;
}

void
list_add_first_link(struct list_link *new_link, struct list *list)
{
	struct list_link *l;

	l = &list->l;
	__list_insert_link(new_link, l, l->next);
}

void
list_add_last_link(struct list_link *new_link, struct list *list)
{
	struct list_link *l;

	l = &list->l;
	__list_insert_link(new_link, l->prev, l);
}

void
list_insert_before_link(struct list_link *new_link, struct list_link *link)
{
	__list_insert_link(new_link, link->prev, link);
}

void
list_insert_after_link(struct list_link *new_link, struct list_link *link)
{
	__list_insert_link(new_link, link, link->next);
}

void
list_bulk_add_first(struct list *from_list, struct list *to_list)
{
	struct list_link *from, *to;

	if (!list_is_empty(from_list))
	{
		from = &from_list->l;
		to = &to_list->l;

		__list_insert_chain(from->next, from->prev, to, to->next);
		__list_link_init(from);
	}
}

void
list_bulk_add_last(struct list *from_list, struct list *to_list)
{
	struct list_link *from, *to;

	if (!list_is_empty(from_list))
	{
		from = &from_list->l;
		to = &to_list->l;

		__list_insert_chain(from->next, from->prev, to->prev, to);
		__list_link_init(from);
	}
}

void
list_bulk_insert_before_link(struct list *from_list, struct list_link *link)
{
	struct list_link *from;

	if (!list_is_empty(from_list))
	{
		from = &from_list->l;

		__list_insert_chain(from->next, from->prev, link->prev, link);
		__list_link_init(from);
	}
}

void
list_bulk_insert_after_link(struct list *from_list, struct list_link *link)
{
	struct list_link *from;

	if (!list_is_empty(from_list))
	{
		from = &from_list->l;

		__list_insert_chain(from->next, from->prev, link, link->next);
		__list_link_init(from);
	}
}

void
list_unlink_link(struct list_link *link)
{
	__list_bind(link->prev, link->next);
	__list_link_init(link);
}

struct list_link *
list_remove_first_link(struct list *list)
{
	struct list_link *ret;

	if ((ret = list_first_link(list)))
	{
		list_unlink_link(ret);
	}

	return ret;
}

struct list_link *
list_remove_last_link(struct list *list)
{
	struct list_link *ret;

	if ((ret = list_last_link(list)))
	{
		list_unlink_link(ret);
	}

	return ret;
}

struct list_link *
list_next(struct list_link *link)
{
	return link->next;
}

struct list_link *
list_prev(struct list_link *link)
{
	return link->prev;
}

struct list_link *
list_find_arg(struct list *in_list, list_link_arg_predicate_t predicate, void *arg)
{
	struct list_link *it;


	for(it = in_list->l.next; it != &(in_list->l); it = list_next(it))
	{
		if(predicate(it, arg))
			return it;
	}

	return NULL;
}

