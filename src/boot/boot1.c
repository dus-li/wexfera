/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#define LOG_FMT(_fmt) _fmt

#include <board.h>

#include <arch/fault.h>
#include <arch/irq.h>
#include <arch/timer.h>

#include <boot/sequence.h>

#include <kernel/alloc.h>

#include <lib/log.h>

#if !defined(BUILD_VERSION)
  #define BUILD_VERSION (unknown)
#endif

#define VERSION STRINGIFY(BUILD_VERSION) " (build " __DATE__ " " __TIME__ ")"

/**
 * Configure core system peripherals and platforms, start scheduler.
 *
 * This function is branched into from @a boot0, which is implemented in
 * Assembly and is responsible for initializing memory and enabling FPU.
 *
 * This function configures faults, relocates IRQ vector table and configures
 * the board clock.
 */
void _Noreturn boot1(void)
{
	fault_enable();
	irq_relocate();
	irq_enable();

	board_init_clock();
	seq_run_early_init();

	log_always("\nThis is Wexfera %s\n", VERSION);

	timer_configure();
	alloc_init();

	// TODO: Enter scheduler.

	for (;;)
		;

	__builtin_unreachable();
}
