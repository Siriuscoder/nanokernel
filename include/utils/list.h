/*
 * list.h
 *
 *  Created on: Mar 24, 2013
 *      Author: sirius
 */

#ifndef LIST_H_
#define LIST_H_

#include <types.h>
#include <utils/membercast.h>

struct list_link
{
	unsigned int poison;
	struct list *list;
	struct list_link *next, *prev;
};

struct list
{
	struct list_link l;
};

/**
 * @param list
 *   Pointer to the list being initialized.
 * @return
 *   The argument.
 */
struct list *
list_init(struct list *list);

/**
 * @param link
 *   Pointer to the link.
 * @return
 *   The argument.
 */
struct list_link *
list_link_init(struct list_link *link);

bool
list_is_empty(struct list *list);

int
list_alone_link(struct list_link *link);

/* Unlinking an element from its list. */
void
list_unlink_link(struct list_link *link);

/* Retrieving the first/last elements. */
struct list_link *
list_first_link(struct list *list);

struct list_link *
list_last_link(struct list *list);

/* Adding an element at the list ends. */
void
list_add_first_link(struct list_link *new_link, struct list *list);

void
list_add_last_link(struct list_link *new_link, struct list *list);

/* Insertion of an element near a given one. */
void
list_insert_before_link(struct list_link *new_link, struct list_link *link);

void
list_insert_after_link(struct list_link *new_link, struct list_link *link);

/* Popping an element from the list ends. */
struct list_link *
list_remove_first_link(struct list *list);

struct list_link *
list_remove_last_link(struct list *list);

void
list_bulk_add_first(struct list *from_list, struct list *to_list);

void
list_bulk_add_last(struct list *from_list, struct list *to_list);

void
list_bulk_insert_before_link(struct list *from_list, struct list_link *link);

void
list_bulk_insert_after_link(struct list *from_list, struct list_link *link);

struct list_link *
list_next(struct list_link *link);

struct list_link *
list_prev(struct list_link *link);

#endif /* LIST_H_ */
