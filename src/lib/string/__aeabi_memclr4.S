/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#include <asm/helpers.h>

/*
 * void *__aeabi_memclr4(void *dest, size_t n)
 *
 * Following diagram roughly represents the implementation details, although it
 * omits all safety checks and focuses on the copy logic itself. The general
 * idea of the algorithm is to align dest to a word  boundary and then proceed
 * with clearing whole words, rather than bytes.
 *
 *                       +----------------------------+
 *                       | while (dest AND 0b11) ≠ 0: |
 *                       |   Set byte of dest to zero |
 *                       |   Decrement size           |
 *                       +----------------------------+
 *                                     |
 *                                     |
 *                       +----------------------------+
 *                       | while size ≥ 4:            |
 *                       |   Set word of dest to zero |
 *                       |   Decrease size by four    |
 *                       +----------------------------+
 *                                     |
 *                                     |
 *                                     |
 *                       +----------------------------+
 *                       | while size > 0:            |
 *                       |   Set byte of dest to zero |
 *                       |   Decrement size           |
 *                       +----------------------------+
 */

BEGIN_GLOBAL_FUNCTION(__aeabi_memclr4)
	CMP	r1, #0
	BEQ	exit
	MOV	r3, r0
	MOV	r2, #0

	// EABI assumes valid pointers, but user may call us from a macro
	CMP	r0, #0
	BEQ	return

	// Align to dest to word boundary
align:	ANDS	r12, r0, #0b11
	BEQ	words
	STRB	r2, [r0], #1
	SUBS	r1, r1, #1
	BNE	align
	B	return

	// Clear 4-byte words
words:	CMP	r1, #4
	BLO	bytes
	STR	r2, [r0], #4
	SUBS 	r1, r1, #4
	BNE	words

	// Clear remaining bytes
bytes:	CMP	r1, #0
	BEQ	return
blop:	STRB	r2, [r0], #1
	SUBS	r1, r1, #1
	BNE	blop

	// Restore return value and return
return:	MOV	r0, r3
exit:	BX	lr
END_FUNCTION(__aeabi_memclr4)
