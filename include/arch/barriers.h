/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#pragma once

/**
 * @file  barriers.h
 * @brief Memory barriers.
 */

/** Data memory barrier. */
#define __data_barrier asm volatile("dmb")

/**
 * Perform an exclusive load from an address.
 * @param addr Source address of the load.
 *
 * @return Value read from @a addr.
 */
static inline unsigned __load_exclusive(volatile unsigned *addr)
{
	unsigned ret;

	asm volatile("ldrex %0, [%1]" : "=r"(ret) : "r"(addr) : "memory");

	return ret;
}

/**
 * Perform an exclusive write to an address.
 * @param addr Destination address of the store.
 * @param val  Value that is to be written.
 *
 * @return @a 0 on success.
 * @return @a 1 on failure.
 */
static inline unsigned __store_exclusive(volatile unsigned *addr, unsigned val)
{
	unsigned ret;

	asm volatile("strex %0, %1, [%2]"
	    : "=&r"(ret)
	    : "r"(val), "r"(addr)
	    : "memory");

	return ret;
}
