/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#pragma once

/**
 * @file  reg.h
 * @brief Register operations.
 */

#include <arch/barriers.h>

#include <lib/types.h>

/**
 * Safely write a 32-bit value after a memory barrier.
 * @param _val  Value that is to be written.
 * @param _addr Address where the value is to be written.
 *
 * This is an equivalent of @a writel often seen in other projects.
 * @see read32
 */
#define write32(_val, _addr)                            \
	do {                                            \
		__data_barrier;                         \
		*(volatile u32 *)(_addr) = (u32)(_val); \
	} while (0)

/**
 * Safely read a 32-bit value after a memory barrier.
 * @param _addr Address to read from.
 *
 * This is an equivalent of @a readl often seen in other projects.
 * @see write32
 */
#define read32(_addr)                     \
	({                                \
		__data_barrier;           \
		*(volatile u32 *)(_addr); \
	})

/**
 * Bitwise-sum a value with existing data in a register.
 * @param _val  Value that is to be added.
 * @param _addr Address to update.
 *
 * This captures a pattern of ORing a value to a readl result and then writing
 * it back, often seen in other projects.
 */
#define union32(_val, _addr) write32((_val) | read32(_addr), (_addr))

/**
 * Bitwise set difference with existing data in a register.
 * @param _val  Value that is to be subtracted (cleared).
 * @param _addr Address to update.
 *
 * This captures a pattern of ANDing an inverse of a value with readl result and
 * then writing it back, often seen in other projects.
 */
#define diff32(_val, _addr) write32(~((u32)(_val)) & read32(_addr), (_addr))
