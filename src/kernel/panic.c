/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#define LOG_FMT(_fmt) _fmt

#include <kernel/panic.h>

#include <lib/log.h>

SYSCALL_VOID(_sys_panic)
{
	struct ctx_full frame;
	SYSCALL_ARG(0, const char *, msg);

	ctx_collect(&frame, SYSCALL_FRAME);
	_panic(&frame, msg);
}

void _Noreturn _panic(struct ctx_full *frame, const char *msg)
{
	log_always("\nPanic!\n");
	log_always("Reason: %s\n", msg);

	ctx_log(LOG_ALWAYS, frame);

	loop {
	}
}
