/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#pragma once

/**
 * @file  list.h
 * @brief Linked list API.
 */

#include <lib/error.h>
#include <lib/types.h>

/** A cyclic, double-linked list. */
struct list {
	struct list *next;
	struct list *prev;
};

/**
 * List head initializer macro.
 * @param _id Identifier of the list head.
 */
#define LIST_INITIALIZER(_id) { .next = (_id), .prev = (_id) }

/**
 * List head typename macro.
 * @param _id Identifier we wish to assign to the declared list head.
 */
#define LIST_HEAD(_id) struct list _id = LIST_INITIALIZER(&(_id))

/**
 * Initialize a list head.
 * @param _head List head that is to be initialized.
 */
#define list_init(_head)                 \
	do {                             \
		(_head)->next = (_head); \
		(_head)->prev = (_head); \
	} while (0)

/**
 * Insert a new element to a list.
 * @param after  Element that is to precede the inserted one.
 * @param before Element that is to follow the inserted one.
 * @param new    Element to be added.
 *
 * @return @ref err_t
 */
err_t list_insert(struct list *after, struct list *before, struct list *new);

/**
 * Append a new element to a list.
 * @param target Head of the list that is to be extended.
 * @param new    Element to be added.
 *
 * @return @ref err_t
 */
err_t list_append(struct list *target, struct list *new);

/**
 * Prepend a new element to a list.
 * @param target Head of the list that is to be extended.
 * @param new    Element to be added.
 *
 * @return @ref err_t
 */
err_t list_prepend(struct list *target, struct list *new);

/**
 * Remove an element from a list.
 * @param elem Element to be removed.
 *
 * @return @ref err_t
 */
err_t list_remove(struct list *elem);

/**
 * Iterate over a list.
 * @param _i    Iterator variable.
 * @param _head Head of the list.
 */
#define list_foreach(_i, _head) \
	for ((_i) = (_head)->next; (_i) != (_head); (_i) = (_i)->next)

/**
 * Obtain first element of a list.
 * @param head Head of the list.
 *
 * @return NULL if list whose head is @a head is empty or @a head is NULL.
 * @return Pointer to first element of the list otherwise.
 */
struct list *list_car(struct list *head);
