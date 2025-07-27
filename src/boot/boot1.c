/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#include <arch/fault.h>
#include <arch/irq.h>

/**
 * Configure core system peripherals and platforms, start scheduler.
 *
 * This function is branched into from @a boot0, which is implemented in
 * Assembly and is responsible for initializing memory and enabling FPU.
 *
 * This function configures faults, relocates IRQ vector table and
 * @todo Clock configuration
 * @todo USART configuration
 */
void _Noreturn boot1(void)
{
	fault_enable();
	irq_relocate();
	irq_enable();

	for (;;)
		;

	__builtin_unreachable();
}
