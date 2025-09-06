/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#define LOG_FMT(_fmt) _fmt

#include <arch/context.h>

#define FAULT_LINE_LEN (66)

extern void __ctx_collect(struct ctx_full *out);

static void _ctx_log_sys(struct ctx_full *full)
{
	log_always("PRIMASK: %s\n", full->primask & 1U ? "Yes" : "No");
	log_always("FPCA:    %s\n", full->control & (1U << 2) ? "Yes" : "No");
	log_always("nPRIV:   %sriviledged\n", full->control & 1 ? "Unp" : "P");

	log_always("Flags:   ");
	log_always("%s", full->psr & (1U << 31) ? "N" : "");
	log_always("%s", full->psr & (1U << 30) ? "Z" : "");
	log_always("%s", full->psr & (1U << 29) ? "C" : "");
	log_always("%s", full->psr & (1U << 28) ? "V" : "");
	log_always("%s", full->psr & (1U << 27) ? "Q" : "");
	log_always("\n");
}

/** Print a separator line of length equal to the register dump. */
static void _ctx_log_sep(void)
{
	for (int i = 0; i < FAULT_LINE_LEN; ++i)
		log_always("-");

	log_always("\n");
}

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

void ctx_log(enum log_levels lvl, struct ctx_full *frame)
{
	u32 *gprs = frame->r;

	_ctx_log_sep();
	_ctx_log_sys(frame);
	_ctx_log_sep();

	log_always("PC was %#10X\n", frame->pc);
	log_always("LR was %#10X\n", frame->lr);
	log_always("SP was %#10X\n", frame->sp);
	_ctx_log_sep();

	// Log GPR values
	for (unsigned i = 0; i < CTX_GPRS_NO; ++i) {
		log_always("R%u:%s%#10X  ", i, i >= 10 ? " " : "  ", gprs[i]);
		if ((i % 4) == 3)
			log_always("\n");
	}
	log_always("\n");
	_ctx_log_sep();
}
