/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#include <lib/_printfmt.h>

#include "__pfmt_proc_funcs.h"

#define SPECIFIER 'u'
#define OPTS(X)        \
	X(group, '\'') \
	X(zero, '0')

// This uses SPECIFIER and OPTS
#include "__pfmt_fmt_maker.h"

/**
 * Largest possible number of characters needed to represent a value.
 *
 * Since we do not support length modifiers (h, l, etc.) we are always working
 * with u32 values. Therefore we need ⌈log10(2^32)⌉ = 10 characters. This, plus
 * one for NUL terminating character forms base size for unmodified format
 * specifier.
 *
 * We will, however need to add 3 more characters for separators from the "'"
 * option.
 */
#define MAX_CHARS (10 + 1 + 4)

err_t __pfmt_proc_u(struct _pfmt_fe *fe, va_list *args, const char *beg,
    const char *end)
{
	char buf[MAX_CHARS] = { 0, /**/ };

	struct options opts;

	u32   i, val;
	err_t ret;

	ret = get_options(&opts, beg, end);
	if (ret != ERR_NONE)
		return ret;

	val = va_arg(*args, u32);
	for (i = 0; i < MAX_CHARS - 1; ++i) {
		if (opts.group && (i == 3 || i == 7 || i == 11))
			buf[i++] = ',';

		if (!opts.zero && val == 0)
			break;

		buf[i]  = '0' + (val % 10);
		val    /= 10;
	}

	if (i == 0)
		buf[i++] = '0';

	return __pfmt_putsrev(fe, buf, MAX_CHARS, i);
}
