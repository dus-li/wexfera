/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#pragma once

/**
 * @file  spinlock.h
 * @brief Spinlock synchronization primitive.
 */

#include <lib/cleanup.h>

#define SPINLOCK_UNLOCKED (0)
#define SPINLOCK_LOCKED   (1)

/** Spinlock type. */
typedef volatile unsigned spinlock_t;

#define SPINLOCK_INITIALIZER SPINLOCK_UNLOCKED

/**
 * Initialize a spinlock.
 * @param lock Spinlock that is to be initialized.
 */
void spinlock_init(spinlock_t *lock);

/**
 * Acquire a spinlock.
 * @param lock Spinlock that is to be locked.
 */
void spinlock_lock(spinlock_t *lock);

/**
 * Release a spinlock.
 * @param lock Spinlock that is to be unlocked.
 */
void spinlock_unlock(spinlock_t *lock);

CLEANUP_LOCK(spinlock, spinlock_t, spinlock_lock, spinlock_unlock);
