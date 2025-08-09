/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#pragma once

#include <lib/compiler.h>
#include <lib/preproc.h>

#define ARRAY_SIZE(_arr)                                       \
	({                                                     \
		_Static_assert(!same_type((_arr), &(_arr)[0]), \
		    "'" STRINGIFY(_arr) "' must be an array"); \
		sizeof(_arr) / sizeof(_arr[0]);                \
	})
