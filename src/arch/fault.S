/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#include <asm/helpers.h>

#define PRIMASK_PM (1)

.include "asm/irq.S"

BEGIN_GLOBAL_FUNCTION(__fault_dispatcher)
	// Check which stack pointer was active during the fault. Put it in R0.
	IRQSAVESP r0

	// Jump to HLL
	B	fault_dispatcher
END_FUNCTION(__fault_dispatcher)

BEGIN_GLOBAL_FUNCTION(__fault_regs_collect)
	// Save GPRs
	STMIA	r0, {r0-r12}

	// PRIMASK and CONTROL system registers
	MRS	r1, primask
	STR	r1, [r0, #0x48]
	MRS	r1, control
	STR	r1, [r0, #0x4C]

	BX	lr
END_FUNCTION(__fault_regs_collect)
