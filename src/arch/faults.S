/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#include <asm/helpers.h>
#include <arch/fault.h>

// Check which stack pointer was active during the fault and put it in reg.
.macro SPSAVE reg
	TST	lr, #4
	ITE	eq
	MRSEQ	\reg, msp
	MRSNE	\reg, psp
.endmacro

// Branch to dispatcher
.macro BDISP constant
	SPSAVE	r0
	MOV	r1, \constant
	B	fault_dispatcher
.endmacro

BEGIN_GLOBAL_FUNCTION(__fault_hardfault)
	BDISP	#FAULT_HARDFAULT
END_FUNCTION(__fault_hardfault)

BEGIN_GLOBAL_FUNCTION(__fault_memmanage)
	BDISP	#FAULT_MEMMANAGE
END_FUNCTION(__fault_memmanage)

BEGIN_GLOBAL_FUNCTION(__fault_busfault)
	BDISP	#FAULT_BUSFAULT
END_FUNCTION(__fault_busfault)

BEGIN_GLOBAL_FUNCTION(__fault_usagefault)
	BDISP	#FAULT_USAGEFAULT
END_FUNCTION(__fault_usagefault)

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
