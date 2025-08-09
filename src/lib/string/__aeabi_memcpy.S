/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#include <asm/helpers.h>

/*
 * void *__aeabi_memcpy(void *dest, const void *src, size_t n)
 *
 * Following diagram roughly represents the implementation details, although it
 * omits all safety checks and focuses on the copy logic itself. The general
 * idea of the algorithm is to try to align the dest and the source to a word
 * boundary and then proceed with copying whole words, rather than bytes.
 *
 * If the alignment could not have been trivially solved (i.e. it would require
 * looking for a smallest common multiple of the two misalignments) then we fall
 * back to a simple byte copy.
 *
 *                   +-------------------------------------+
 *                   | Bytes [1:0] of src and dest differ? |
 *                   +-------------------------------------+
 *                           NO        / \        YES
 *                 +------------------+   +------------------+
 *                 |                                         |
 *   +----------------------------+                          |
 *   | while (dest AND 0b11) ≠ 0: |                          |
 *   |   Copy byte of src to dest |                          |
 *   |   Decrement size           |                          |
 *   +----------------------------+                          |
 *                 |                                         |
 *                 |                                         |
 *   +----------------------------+                          |
 *   | while size ≥ 4:            |                          |
 *   |   Copy word of src to dest |                          |
 *   |   Decrease size by four    |                          |
 *   +----------------------------+                          |
 *                 |                                         |
 *                 +--------------------+--------------------+
 *                                      |
 *                       +----------------------------+
 *                       | while size > 0:            |
 *                       |   Copy byte of src to dest |
 *                       |   Decrement size           |
 *                       +----------------------------+
 */

BEGIN_GLOBAL_FUNCTION(__aeabi_memcpy)
	// Check arguments and save return value
	CMP	r2, #0
	BEQ	exit
	MOV	r3, r0

	// If dest and src are misaligned in different ways - copy bytes
	EOR	r12, r0, r1
	ANDS	r12, r12, #0b11
	BNE	bytes

	// If dest and src are misaligned in the same way - copy leading bytes
align:	ANDS	r12, r0, #0b11
	BEQ	words
	CMP	r2, #0
	BEQ	return
	LDRB	r12, [r1], #1
	STRB	r12, [r0], #1
	SUB	r2, r2, #1
	B	align

	// Copy 4-byte words
words:	CMP	r2, #4
	BLO	bytes
	LDR	r12, [r1], #4
	STR	r12, [r0], #4
	SUBS	r2, r2, #4
	B	words

	// Copy remaining bytes
bytes:	CMP	r2, #0
	BEQ	return
blop:	LDRB	r12, [r1], #1
	STRB	r12, [r0], #1
	SUBS	r2, r2, #1
	BNE	blop

	// Restore return value and return
return:	MOV	r0, r3
exit:	BX	lr
END_FUNCTION(__aeabi_memcpy)
