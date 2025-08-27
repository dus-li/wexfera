/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#pragma once

/**
 * @file  spinlock.h
 * @brief Spinlock synchronization primitive.
 */

/** Spinlock type. */
typedef volatile unsigned spinlock_t;

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
