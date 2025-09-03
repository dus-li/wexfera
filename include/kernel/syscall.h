/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#pragma once

/**
 * @file  syscall.h
 * @brief Supervisor calls.
 */

#include <arch/irq.h>

#include <lib/compiler.h>
#include <lib/preproc.h>

/** Arguments passed to syscall handler functions. */
struct sys_args {
	struct irq_stacked *frame;   ///< Stack frame of the caller.
	u32                 args[4]; ///< Arguments extracted from the frame.
};

/**
 * Declare a syscall handler that does not return anything.
 * @param _name Name of the syscall handler.
 */
#define SYSCALL_VOID(_name) void _name(struct sys_args *__syscall_args)

/**
 * Declare a syscall that returns something.
 * @param _name Name of the syscall handler.
 *
 * Note that all returned values must be cast to u32. The syscall wrapper may
 * then cast them back into a proper value should the return type be something
 * else.
 */
#define SYSCALL_NONVOID(_name) u32 _name(struct sys_args *__syscall_args)

/** Get arguments inside a syscall handler. */
#define SYSCALL_ARGS __syscall_args->args

/** Get caller stack frame inside a syscall handler. */
#define SYSCALL_FRAME __syscall_args->frame

/** List of syscalls. */
#define SYSCALLS(X)

#define _SYS_NUMBER(_name) CONCAT(SYS_, _name)

#define _sys_declare_svcall(_svcno, _handler, _name, _ret, _args...) \
	_Static_assert(VA_ARGS_COUNT(_args) <= 4,                    \
	    "Too many arguments for syscall " #_svcno);              \
	static inline __used _ret CONCAT(sys_, _name)(_args)         \
	{                                                            \
		register u32 r0 asm("r0");                           \
		asm volatile("svc %[imm]\n\t"                        \
		    : "=r"(r0)                                       \
		    : [imm] "I"(_SYS_NUMBER(_svcno))                 \
		    : "memory");                                     \
		if (sizeof(_ret))                                    \
			return (_ret)r0;                             \
	}

#define _sys_enumify(_name, ...) _SYS_NUMBER(_name),

// Generate syscall numbers.
enum sys_numbers { SYSCALLS(_sys_enumify) _SYS_MAX };

// Generate syscall functions.
SYSCALLS(_sys_declare_svcall)
