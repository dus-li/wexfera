/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#include <lib/_printfmt.h>
#include <lib/preproc.h>

#include "__pfmt_proc_funcs.h"

#define SPECIFIER 'x'
#define OPTS(X)      \
	X(zero, '0') \
	X(alt, '#')

// This uses SPECIFIER and OPTS
#include "__pfmt_fmt_maker.h"

/**
 * Largest possible number of characters needed to represent a value.
 *
 * Since we do not support length modifiers (h, l, etc.) we are always working
 * with u32 values. Therefore we need ⌈log16(2^32)⌉ = 8 characters. This, plus
 * one for NUL terminating character forms base size for unmodified format
 * specifier. Moreover, we need to add 2 characters in case '#' option is used.
 */
#define MAX_CHARS (8 + 1 + 2)

static const char lo_set[] = "0123456789abcdef";
static const char hi_set[] = "0123456789ABCDEF";

struct params {
	const char     *beg;
	const char     *end;
	const char     *set;
	struct options *opts;
	int             width;
};

err_t ___pfmt_proc_x(struct _pfmt_fe *fe, u32 val, struct params *params)
{
	char buf[MAX_CHARS] = { 0, /**/ };
	u32  i;

	for (i = 0; i < MAX_CHARS - 1; ++i) {
		if (!params->opts->zero && val == 0)
			break;

		buf[i]   = params->set[val & 0xF];
		val    >>= 4;
	}

	if (i == 0)
		buf[i++] = '0';

	if (params->opts->alt) {
		buf[i++] = 'x';
		buf[i++] = '0';
	}

	if (params->width != 0) {
		while (i < params->width || i < MAX_CHARS - 1)
			buf[i++] = ' ';
	}

	return __pfmt_putsrev(fe, buf, MAX_CHARS, i);
}

err_t setup(struct params *params, struct options *opts, const char *beg,
    const char *end)
{
	int ret = get_options(opts, beg, end);
	if (ret < 0)
		return ret;

	params->beg   = beg;
	params->end   = end;
	params->set   = lo_set;
	params->opts  = opts;
	params->width = ret;

	return ERR_NONE;
}

err_t __pfmt_proc_x(struct _pfmt_fe *fe, va_list *args, const char *beg,
    const char *end)
{
	struct params  params;
	struct options opts;

	return setup(&params, &opts, beg, end) ?:
	    ___pfmt_proc_x(fe, va_arg(*args, u32), &params);
}

err_t __pfmt_proc_X(struct _pfmt_fe *fe, va_list *args, const char *beg,
    const char *end)
{
	struct params  params;
	struct options opts;

	err_t ret = setup(&params, &opts, beg, end);
	if (ret != ERR_NONE)
		return ret;

	params.set = hi_set;

	return ___pfmt_proc_x(fe, va_arg(*args, u32), &params);
}

err_t __pfmt_proc_p(struct _pfmt_fe *fe, va_list *args, const char *beg,
    const char *end)
{
	struct params  params;
	struct options opts;

	err_t ret = setup(&params, &opts, beg, end);
	if (ret != ERR_NONE)
		return ret;

	opts.alt = true;

	return ___pfmt_proc_x(fe, va_arg(*args, u32), &params);
}
