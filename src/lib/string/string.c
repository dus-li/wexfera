/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#include <lib/_printfmt.h>
#include <lib/extrema.h>
#include <lib/string.h>

struct snprintf_state {
	char  *cur; ///< Current position in the output string.
	size_t rem; ///< Remaining characters, excluding NUL.
};

err_t snprintf_writer(char c, void *vstate)
{
	struct snprintf_state *state = vstate;

	if (state->rem == 0)
		return -ERR_SIZE;

	*state->cur++ = c;
	state->rem--;

	return ERR_NONE;
}

// TODO: Write a custom, optimized version in assembly
char *strncpy(char *dest, const char *src, size_t n)
{
	size_t len = strlen(src) + 1;
	size_t sz  = min(len, n);

	memcpy(dest, src, sz);
	bzero(dest + sz, n - sz);
	dest[n - 1] = '\0';

	return dest;
}

err_t _snprintf(char *dest, size_t size, const char *fmt, ...)
{
	struct snprintf_state state = { .cur = dest, .rem = size - 1 };

	struct _pfmt_fe fe = { .writer = snprintf_writer, .state = &state };
	va_list         args;
	err_t           ret;

	bzero(dest, size);
	va_start(args, fmt);
	ret = _pfmt_vproc(&fe, fmt, &args);
	va_end(args);

	return ret;
}
