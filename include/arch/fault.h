/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#pragma once

/**
 * @file  fault.h
 * @brief Fault-related declarations and constants.
 */

#include <lib/check.h>
#include <lib/compiler.h>
#include <lib/preproc.h>
#include <lib/types.h>

#define FAULT_GPRS_NO (13)

struct fault_regs {
	u32 r[FAULT_GPRS_NO];
	u32 lr;
	u32 sp;
	u32 pc;
	u32 psr;
	u32 primask;
	u32 control;
};

typedef void (*fault_hook_t)(const struct fault_regs *);

#define __FAULT_HOOKS_SEC ".data.fault.hooks"

#define FAULT_HOOK(_fn)                                         \
	CHECK_BUILDTIME(same_type(typeof(&_fn), fault_hook_t)); \
	const fault_hook_t __section(__FAULT_HOOKS_SEC)         \
	    CONCAT(__fault_hook, __LINE__) = (_fn);

/**
 * Enable faults.
 *
 * By default only HardFault is enabled on ARMv7m. The three remaining fault
 * types must be explicitly enabled by software in order for them to be
 * generated. These are MemManage fault, BusFault and UsageFault. This function
 * is responsible for enabling them.
 */
void fault_enable(void);
