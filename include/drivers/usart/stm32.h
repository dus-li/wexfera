/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#pragma once

#include <lib/error.h>
#include <lib/types.h>

/**
 * @file  stm32.h
 * @brief STM32 USART driver.
 */

/**
 * STM32 USART instance descriptor.
 *
 * Instances of this structure are meant to be created with macro
 * @ref USART_STM32_INSTANCE and their purpose is to be passed as an argument to
 * the @ref usart_stm32_init function in order to configure given USART instance
 * and make it operational.
 */
struct usart_stm32_instance {
	void *base; ///< Base address of USART instance's registers.

	/** Bus descriptor. */
	struct {
		void *rcc; ///< Address of corresponding APBxEN register.
		u8    bit; ///< Bit within @a rcc corresponding to the instance.
	} bus;

	/** GPIO descriptor. */
	struct {
		void *rcc;  ///< Base address of the RCC register.
		void *bank; ///< Address of base of corresponding GPIO bank.
		u8    bit;  ///< RCC bit for the bank.
		u8    tx;   ///< GPIO pin used for transmission.
		u8    rx;   ///< GPIO pin used for receiving.
	} gpio;
};

#define USART_STM32_INSTANCE_BUS(_rcc, _bit) \
	{ .rcc = (void *)(_rcc), .bit = (_bit) }

#define USART_STM32_INSTANCE_GPIO(_bank, _rcc, _tx, _rx, _bit) \
	{                                                      \
		/**/                                           \
		.bank = (void *)(_bank),                       \
		.rcc  = (void *)(_rcc),                        \
		.bit  = (_bit),                                \
		.tx   = (_tx),                                 \
		.rx   = (_rx),                                 \
	}

/**
 * Initializer macro for @ref usart_stm32_instance.
 * @param _base UART instance registers' base address.
 * @param _bus  Result of @ref USART_STM32_INSTANCE_BUS.
 * @param _gpio Result of @ref USART_STM32_INSTANCE_GPIO.
 */
#define USART_STM32_INSTANCE(_base, _bus, _gpio) \
	{ .base = (void *)(_base), .bus = _bus, .gpio = _gpio }

/**
 * Initialize a USART instance.
 * @param in USART instance descriptor created by @ref USART_STM32_INSTANCE.
 *
 * @returns @ref err_t
 */
err_t usart_stm32_init(struct usart_stm32_instance *in);

/**
 * Transmit a byte over USART.
 * @param base Base address of the USART instance registers.
 * @param data Byte that is to be sent.
 */
void usart_stm32_send_byte(void *base, u8 data);
