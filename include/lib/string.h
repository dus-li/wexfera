/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#pragma once

/**
 * @file  string.h
 * @brief String and memory operations.
 */

#include <lib/types.h>

/**
 * Copy memory area.
 * @param _dest Destination address.
 * @param _src  Source address.
 * @param _n    Number of bytes to copy.
 *
 * The implementation imitates POSIX in terms of behaviour.
 */
#define memcpy(_dest, _src, _n) __aeabi_memcpy((_dest), (_src), (_n))
void *__aeabi_memcpy(void *dest, const void *src, size_t n);

/**
 * Fill memory area with zeros.
 * @param _dest Destination address.
 * @param _n    Number of bytes to zero-out.
 */
#define bzero(_dest, _n) __aeabi_memclr4((_dest), (_n))
void *__aeabi_memclr4(void *dest, size_t n);

/**
 * Compute length of a string.
 * @param s String whose length is to be found.
 *
 * @return Length of @a s.
 */
size_t strlen(const char *s);

/**
 * Copy a string to a fixed-size buffer, padding with NUL bytes as needed.
 * @param dest Destination buffer.
 * @param src  Source string.
 * @param n    Size of @a dest.
 *
 * @return @a dest
 */
char *strncpy(char *dest, const char *src, size_t n);
