/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#pragma once

/**
 * @file  scs.h
 * @brief System Control Space registers.
 */

/**
 * @defgroup scbRegisters System Control Block registers.
 * @brief    Addresses of architecture-defined SCB registers.
 * @{
 */

#define SCS_SCB_BASE  (0xE000ED00)          ///< Base address.
#define SCS_SCB_VTOR  (SCS_SCB_BASE + 0x08) ///< Vector Table Offset.
#define SCS_SCB_SHCSR (SCS_SCB_BASE + 0x24) ///< System Handler Control & State.
#define SCS_SCB_CPACR (SCS_SCB_BASE + 0x88) ///< Coprocessor Access Control.

/** @} */ // scbRegisters

/// Full access bits for CP10 and CP11 to control FPU.
#define SCS_SCB_CPACR_FPU_FULL (0xF << 20)

/// UsageFault enable bit.
#define SCS_SCB_SHCSR_USGFAULTENA (1 << 18)

/// BusFault enable bit.
#define SCS_SCB_SHCSR_BUSFAULTENA (1 << 17)

/// MemManage fault enable bit.
#define SCS_SCB_SHCSR_MEMFAULTENA (1 << 16)
