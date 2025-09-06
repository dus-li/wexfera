/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#pragma once

/**
 * @file  context.h
 * @brief CPU context.
 */

#include <arch/irq.h>

#define CTX_GPRS_NO (13)

struct ctx_full {
	u32 r[CTX_GPRS_NO];
	u32 lr;
	u32 sp;
	u32 pc;
	u32 psr;
	u32 primask;
	u32 control;
};

/**
 * Assemble a full context from a stacked frame.
 * @param out   Pointer to a context that is to be filled by the function.
 * @param frame Frame stacked on IRQ entry by the hardware.
 */
void ctx_collect(struct ctx_full *out, struct irq_stacked *frame);
