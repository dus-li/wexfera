/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#include <lib/_printfmt.h>

#include "__pfmt_proc_funcs.h"

err_t __pfmt_proc_s(struct _pfmt_fe *fe, va_list *args, const char *beg,
    const char *end)
{
	const char *s = va_arg(*args, const char *);

	return __pfmt_puts(fe, s ?: "(null)");
}
