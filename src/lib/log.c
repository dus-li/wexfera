/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#include <board.h>

#include <arch/spinlock.h>

#include <lib/_printfmt.h>
#include <lib/array_size.h>
#include <lib/compiler.h>
#include <lib/error.h>
#include <lib/log.h>
#include <lib/types.h>

static err_t __log_writer(char c, void *_)
{
	if (c == '\n')
		board_log_be('\r');

	board_log_be(c);

	return ERR_NONE;
}

static struct _pfmt_fe log_fe = {
	.writer = __log_writer,
	.state  = NULL,
};

/** ANSI color codes added to different log levels. */
static const char *const _log_pfx_map[] = {
	[LOG_DEBUG]  = "\033[32m", // Green
	[LOG_INFO]   = "\033[36m", // Light blue
	[LOG_WARN]   = "\033[33m", // Yellow
	[LOG_ERROR]  = "\033[31m", // Red
	[LOG_ALWAYS] = "",
};

err_t __printfmt(2, 3) _log(enum log_levels lvl, const char *fmt, ...)
{
	static spinlock_t log_lock = SPINLOCK_INITIALIZER;

	va_list args;
	err_t   ret;

	if (lvl < BOARD_LOG_LEVEL)
		return ERR_NONE;

	va_start(args, fmt);

	LOCKED (spinlock, &log_lock) {
		ret = _pfmt_proc(&log_fe, "%s", _log_pfx_map[lvl]) ?:
		    _pfmt_vproc(&log_fe, fmt, &args)               ?:
		    _pfmt_proc(&log_fe, "\033[0m");
	}

	va_end(args);

	return ret;
}
