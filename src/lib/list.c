/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#include <lib/list.h>

err_t list_insert(struct list *after, struct list *before, struct list *new)
{
	if (after == NULL || before == NULL || new == NULL)
		return -ERR_ARG;

	if (after->next != before || before->prev != after)
		return -ERR_CORRUPT;

	new->prev    = after;
	new->next    = before;
	after->next  = new;
	before->prev = new;

	return ERR_NONE;
}

err_t list_append(struct list *target, struct list *new)
{
	return list_insert(target->prev, target, new);
}

err_t list_prepend(struct list *target, struct list *new)
{
	return list_insert(target, target->next, new);
}

err_t list_remove(struct list *elem)
{
	struct list *prev, *next;

	if (elem == NULL)
		return -ERR_ARG;

	prev = elem->prev;
	next = elem->next;

	if (prev->next != elem || next->prev != elem)
		return -ERR_CORRUPT;

	next->prev = prev;
	prev->next = next;
	elem->next = elem->prev = NULL;

	return ERR_NONE;
}
