/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#pragma once

/**
 * @file  _printfmt.h
 * @brief Print format processor.
 */

#include <lib/compiler.h>
#include <lib/error.h>
#include <lib/types.h>

/** Stateful callback capable of receiving stream of processed characters. */
typedef err_t (*_pfmt_cb)(char, void *);

/** Print format processor frontent control block. */
struct _pfmt_fe {
	_pfmt_cb writer; ///< Last stage callback. @ref _pfmt_cb
	void    *state;  ///< @a writer state.
};

/**
 * Run print format processing pipeline.
 * @param fe  Frontend that is to be used at the last stage of processing.
 * @param fmt Format specifier string.
 * @param ... Variadic argument list.
 *
 * @return @ref err_t
 */
err_t __printfmt(2, 3) _pfmt_proc(struct _pfmt_fe *fe, const char *fmt, ...);

/**
 * Run print format processing pipeline for a variadic list.
 * @param fe   Frontend that is to be used at the last stage of processing.
 * @param fmt  Format specifier string.
 * @param args Argument list.
 *
 * @return @ref err_t
 */
err_t _pfmt_vproc(struct _pfmt_fe *fe, const char *fmt, va_list *args);
