/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#pragma once

/**
 * @file  fault.h
 * @brief Fault-related declarations and constants.
 */

#include <arch/context.h>

#include <lib/compiler.h>
#include <lib/preproc.h>
#include <lib/types.h>

typedef void (*fault_hook_t)(const struct ctx_full *);

#define __FAULT_HOOKS_SEC ".data.fault.hooks"

#define FAULT_HOOK(_fn)                                       \
	_Static_assert(same_type(typeof(&_fn), fault_hook_t), \
	    "Incorrect signature for a fault hook.");         \
	const fault_hook_t __section(__FAULT_HOOKS_SEC)       \
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
