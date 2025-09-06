/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#pragma once

/**
 * @file  extrema.h
 * @brief Greatest and least element macros.
 */

/**
 * Return smaller of the two values.
 * @param _a First number.
 * @param _b Second number.
 */
#define min(_a, _b)                     \
	({                              \
		__auto_type __a = (_a); \
		__auto_type __b = (_b); \
		__a < __b ? __a : __b;  \
	})

/**
 * Return larger of the two values.
 * @param _a First number.
 * @param _b Second number.
 */
#define max(_a, _b)                     \
	({                              \
		__auto_type __a = (_a); \
		__auto_type __b = (_b); \
		__a > __b ? __a : __b;  \
	})
