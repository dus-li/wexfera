/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#pragma once

// Number of IRQs
#define BOARD_IRQ_NUMBER (98)

// Set to 1 if system contains an FPU coprocessor
#define BOARD_HAS_FPU (1)

// Set to 1 if non-maskable interrupt is used for watchdog expiration
#define BOARD_NMI_IS_WDG (1)

// Board clock frequency (72 MHz)
#define BOARD_HZ (72000000)

#if !defined(__ASSEMBLER__)
void board_init_clock(void);
#endif // !defined(__ASSEMBLER__)
