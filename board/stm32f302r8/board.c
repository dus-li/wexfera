/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#include <lib/bits.h>
#include <lib/reg.h>

#define RCC_BASE (0x40021000)
#define RCC_CR   (RCC_BASE + 0x00)
#define RCC_CFGR (RCC_BASE + 0x04)

#define FLASH_BASE (0x40022000)
#define FLASH_ACR  (FLASH_BASE + 0x00)

#define RCC_CR_HSEON  (1U << 16)
#define RCC_CR_HSERDY (1U << 17)
#define RCC_CR_PLLON  (1U << 24)
#define RCC_CR_PLLRDY (1U << 25)

#define RCC_CFGR_SW     BITS(1, 0)
#define RCC_CFGR_PPRE1  BITS(10, 8)
#define RCC_CFGR_PPRE2  BITS(13, 11)
#define RCC_CFGR_PLLMUL BITS(21, 18)

#define FLASH_ACR_LATENCY BITS(2, 0)

#define HCLK_DIV_2      (0b100)
#define RCC_CFGR_SW_PLL (0b10)

void board_init_clock(void)
{
	// Enable high-speed external oscillator. (8 MHz)
	write32(RCC_CR_HSEON | read32(RCC_CR), RCC_CR);
	while (!(RCC_CR_HSERDY & read32(RCC_CR)))
		;

	// Set flash latency to 2 wait states (48 MHz < HCLK ≤ 72 MHz)
	write32(BITS_CONS(2, FLASH_ACR_LATENCY) | FLASH_ACR, FLASH_ACR);

	// Set PLL multiplier to 7 + 2 = 9. (72 MHz)
	write32(BITS_CONS(7, RCC_CFGR_PLLMUL) | read32(RCC_CFGR), RCC_CFGR);
	write32(RCC_CR_PLLON | read32(RCC_CR), RCC_CR);
	while (!(RCC_CR_PLLRDY & read32(RCC_CR)))
		;

	// Max frequency of APB1 is 36 MHz, therefore we must set divider to 2.
	write32(BITS_CONS(HCLK_DIV_2, RCC_CFGR_PPRE1) | RCC_CFGR, RCC_CFGR);

	// Set PLL as system clock
	write32(BITS_CONS(RCC_CFGR_SW_PLL, RCC_CFGR_SW) | RCC_CFGR, RCC_CFGR);
}
