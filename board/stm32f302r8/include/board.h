/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#pragma once

// Number of IRQs
#define BOARD_IRQ_NUMBER (98)

// Set to 1 if system contains an FPU coprocessor
#define BOARD_HAS_FPU (1)

// Set to 1 if non-maskable interrupt is used for watchdog expiration
#define BOARD_NMI_IS_WDG (1)

// Set to 1 to enable debug features
#define BOARD_DEBUG_FEATURES (0)

// Board clock frequency (36 MHz)
#define BOARD_HZ (36000000)

// Function used for logging
#define BOARD_LOG_BACKEND board_log_be

#if !defined(__ASSEMBLER__)
  #include <lib/log.h>

// Log level
extern const enum log_levels BOARD_LOG_LEVEL;

void board_init_clock(void);
void board_log_be(char c);
#endif // !defined(__ASSEMBLER__)
