/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#pragma once

/**
 * @file  fault.h
 * @brief Fault-related declarations and constants.
 */

#include <lib/compiler.h>

/**
 * @defgroup faultConstants Fault Constants
 * @brief    Numerical codes denoting different types of faults.
 *
 * Under Wexfera all faults are funneled into the same handler after some
 * initial processing done in Assembly. The fault-specific vectors set one of
 * the fault constant flags to inform the catch-all handler what specific issue
 * has taken place.
 *
 * @{
 */

#define FAULT_HARDFAULT  (0) ///< HardFault
#define FAULT_MEMMANAGE  (1) ///< MemManage Fault
#define FAULT_BUSFAULT   (2) ///< BusFault
#define FAULT_USAGEFAULT (3) ///< UsageFault

/** @} */ // faultConstants

#if !defined(__ASSEMBLER__)

/**
 * Enable faults.
 *
 * By default only HardFault is enabled on ARMv7m. The three remaining fault
 * types must be explicitly enabled by software in order for them to be
 * generated. These are MemManage fault, BusFault and UsageFault. This function
 * is responsible for enabling them.
 */
void fault_enable(void);

#endif // !defined(__ASSEMBLER__)
