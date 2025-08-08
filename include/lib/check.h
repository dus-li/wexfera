/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#pragma once

/**
 * @file  check.h
 * @brief Asserts and checks.
 */

#include <lib/preproc.h>

#define check_boolify(_val) (!!(_val))

/**
 * Ensure that a condition holds true at build time.
 * @param _pred Predicate that is to be evaluated.
 *
 * This is largely similar to @a _Static_assert, except it does not need to be
 * placed within a function and therefore is better suited for macros that
 * are used in file scope.
 */
#define CHECK_BUILDTIME(_pred) \
	int CONCAT(__check_buildtime_, __LINE__)[-1 + check_boolify(_pred)]
