/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#pragma once

/**
 * @file  compiler.h
 * @brief Compiler builtins and attributes.
 */

#define __section(_sec) __attribute__((section(_sec)))
#define __naked         __attribute__((naked))
#define __used          __attribute__((used))

#define ctz(_val) __builtin_ctz(_val)
