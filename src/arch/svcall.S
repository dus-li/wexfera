/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#include <asm/helpers.h>

.include "asm/irq.S"

BEGIN_GLOBAL_FUNCTION(__svcall_dispatcher)
	// Save stack pointer in R0
	IRQSAVESP r0

	// Get address of the SVC instruction from stacked PC
	LDR	r2, [r0, #24]
	SUB	r2, r2, #2

	// Read imm8 from the instruction and save in R1
	LDRH	r1, [r2]
	UXTH	r1, r1
	AND	r1, r1, #0xFF

	// Branch to HLL dispatcher
	B	svcall_dispatcher
END_FUNCTION(__svcall_dispatcher)
