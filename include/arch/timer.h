/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#pragma once

/**
 * @file  timer.h
 * @brief SysTick timer configuration.
 */

/** SysTick interrupt frequency. */
#define TIMER_HZ (1000)

/**
 * Setup SysTick timer.
 *
 * Using value of a board-defined @a BOARD_HZ constant, this function configures
 * the system timer so that frequency of its interrupts matches @ref TIMER_HZ.
 */
void timer_configure(void);
