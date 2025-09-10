/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#pragma once

#include <lib/_printfmt.h>
#include <lib/types.h>

#define __PFMT_FORMATTER(_name)            \
	err_t _name(struct _pfmt_fe *fe,   \
	    va_list                 *args, \
	    const char              *beg,  \
	    const char              *end)

err_t __pfmt_puts(struct _pfmt_fe *fe, const char *buf);

/** Reverse buffer. */
char *__pfmt_reverse(char *buf, size_t elems);

#define __pfmt_putsrev(_fe, _buf, _len, _elems) \
	__pfmt_puts((_fe), __pfmt_reverse((_buf), (_elems)))

// Format processor declarations.
__PFMT_FORMATTER(__pfmt_proc_X);
__PFMT_FORMATTER(__pfmt_proc_d);
__PFMT_FORMATTER(__pfmt_proc_p);
__PFMT_FORMATTER(__pfmt_proc_s);
__PFMT_FORMATTER(__pfmt_proc_u);
__PFMT_FORMATTER(__pfmt_proc_x);
