/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#include <lib/_printfmt.h>
#include <lib/array_size.h>

#include "__pfmt_proc_funcs.h"

/** Mapping of format specifiers to their processor functions. */
static const struct {
	char spec;
	err_t (*proc)(struct _pfmt_fe *, va_list *, const char *, const char *);
} __pfmt_specifier_map[] = {
	{ 'X', __pfmt_proc_X },
	{ 'd', __pfmt_proc_d },
	{ 'p', __pfmt_proc_p },
	{ 'u', __pfmt_proc_u },
	{ 'x', __pfmt_proc_x },
	{ 's', __pfmt_proc_s },
};

static inline int __pfmt_getspec(char c)
{
	for (int i = 0; i < ARRAY_SIZE(__pfmt_specifier_map); ++i) {
		if (__pfmt_specifier_map[i].spec == c)
			return i;
	}

	return -1;
}

err_t __printfmt(2, 3) _pfmt_proc(struct _pfmt_fe *fe, const char *fmt, ...)
{
	va_list args;
	err_t   ret;

	if (fmt == NULL || fe == NULL || fe->writer == NULL)
		return -ERR_ARG;

	va_start(args, fmt);
	ret = _pfmt_vproc(fe, fmt, &args);
	va_end(args);

	return ret;
}

#define __pfmt_putc(_ret, _fe, _c) \
	_ret = _ret ?: (_fe)->writer((_c), (_fe)->state)

err_t __pfmt_puts(struct _pfmt_fe *fe, const char *buf)
{
	err_t ret;

	if (buf == NULL || fe == NULL)
		return -ERR_ARG;

	for (size_t i = 0; buf[i] != '\0'; ++i) {
		ret = fe->writer(buf[i], fe->state);
		if (ret != ERR_NONE)
			return ret;
	}

	return ERR_NONE;
}

char *__pfmt_reverse(char *buf, size_t elems)
{
	u32 tmp, end = elems - 1;

	for (size_t i = 0; i < elems / 2; ++i) {
		tmp          = buf[i];
		buf[i]       = buf[end - i];
		buf[end - i] = tmp;
	}

	return buf;
}

err_t _pfmt_vproc(struct _pfmt_fe *fe, const char *fmt, va_list *args)
{
	const char *start;
	err_t       ret = ERR_NONE;
	int         spec;
	char        cur;

	if (fmt == NULL || fe == NULL || fe->writer == NULL)
		return -ERR_ARG;

	for (cur = *fmt; cur != '\0'; cur = *++fmt) {
		if (ret != ERR_NONE)
			return ret;

		// Characters that are not type specifiers are simply forwarded.
		if (cur != '%') {
			__pfmt_putc(ret, fe, cur);
			continue;
		}

		// Locate bounds of type specifier.
		start = fmt + 1;
		while ((spec = __pfmt_getspec(*++fmt)) == -1) {
			if (*fmt == NULL)
				return -ERR_ARG;
		}

		ret = __pfmt_specifier_map[spec].proc(fe, args, start, fmt);
	}

	return ERR_NONE;
}
