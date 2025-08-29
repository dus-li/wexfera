/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#pragma once

#include <board.h>

#include <lib/error.h>
#include <lib/types.h>

/**
 * @file  alloc.h
 * @brief Memory management and dynamic allocations.
 */

/**
 * Initialize memory manager.
 * @return @ref err_t
 */
err_t alloc_init(void);

/**
 * Allocate a chunk of memory.
 * @param size Size of the requested memory region.
 *
 * @return An error pointer on failure.
 * @return An address to allocated memory chunk.
 */
void *alloc_get(size_t size);

/**
 * Free a chunk of allocated memory.
 * @param addr Address of the memory that is to be freed.
 *
 * @return @ref err_t
 */
err_t alloc_free(void *addr);

#if (BOARD_DEBUG_FEATURES == 1)
void alloc_describe_heap(void);
#else // BOARD_DEBUG_FEATURES != 1
  #define alloc_describe_heap(...)
#endif // BOARD_DEBUG_FEATURES
