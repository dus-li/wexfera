/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#pragma once

/**
 * @file  compiler.h
 * @brief Compiler builtins and attributes and pseudo-keywords.
 */

// Non-parametrized attributes
#define __closed __attribute__((enum_extensibility(closed)))
#define __naked  __attribute__((naked))
#define __retain __attribute__((retain))
#define __used   __attribute__((used))

// Parametrized attributes
#define __cleanup(_func)        __attribute__((__cleanup__(_func)))
#define __printfmt(_fmt, _args) __attribute__((format(printf, (_fmt), (_args))))
#define __section(_sec)         __attribute__((section(_sec)))

#define ctz(_val) __builtin_ctz(_val)

#define same_type(_fst, _snd) \
	__builtin_types_compatible_p(typeof(_fst), typeof(_snd))

// Pseudo-keywords
#define fallthrough __attribute__((fallthrough))
#define loop        for (;;)
