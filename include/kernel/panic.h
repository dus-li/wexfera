/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#pragma once

/**
 * @file  panic.h
 * @brief Unrecoverable error reporting.
 */

#include <arch/context.h>

#include <kernel/syscall.h>

#include <lib/compiler.h>
#include <lib/types.h>

/** Panic syscall handler. */
SYSCALL_VOID(_sys_panic);

/**
 * Halt execution due to a severe software-detected issue.
 * @param msg Format string.
 * @param ... Optional arguments.
 *
 * Call this only if your code detects a serious non-recoverable error. Provide
 * as much detail in the message as possible to allow for easier debugging.
 *
 * This function does not return. It hangs the system.
 */
static inline void _Noreturn __printfmt(1, 2) panic(const char *msg, ...)
{
	// TODO: snprintf to a buffer, pass it as msg

	sys_panic(msg);
	__builtin_unreachable();
}

void _Noreturn _panic(struct ctx_full *frame, const char *msg);
