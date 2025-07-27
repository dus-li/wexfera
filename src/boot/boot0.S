/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#include <board.h>

#include <asm/helpers.h>

#include <arch/scs.h>

/// @fn    void boot0(void)
/// @brief Entry point and reset handler.
///
/// This function performs initial, pre-HLL system configuration. It populates
/// memory sections with proper values, enables the FPU and branches to
/// high-level language.
BEGIN_GLOBAL_FUNCTION(boot0)
	// Set up the stack pointer
	LDR	r0, =LD_STACK_END
	MOV	sp, r0

	// Disable interrupts until relocation
	CPSID	i

	// Populate .BSS with zeros
	MOV	r0, #0
	LDR	r1, =LD_BSS_BEG
	LDR	r2, =LD_BSS_END
bss:	STR	r0, [r1], #4
	CMP	r1, r2
	BLT	bss

	// Copy .DATA from flash to SRAM
	LDR	r0, =LD_DATA_LMA_BEG
	LDR	r1, =LD_DATA_BEG
	LDR	r2, =LD_DATA_END
data:	LDR	r3, [r0], #4
	STR	r3, [r1], #4
	CMP	r1, r2
	BLT	data

#if (BOARD_HAS_FPU == 1)
	// Enable the FPU
	LDR	r0, =SCS_SCB_CPACR
	LDR	r1, [r0]
	ORR	r1, r1, #SCS_SCB_CPACR_FPU_FULL
	STR	r1, [r0]
	DSB
	ISB
#endif

	// Branch to HLL
	BL	boot1
END_FUNCTION(boot0)
