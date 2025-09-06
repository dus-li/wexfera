/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#include <arch/irq.h>

#include <kernel/panic.h>
#include <kernel/syscall.h>

#include <lib/array_size.h>
#include <lib/compiler.h>
#include <lib/log.h>
#include <lib/types.h>

typedef void (*_sys_void)(struct sys_args *args);
typedef u32 (*_sys_nonvoid)(struct sys_args *args);

static void __sys_call_void(_sys_void fn, struct sys_args *args)
{
	fn(args);
}

static void __sys_call_nonvoid(_sys_nonvoid fn, struct sys_args *args)
{
	args->frame->r0 = fn(args);
}

#define _sys_call(_handler)             \
	_Generic((_handler),            \
	    _sys_void: __sys_call_void, \
	    _sys_nonvoid: __sys_call_nonvoid)(_handler, &args)

#define _sys_caseify(_svcno, _handler, ...) \
	case _SYS_NUMBER(_svcno):           \
		_sys_call(_handler);        \
		break;

void __used svcall_dispatcher(struct irq_stacked *regs, u32 svcno)
{
	if (svcno >= _SYS_MAX) {
		log_error("Unrecognized syscall: %u\n", svcno);
		return;
	}

	struct sys_args args = { /**/
		.frame = regs,
		.args  = { regs->r0, regs->r1, regs->r2, regs->r3 }
	};

	switch (svcno) {
		SYSCALLS(_sys_caseify)
	}
}
