/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#define LOG_FMT(_fmt) _fmt

#include <arch/fault.h>
#include <arch/irq.h>
#include <arch/scs.h>

#include <lib/array_size.h>
#include <lib/compiler.h>
#include <lib/log.h>
#include <lib/reg.h>

#define FAULT_GPRS_NO (13)

#define FAULT_ENABLE_BITS                                        \
	(SCS_SCB_SHCSR_USGFAULTENA | SCS_SCB_SHCSR_BUSFAULTENA | \
	    SCS_SCB_SHCSR_MEMFAULTENA)

struct fault_stacked {
	u32 r0, r1, r2, r3, r12, lr, pc, psr;
};

struct fault_regs {
	u32 r[FAULT_GPRS_NO];
	u32 lr;
	u32 sp;
	u32 pc;
	u32 psr;
	u32 primask;
	u32 control;
};

void fault_enable(void)
{
	union32(FAULT_ENABLE_BITS, SCS_SCB_SHCSR);
}

extern void __fault_regs_collect(struct fault_regs *out);

/**
 * Collect register values from the time of the fault.
 * @param out Output structure.
 * @param in  Stacked registers pushed upon fault trigger.
 *
 * This function collects current values of relevant registers and then
 * overwrites those, that are stacked to construct a complete picture of the
 * fault-time situation.
 */
static void fault_regs_collect(struct fault_regs *out, struct fault_stacked *in)
{
	__fault_regs_collect(out);

	out->r[0]  = in->r0;
	out->r[1]  = in->r1;
	out->r[2]  = in->r2;
	out->r[3]  = in->r3;
	out->r[12] = in->r12;
	out->lr    = in->lr;
	out->pc    = in->pc;
	out->psr   = in->psr;
}

/** Mapping between fault constants and names of corresponding fault types. */
static const char *const _fault_name_map[] = {
	[FAULT_HARDFAULT]  = "HardFault",
	[FAULT_MEMMANAGE]  = "MemManage Fault",
	[FAULT_BUSFAULT]   = "BusFault",
	[FAULT_USAGEFAULT] = "UsageFault",
};

/** Print a separator line of length equal to the register dump. */
static void _fault_log_sep(void)
{
	for (int i = 0; i < 66; ++i)
		log_always("-");

	log_always("\n");
}

/**
 * Handle a fault.
 * @param regs Register values of the offending process.
 * @param type Type of a fault. One of constants from @ref faultConstants.
 *
 * @see faultConstants
 *
 * @todo Interpret fault registers
 */
void fault_dispatcher(struct fault_stacked *regs, unsigned type)
{
	struct fault_regs full;

	irq_disable();
	fault_regs_collect(&full, regs);

	log_always("\nPanic!\n");
	_fault_log_sep();

	log_always("Issue:   %s\n", _fault_name_map[type]);
	log_always("PRIMASK: %s\n", full.primask & 1U ? "Yes" : "No");
	log_always("FPCA:    %s\n", full.control & (1U << 2) ? "Yes" : "No");
	log_always("nPRIV:   %sriviledged\n", full.control & 1 ? "Unp" : "P");

	log_always("Flags:   ");
	log_always("%s", full.psr & (1U << 31) ? "N" : "");
	log_always("%s", full.psr & (1U << 30) ? "Z" : "");
	log_always("%s", full.psr & (1U << 29) ? "C" : "");
	log_always("%s", full.psr & (1U << 28) ? "V" : "");
	log_always("%s", full.psr & (1U << 27) ? "Q" : "");
	log_always("\n");
	_fault_log_sep();

	log_always("PC was %#10X\n", full.pc);
	log_always("LR was %#10X\n", full.lr);
	_fault_log_sep();

	for (unsigned i = 0; i < FAULT_GPRS_NO; ++i) {
		log_always("R%u:%s%#10X  ", i, i >= 10 ? " " : "  ", full.r[i]);
		if ((i % 4) == 3)
			log_always("\n");
	}

	log_always("\n");
	_fault_log_sep();

	asm volatile("bkpt #0");
	for (;;)
		;

	__builtin_unreachable();
}
