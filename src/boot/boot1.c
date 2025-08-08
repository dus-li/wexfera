/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#include <board.h>

#include <arch/fault.h>
#include <arch/irq.h>
#include <arch/timer.h>

#include <boot/sequence.h>

/**
 * Configure core system peripherals and platforms, start scheduler.
 *
 * This function is branched into from @a boot0, which is implemented in
 * Assembly and is responsible for initializing memory and enabling FPU.
 *
 * This function configures faults, relocates IRQ vector table and configures
 * the board clock.
 *
 * @todo USART configuration
 * @todo SysTick configuration
 */
void _Noreturn boot1(void)
{
	fault_enable();
	irq_relocate();
	irq_enable();

	board_init_clock();
	seq_run_early_init();

	board_log_be('\r');
	board_log_be('\n');
	board_log_be('W');
	board_log_be('e');
	board_log_be('x');
	board_log_be('f');
	board_log_be('e');
	board_log_be('r');
	board_log_be('a');
	board_log_be('\r');
	board_log_be('\n');

	// TODO: Set up logging.

	timer_configure();

	// TODO: Enter scheduler.

	for (;;)
		;

	__builtin_unreachable();
}
