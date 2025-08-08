/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#include <lib/compiler.h>
#include <lib/types.h>

#include <boot/sequence.h>

extern void *const LD_DATA_SEQ_EARLY_BEG;
extern void *const LD_DATA_SEQ_EARLY_END;

/**
 * Used for enforcing creation of all sequence ELF sections.
 */
void __used __seq_dummy(void)
{
	// Intentionally empty
}
SEQ_EARLY_INIT(__seq_dummy);

void seq_run_early_init(void)
{
	const seq_func_t *fns  = (seq_func_t *)&LD_DATA_SEQ_EARLY_BEG;
	const seq_func_t *fnse = (seq_func_t *)&LD_DATA_SEQ_EARLY_END;

	u32 count  = (u32)fnse - (u32)fns;
	count     /= sizeof(seq_func_t);

	for (u32 i = 0; i < count; ++i)
		fns[i]();
}
