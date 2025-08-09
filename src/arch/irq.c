/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#include <board.h>

#include <arch/irq.h>
#include <arch/scs.h>

#include <lib/compiler.h>
#include <lib/reg.h>
#include <lib/round.h>
#include <lib/string.h>

#define IRQ_CORE_NUMBER  (16)
#define IRQ_NVIC_MAX_IRQ (496)
#define IRQ_NUMBER       (ROUND_UP(BOARD_IRQ_NUMBER, 128))

/**
 * Dynamically assignable vector table.
 * @see irq_relocate
 */
static irq_handler_t _irq_dynvtab[IRQ_NUMBER] __section(".bss.dynvtab");

extern const u32 LD_COREVTAB_BEG;
extern const u32 LD_COREVTAB_END;

/**
 * Default IRQ handler.
 */
void __used _irq_def_handler(void)
{
	// TODO: Panic for unregistered interrupt. Use IPSR to get IRQ number.

	asm volatile("bkpt #0");
	for (;;)
		;
}

void irq_relocate(void)
{
	const void *core_beg = &LD_COREVTAB_BEG;
	const void *core_end = &LD_COREVTAB_END;
	size_t      len      = core_end - core_beg;
	u32         addr;

	// ARMv7m defines first 16 fields of the vector tab.
	_Static_assert(IRQ_NUMBER >= IRQ_CORE_NUMBER,
	    "Number of board Irq must not be smaller than 16");

	_Static_assert(IRQ_NUMBER <= IRQ_NVIC_MAX_IRQ,
	    "Number of board Irq must not exceed NVIC maximum");

	// Keep core interrupts, assign default to the rest.
	memcpy(_irq_dynvtab, core_beg, len);
	for (int i = IRQ_CORE_NUMBER; i < IRQ_NUMBER; ++i)
		_irq_dynvtab[i] = _irq_def_handler;

	addr = ((u32)_irq_dynvtab) & 0xFFFFFF80;
	write32(addr, SCS_SCB_VTOR);
}
