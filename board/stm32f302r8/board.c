/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#include <lib/bits.h>
#include <lib/reg.h>

#include <boot/sequence.h>
#include <drivers/usart/stm32.h>

#define RCC_BASE    (0x40021000)
#define RCC_CR      (RCC_BASE + 0x00)
#define RCC_CFGR    (RCC_BASE + 0x04)
#define RCC_APB2ENR (RCC_BASE + 0x18)

#define FLASH_BASE (0x40022000)
#define FLASH_ACR  (FLASH_BASE + 0x00)

#define USART1_BASE (0x40013800)

#define RCC_CR_HSEON  (1U << 16)
#define RCC_CR_HSERDY (1U << 17)
#define RCC_CR_PLLON  (1U << 24)
#define RCC_CR_PLLRDY (1U << 25)

#define RCC_CFGR_SW     BITS(1, 0)
#define RCC_CFGR_PPRE1  BITS(10, 8)
#define RCC_CFGR_PPRE2  BITS(13, 11)
#define RCC_CFGR_PLLMUL BITS(21, 18)

#define GPIOA_BASE (0x48000000)

#define FLASH_ACR_LATENCY BITS(2, 0)

#define HCLK_DIV_2      (0b100)
#define RCC_CFGR_SW_PLL (0b10)

void board_init_clock(void)
{
	// Enable high-speed external oscillator.
	union32(RCC_CR_HSEON, RCC_CR);
	while (!(RCC_CR_HSERDY & read32(RCC_CR)))
		;

	// Set flash latency to 1 wait state (24 MHz < HCLK ≤ 48 MHz)
	union32(BITS_CONS(2, FLASH_ACR_LATENCY), FLASH_ACR);

	// Set PLL multiplier.
	union32(BITS_CONS(7, RCC_CFGR_PLLMUL), RCC_CFGR);
	union32(RCC_CR_PLLON, RCC_CR);
	while (!(RCC_CR_PLLRDY & read32(RCC_CR)))
		;

	// Set PLL as system clock
	union32(BITS_CONS(RCC_CFGR_SW_PLL, RCC_CFGR_SW), RCC_CFGR);
}

static void board_init_usart(void)
{
	struct usart_stm32_instance in = USART_STM32_INSTANCE(USART1_BASE,
	    USART_STM32_INSTANCE_BUS(RCC_APB2ENR, 14),
	    USART_STM32_INSTANCE_GPIO(GPIOA_BASE, RCC_BASE, 9, 10, 17));

	usart_stm32_init(&in);
}
SEQ_EARLY_INIT(board_init_usart);

void board_log_be(char c)
{
	usart_stm32_send_byte((void *)USART1_BASE, *(u8 *)(&c));
}
