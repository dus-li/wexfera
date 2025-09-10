/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#if !defined(OPTS) || !defined(SPECIFIER)
  #error Format maker header requires definition of OPTS and SPECIFIER
#endif

#include <lib/error.h>
#include <lib/string.h>
#include <lib/types.h>

#define __pfmt_fmt_maker_bitfieldify(_name, ...) bool _name : 1;

struct options {
	OPTS(__pfmt_fmt_maker_bitfieldify)
};

/**
 * Process format specifier options.
 * @param ret Return struct to populate with options set.
 * @param beg Start of the format specifier string.
 * @param end Last character of the format specifier string.
 */
static int get_options(struct options *ret, const char *beg, const char *end)
{
#define __pfmt_fmt_maker_make_case(_name, _char) \
	case (_char):                            \
		ret->_name = true;               \
		break;

	const char *cur;
	int         acc    = 0;
	bool        number = false;

	bzero(ret, sizeof(*ret));

	for (cur = beg; cur != end && *cur != SPECIFIER; ++cur) {
		if (number || (isdigit(*cur) && *cur != '0')) {
			acc    *= 10;
			acc    += *cur - '0';
			number  = true;
			continue;
		}

		number = false;
		switch (*cur) {
			OPTS(__pfmt_fmt_maker_make_case)
		default:
			return -ERR_ARG;
		}
	}

	return acc;
}
