/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#include <arch/barriers.h>
#include <arch/spinlock.h>

#include <lib/compiler.h>

#define SPINLOCK_UNLOCKED (0)
#define SPINLOCK_LOCKED   (1)

void spinlock_init(spinlock_t *lock)
{
	*lock = SPINLOCK_UNLOCKED;
}

void spinlock_lock(spinlock_t *lock)
{
	spinlock_t cur;

	loop {
		do {
			cur = __load_exclusive(lock);
		} while (cur != SPINLOCK_UNLOCKED);

		if (__store_exclusive(lock, SPINLOCK_LOCKED) == 0)
			break;
	}
}

void spinlock_unlock(spinlock_t *lock)
{
	__data_barrier;
	*lock = SPINLOCK_UNLOCKED;
}
