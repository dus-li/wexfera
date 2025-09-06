/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#include <asm/helpers.h>

/*
 * size_t strlen(const char *s)
 *
 * This implementation uses a known bit-twiddling trick for fast checking for
 * NUL bytes in a word: subtracting least significant bit in every byte and then
 * bitwise AND-ing it with masked more significant nibbles of each byte in the
 * inverse of the word. The former forces a borrow in every NUL byte, the latter
 * ensures that we don't get false-positives for bytes with values larger than
 * 0xF0. Once a word containing a NUL byte is found, we go through each of its
 * bytes searching for the first occurance of 0x00.
 *
 * Return value is calculated as difference between the pointer to the first NUL
 * byte and the string start pointer.
 */

BEGIN_GLOBAL_FUNCTION(strlen)
	// Backup start pointer.
	MOV	r12, r0

	// Align to a word boundary if needed. If NUL byte is found - exit.
align:	AND	r1, r0, #0b11
	CMP	r1, #0
	BEQ	words
	LDRB	r1, [r0]
	CMP	r1, #0
	BEQ	exit
	ADD	r0, r0, #1
	B	align

	// Load a word. If it contains a NUL byte, check where exactly.
words:	LDR	r1, [r0]
	SUB	r2, r1, #0x01010101
	MVN	r1, r1
	AND	r1, r1, r2
	AND	r1, r1, #0x80808080
	CMP	r1, #0
	BNE	bytes
	ADD	r0, r0, #4
	B	words

	// Locate a byte that is equal to NUL.
bytes:	LDRB	r1, [r0]
	CMP	r1, #0
	BEQ	exit
	ADD	r0, r0, #1
	B	bytes

	// Compute and return string length.
exit:	SUB	r0, r0, r12
	BX	lr
END_FUNCTION(strlen)
