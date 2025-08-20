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
#define SCS_SCB_SHPR1 (SCS_SCB_BASE + 0x14) ///< System Handler Priority #1.
#define SCS_SCB_SHPR2 (SCS_SCB_BASE + 0x18) ///< System Handler Priority #2.
#define SCS_SCB_SHPR3 (SCS_SCB_BASE + 0x1C) ///< System Handler Priority #3.
#define SCS_SCB_SHCSR (SCS_SCB_BASE + 0x24) ///< System Handler Control & State.
#define SCS_SCB_CFSR  (SCS_SCB_BASE + 0x28) ///< Configurable Fault Status.
#define SCS_SCB_HFSR  (SCS_SCB_BASE + 0x2C) ///< HardFault Status.
#define SCS_SCB_MMFAR (SCS_SCB_BASE + 0x34) ///< MemManage Fault Address.
#define SCS_SCB_BFAR  (SCS_SCB_BASE + 0x38) ///< BusFault Address.
#define SCS_SCB_CPACR (SCS_SCB_BASE + 0x88) ///< Coprocessor Access Control.

/** @} */ // scbRegisters

/**
 * @defgroup systRegisters SysTick registers.
 * @brief    Addresses of architecture-defined SysTick registers.
 * @{
 */

#define SCS_SYST_BASE (0xE000E010)
#define SCS_SYST_CSR  (SCS_SYST_BASE + 0x00)
#define SCS_SYST_RVR  (SCS_SYST_BASE + 0x04)
#define SCS_SYST_CVR  (SCS_SYST_BASE + 0x08)

/** @} */ // systRegisters

/** Full access bits for CP10 and CP11 to control FPU. */
#define SCS_SCB_CPACR_FPU_FULL (0xF00000)

#define SCS_SCB_SHCSR_USGFAULTENA (1U << 18) ///< UsageFault enable bit.
#define SCS_SCB_SHCSR_BUSFAULTENA (1U << 17) ///< BusFault enable bit.
#define SCS_SCB_SHCSR_MEMFAULTENA (1U << 16) ///< MemManage fault enable bit.

#define SCS_SYST_CSR_ENABLE    (1U << 0) ///< SysTick enable bit.
#define SCS_SYST_CSR_TICKINT   (1U << 1) ///< SysTick interrupt enable bit.
#define SCS_SYST_CSR_CLKSOURCE (1U << 2) ///< If set, SysTick uses CPU clock.

#if !defined(__ASSEMBLER__)

  #include <lib/bits.h>

  #define SCS_SCB_CFSR_MEMMANAGE  BITS(7, 0)   ///< MemManage exception info.
  #define SCS_SCB_CFSR_BUSFAULT   BITS(15, 8)  ///< BusFault exception info.
  #define SCS_SCB_CFSR_USAGEFAULT BITS(31, 16) ///< UsageFault exception info.

#endif // !defined(__ASSEMBLER__)
