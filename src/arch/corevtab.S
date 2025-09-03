/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#include <asm/helpers.h>

// See: arch/irq.c
.macro default
	.word	_irq_def_handler
.endmacro

SECTION(.vtab, SHF_ALLOC, SHT_PROGBITS)
BEGIN_GLOBAL_OBJECT(__corevtab)
	.word	LD_STACK_END
	.word	boot0

#if (BOARD_NMI_IS_WDG == 1)
	.word	wdg_expired
#else
	default
#endif

	.word	__fault_dispatcher // HardFault
	.word	__fault_dispatcher // MemManage
	.word	__fault_dispatcher // BusFault
	.word	__fault_dispatcher // UsageFault

	// Reserved 7-10
	.rept	4
	.word	0
	.endr

	.word	__svcall_dispatcher // SVCall
	default // DebugMonitor

	// Reserved 13
	.word	0

	default // PendSV
	.word	__systick_isr
END_OBJECT(__corevtab)
