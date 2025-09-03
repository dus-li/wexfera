/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

.macro IRQSAVESP, dest
	TST	lr, #4
	ITE	eq
	MRSEQ	\dest, msp
	MRSNE	\dest, psp
.endmacro
