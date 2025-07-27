/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#pragma once

/**
 * @file  helpers.h
 * @brief Helper macros for use in Assembly.
 *
 * Including this file in C is pointless. Its purpose is to abstract boilerplate
 * related to performing common actions such as defining functions or objects.
 */

// clang-format off

#if defined(__ASSEMBLY__)
	.syntax unified
	.thumb
#endif // defined(__ASSEMBLY__)

#define SECTION(name, flags, type) \
	.section name, flags, type

#define SHF_ALLOC "a"
#define SHT_PROGBITS %progbits

#define __END(name) .size name, .-name

#define BEGIN_GLOBAL_FUNCTION(name)     \
        .global name;                   \
        .type   name, %function;        \
name:;

#define BEGIN_GLOBAL_OBJECT(name) \
        .global name;             \
        .type   name, %object;    \
name:;

#define END_FUNCTION(name) __END(name)
#define END_OBJECT(name)   __END(name)
