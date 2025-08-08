/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#include <board.h>

#include <lib/bits.h>
#include <lib/error.h>
#include <lib/reg.h>

#include <drivers/gpio/stm32.h>
#include <drivers/usart/stm32.h>

/**
 * @defgroup usartStm32Registers STM32 USART registers.
 * @brief    Offsets of STM32 USART control registers.
 * @{
 */

#define USART_STM32_USART_CR1_OFF (0x00) ///< Control #1.
#define USART_STM32_USART_BRR_OFF (0x0C) ///< Baud rate.
#define USART_STM32_USART_ISR_OFF (0x1C) ///< Interrupt and status.
#define USART_STM32_USART_TDR_OFF (0x28) ///< Transmit data.

/** @} */ // usartStm32Registers

#define USART_STM32_GPIO_AFRL_OFF (0x20)
#define USART_STM32_GPIO_AFRH_OFF (0x24)

#define USART_STM32_BAUDRATE  (115200)
#define USART_STM32_BRR_VALUE (BOARD_HZ / USART_STM32_BAUDRATE)
#define USART_STM32_BRR_MASK  BITS(15, 0)

#define _USART_STM32_GPIO(_in, _pin)             \
	GPIO_STM32_PIN(/**/                      \
	    GPIO_STM32_PIN_BUS((_in)->gpio.bank, \
		(_in)->gpio.rcc,                 \
		(_in)->gpio.bit),                \
	    (_in)->gpio._pin,                    \
	    GPIO_STM32_MODE_ALT,                 \
	    GPIO_STM32_SPEED_LOW,                \
	    GPIO_STM32_PULL_NONE)

#define USART_STM32_GPIO_TX(_in) _USART_STM32_GPIO(_in, tx)
#define USART_STM32_GPIO_RX(_in) _USART_STM32_GPIO(_in, rx)

/**
 * Set given GPIO pin's alternative function to USART.
 * @param bank Base address of the
 * @param pin  GPIO pin that is to be configured.
 */
static inline void usart_stm32_gpio_set_afr(void *bank, u32 pin)
{
	const u32 AFR_WIDTH     = 4;
	const u8  USART_AF_CODE = 7;
	const u32 AFRS_PER_REG  = 32 / AFR_WIDTH;

	u32 off = pin < AFR_WIDTH ? USART_STM32_GPIO_AFRL_OFF :
				    USART_STM32_GPIO_AFRH_OFF;

	union32(USART_AF_CODE << AFR_WIDTH * (pin % AFRS_PER_REG), bank + off);
}

/**
 * Configure GPIO pins to USART alternative function.
 * @param in USART instance whose pins are to be configured.
 *
 * @returns @a ERR_NONE on success.
 * @returns Negative error code otherwise.
 */
static i32 usart_stm32_gpio_configure(struct usart_stm32_instance *in)
{

	struct gpio_stm32_pin tx = USART_STM32_GPIO_TX(in);
	struct gpio_stm32_pin rx = USART_STM32_GPIO_RX(in);

	i32 ret = gpio_stm32_init(&tx) ?: gpio_stm32_init(&rx);
	if (ret < 0)
		return ret;

	usart_stm32_gpio_set_afr(in->gpio.bank, in->gpio.tx);
	usart_stm32_gpio_set_afr(in->gpio.bank, in->gpio.rx);

	return ERR_NONE;
}

i32 usart_stm32_init(struct usart_stm32_instance *in)
{
	const u32 CR1_UE = 1U << 0;
	const u32 CR1_TE = 1U << 3;

	u32 tmp;
	i32 ret;

	// Enable USART clock
	union32(1U << in->bus.bit, in->bus.rcc);

	ret = usart_stm32_gpio_configure(in);
	if (ret < 0)
		return ret;

	union32(1U << 18, 0x4800000C);

	// Disable USART for the time of configuration.
	diff32(CR1_UE, in->base + USART_STM32_USART_CR1_OFF);

	// Configure baud rate
	tmp = USART_STM32_BRR_VALUE & USART_STM32_BRR_MASK;
	write32(tmp, in->base + USART_STM32_USART_BRR_OFF);

	// Enable TX and USART
	union32(CR1_TE, in->base + USART_STM32_USART_CR1_OFF);
	union32(CR1_UE, in->base + USART_STM32_USART_CR1_OFF);

	return ERR_NONE;
}

void usart_stm32_send_byte(void *base, u8 data)
{
	const u32 ISR_TXE = (1U << 7);

	// Wait for empty transmission buffer
	while (!(ISR_TXE & read32(base + USART_STM32_USART_ISR_OFF)))
		;

	write32(data, base + USART_STM32_USART_TDR_OFF);
}
