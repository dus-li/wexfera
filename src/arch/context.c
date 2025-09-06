/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#include <arch/context.h>

extern void __ctx_collect(struct ctx_full *out);

void ctx_collect(struct ctx_full *out, struct irq_stacked *frame)
{
	__ctx_collect(out);

	out->r[0]  = frame->r0;
	out->r[1]  = frame->r1;
	out->r[2]  = frame->r2;
	out->r[3]  = frame->r3;
	out->r[12] = frame->r12;
	out->lr    = frame->lr;
	out->pc    = frame->pc;
	out->psr   = frame->psr;
}
