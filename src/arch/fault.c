/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#include <arch/fault.h>
#include <arch/scs.h>

#include <lib/compiler.h>
#include <lib/reg.h>

#define FAULT_ENABLE_BITS                                        \
	(SCS_SCB_SHCSR_USGFAULTENA | SCS_SCB_SHCSR_BUSFAULTENA | \
	    SCS_SCB_SHCSR_MEMFAULTENA)

struct fault_stacked {
	u32 r0, r1, r2, r3, r12, lr, pc, psr;
};

void fault_enable(void)
{
	write32(FAULT_ENABLE_BITS, SCS_SCB_SHCSR);
}

/**
 * Handle a fault.
 * @param regs Registers values of the offending process.
 * @param type Type of a fault. One of constants from @ref faultConstants.
 *
 * @see faultConstants
 */
void fault_dispatcher(struct fault_stacked regs, int type)
{
	// TODO: Print debug info

	asm volatile("bkpt #0");
	for (;;)
		;
}
