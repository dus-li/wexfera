/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#pragma once

/**
 * @file  bits.h
 * @brief Bit operations and masks.
 */

#include <lib/compiler.h>
#include <lib/types.h>

#define _BITS(_upto) \
	((_upto) == 0 ? 1U : (_upto) > 31 ? ~0U : ~0U >> (31 - (_upto)))

/**
 * Generate a continuous bitmask.
 * @param _hi Most significant bit to include in the mask.
 * @param _lo Least significant bit to include in the mask.
 */
#define BITS(_hi, _lo) (_BITS(_hi) & (~_BITS(_lo) >> 1))

/**
 * Construct a bitset field.
 * @param _val  Value to be placed in the field.
 * @param _mask Mask with bits that are to be occupied by the value.
 */
#define BITS_CONS(_val, _mask) (((_val) << ctz(_mask)) & (_mask))

/**
 * Extract a field from a bitset.
 * @param _val  Bitset to extract from.
 * @param _mask Mask with bits that are to be extracted.
 */
#define BITS_EXTRACT(_val, _mask) (((_val) & (_mask)) >> ctz(_mask))
