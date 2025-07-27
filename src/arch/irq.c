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

extern void *const LD_COREVTAB_BEG;
extern void *const LD_COREVTAB_END;
#define IRQ_COREVTAB_SIZE (LD_COREVTAB_END - LD_COREVTAB_BEG)

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
	// ARMv7m defines first 16 fields of the vector tab.
	_Static_assert(IRQ_NUMBER >= IRQ_CORE_NUMBER,
	    "Number of board Irq must not be smaller than 16");

	_Static_assert(IRQ_NUMBER <= IRQ_NVIC_MAX_IRQ,
	    "Number of board Irq must not exceed NVIC maximum");

	// Keep core interrupts, assign default to the rest.
	memcpy(_irq_dynvtab, LD_COREVTAB_BEG, IRQ_COREVTAB_SIZE);
	for (int i = IRQ_CORE_NUMBER; i < IRQ_NUMBER; ++i)
		_irq_dynvtab[i] = _irq_def_handler;

	write32(_irq_dynvtab, SCS_SCB_VTOR);
}
