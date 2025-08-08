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

#define BITS(_hi, _lo) (_BITS(_hi) & (~_BITS(_lo) >> 1))

#define BITS_CONS(_val, _mask) \
	(((_val) & ((_mask) >> ctz(_mask))) << ctz(_mask))
