/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#pragma once

#include <lib/compiler.h>
#include <lib/error.h>
#include <lib/types.h>

/**
 * @file  stm32.h
 * @brief STM32 GPIO driver.
 */

/**
 * STM32 GPIO pin descriptor.
 *
 * Instances of this structure are meant to be created with macro
 * @ref GPIO_STM32_PIN and their puspose is to be passed as an argument to the
 * @ref gpio_stm32_init function in order to configure given GPIO pin and make
 * it operational.
 *
 * @todo Design implementation-agnostic high-level GPIO wrapper.
 */
struct gpio_stm32_pin {
	u8 pin; ///< Pin number.

	/** GPIO bank information. */
	struct {
		void *bank; ///< Base address of GPIO pin's bank registers.
		void *rcc;  ///< Base address of the RCC register.
		u8    bit;  ///< Bit in the AHBENR representing the bank.
	} bus;

	/** GPIO mode. */
	enum __closed {
		GPIO_STM32_MODE_IN  = 0b00, ///< Digital input.
		GPIO_STM32_MODE_OUT = 0b01, ///< Digital output.
		GPIO_STM32_MODE_ALT = 0b10, ///< Alternative function.
		GPIO_STM32_MODE_AN  = 0b11, ///< Analog mode.
	} mode;

	/** GPIO speed. */
	enum __closed {
		GPIO_STM32_SPEED_LOW = 0b00, ///< Low speed.
		GPIO_STM32_SPEED_MED = 0b01, ///< Medium speed.
		GPIO_STM32_SPEED_HI  = 0b11, ///< High speed.
	} speed;

	/** GPIO pull up/down. */
	enum __closed {
		GPIO_STM32_PULL_NONE = 0b00, ///< None.
		GPIO_STM32_PULL_UP   = 0b01, ///< Pull-up.
		GPIO_STM32_PULL_DOWN = 0b10, ///< Pull-down.
	} pull;
};

#define GPIO_STM32_PIN_BUS(_bank, _rcc, _bit) \
	{ .bank = (_bank), .rcc = (_rcc), .bit = (_bit) }

#define GPIO_STM32_PIN(_bus, _pin, _mode, _speed, _pull) \
	{ /**/                                           \
		.pin   = (_pin),                         \
		.bus   = _bus,                           \
		.mode  = (_mode),                        \
		.speed = (_speed),                       \
		.pull  = (_pull)                         \
	}

err_t gpio_stm32_init(struct gpio_stm32_pin *pin);
