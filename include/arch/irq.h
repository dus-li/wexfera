/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#pragma once

/**
 * @file  irq.h
 * @brief IRQ-related declarations and constants.
 */

/** A type describing a pointer to an interrupt handler function. */
typedef void (*irq_handler_t)(void);

/** Disable incoming interrupts. */
#define irq_disable() asm volatile("cpsid i")

/** Enable incoming interrupts. */
#define irq_enable() asm volatile("cpsie i")

/**
 * Relocate interrupt vector table to memory.
 *
 * Wexfera starts with a static, compiled-in basic interrupt vector table that
 * services core ARMv7m faults and resides in Flash storage. In order to
 * dynamically register interrupt handlers this needs to be moved to SRAM.
 */
void irq_relocate(void);
