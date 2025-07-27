/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#include <board.h>

#include <arch/scs.h>
#include <arch/timer.h>

#include <lib/compiler.h>
#include <lib/reg.h>

#define RELOAD_VALUE (BOARD_HZ / TIMER_HZ - 1)

#define CSR_FLAGS \
	(SCS_SYST_CSR_ENABLE | SCS_SYST_CSR_TICKINT | SCS_SYST_CSR_CLKSOURCE)

static u32 timer_count;

void __used __systick_isr(void)
{
	timer_count++;
}

void timer_configure(void)
{
	_Static_assert(RELOAD_VALUE < (1U << 24),
	    "Reload value must fit in 24 bits.");

	write32(RELOAD_VALUE, SCS_SYST_RVR);
	write32(0, SCS_SYST_CVR);
	write32(CSR_FLAGS, SCS_SYST_CSR);
}
