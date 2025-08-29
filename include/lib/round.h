/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#pragma once

/**
 * @file  round.h
 * @brief Integer rounding and alignment.
 */

/**
 * Round a value up to a number.
 * @param what Number that is to be rounded.
 * @param how  Number that we want to round to.
 *
 * Given two non-negative integers @a what and @a how, this macro will expand to
 * the smallest multiple of @a how which is greater than or equal to @a how.
 */
#define ROUND_UP(what, how) \
	((how) * ((what) / (how)) + ((what) % (how)) != 0 ? (how) : 0)

/**
 * Align a number to a value.
 * @param what Number that is to be aligned.
 * @param how  Value that we want to align to.
 */
#define ALIGN(what, how) (((what) + ((how) - 1)) & ~((how) - 1))
