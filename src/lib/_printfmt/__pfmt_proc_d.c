/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#include <lib/_printfmt.h>

#include "__pfmt_proc_funcs.h"

#define SPECIFIER 'd'
#define OPTS(X)        \
	X(group, '\'') \
	X(sign, '+')   \
	X(zero, '0')

// This uses SPECIFIER and OPTS
#include "__pfmt_fmt_maker.h"

/**
 * Largest possible number of characters needed to represent a value.
 *
 * Since we do not support length modifiers (h, l, etc.) we are always working
 * with i32 values. Therefore we need ⌈log10(2^31)⌉ = 10 characters. This, plus
 * one for NUL terminating character forms base size for unmodified format
 * specifier.
 *
 * We also need to reserve one character for the sign and add 3 more characters
 * for separators from the "'" option.
 */
#define MAX_CHARS (10 + 1 + 1 + 4)

err_t __pfmt_proc_d(struct _pfmt_fe *fe, va_list *args, const char *beg,
    const char *end)
{
	char buf[MAX_CHARS] = { 0, /**/ };

	struct options opts;

	u32   i;
	i32   val;
	err_t ret;
	bool  negative = false;

	ret = get_options(&opts, beg, end);
	if (ret != ERR_NONE)
		return ret;

	val = va_arg(*args, i32);
	if (val < 0) {
		val      = -val;
		negative = true;
	}

	for (i = 0; i < MAX_CHARS - 2; ++i) {
		if (opts.group && (i == 3 || i == 7 || i == 11))
			buf[i++] = ',';

		if (!opts.zero && val == 0)
			break;

		buf[i]  = '0' + (val % 10);
		val    /= 10;
	}

	if (i == 0)
		buf[i++] = '0';

	if (opts.sign || negative)
		buf[i++] = negative ? '-' : '+';

	return __pfmt_putsrev(fe, buf, MAX_CHARS, i);
}
