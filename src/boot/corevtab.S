/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#include <asm/helpers.h>

// See: arch/irq.c
.macro default
	.word	_irq_def_handler
.endmacro

SECTION(.corevtab, SHF_ALLOC, SHT_PROGBITS)
BEGIN_GLOBAL_OBJECT(__corevtab)
	.word	LD_STACK_END
	.word	boot0

#if (BOARD_NMI_IS_WDG == 1)
	.word	wdg_expired
#else
	default
#endif

	.word	__fault_hardfault
	.word	__fault_memmanage
	.word	__fault_busfault
	.word	__fault_usagefault

	// Reserved 7-10
	.rept	4
	.word	0
	.endr

	default // SVCall
	default // DebugMonitor

	// Reserved 13
	.word	0

	default // PendSV
	default // SysTick
END_OBJECT(__corevtab)
