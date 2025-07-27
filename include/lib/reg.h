/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#pragma once

/**
 * @file  reg.h
 * @brief Register operations.
 */

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
		asm volatile("dmb");                    \
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
		asm volatile("dmb");      \
		*(volatile u32 *)(_addr); \
	})
