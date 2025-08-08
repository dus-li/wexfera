/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#pragma once

/**
 * @file  sequence.h
 * @brief Boot sequence hooks.
 */

#include <lib/check.h>
#include <lib/compiler.h>
#include <lib/preproc.h>

typedef void (*seq_func_t)(void);

#define __SEQ_EARLY_INIT_SEC ".data.seq.early"

#define SEQ_EARLY_INIT(_fn)                                   \
	CHECK_BUILDTIME(same_type(typeof(&_fn), seq_func_t)); \
	const seq_func_t __section(__SEQ_EARLY_INIT_SEC)      \
	    CONCAT(__seq_early, __LINE__) = (_fn)

void seq_run_early_init(void);
