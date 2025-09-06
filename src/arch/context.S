/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#include <asm/helpers.h>

BEGIN_GLOBAL_FUNCTION(__ctx_collect)
	// Save GPRs
	STMIA	r0, {r0-r12}

	// PRIMASK and CONTROL system registers
	MRS	r1, primask
	STR	r1, [r0, #0x48]
	MRS	r1, control
	STR	r1, [r0, #0x4C]

	BX	lr
END_FUNCTION(__ctx_collect)
