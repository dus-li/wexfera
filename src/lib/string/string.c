/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#include <lib/extrema.h>
#include <lib/string.h>

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
