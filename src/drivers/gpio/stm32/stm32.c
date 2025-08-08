/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#include <lib/error.h>
#include <lib/reg.h>

#include <drivers/gpio/stm32.h>

#define GPIO_STM32_MAX_PIN (15)

#define GPIO_STM32_RCC_AHBENR_OFF (0x14)

#define GPIO_STM32_GPIO_MODER_OFF   (0x00)
#define GPIO_STM32_GPIO_OSPEEDR_OFF (0x08)
#define GPIO_STM32_GPIO_PUPDR_OFF   (0x0C)

i32 gpio_stm32_init(struct gpio_stm32_pin *pin)
{
	static const u32 RCC_AHBENR_OFF = 0x14;

	const void *bank  = pin->bus.bank;
	const u32   shift = 2 * pin->pin;

	if (pin->pin > GPIO_STM32_MAX_PIN || pin->bus.bit > 31)
		return -ERR_ARG;

	// Enable bus clock
	union32(1U << pin->bus.bit, pin->bus.rcc + RCC_AHBENR_OFF);

	// Mode, speed and pull up/down.
	union32(pin->mode << shift, bank + GPIO_STM32_GPIO_MODER_OFF);
	union32(pin->speed << shift, bank + GPIO_STM32_GPIO_OSPEEDR_OFF);
	union32(pin->pull << shift, bank + GPIO_STM32_GPIO_PUPDR_OFF);

	return ERR_NONE;
}
