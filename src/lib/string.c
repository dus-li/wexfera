/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#include <lib/string.h>

void *__aeabi_memcpy(void *dest, const void *src, size_t n)
{
	const u8 *s = src;
	u8       *d = dest;

	while (n--)
		*d++ = *s++;

	return dest;
}

void *__aeabi_memclr4(void *dest, size_t n)
{
	u8 *d = dest;

	while (n--)
		*d++ = 0;

	return dest;
}
