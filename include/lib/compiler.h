/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#pragma once

/**
 * @file  compiler.h
 * @brief Compiler builtins and attributes.
 */

#define __closed                __attribute__((enum_extensibility(closed)))
#define __naked                 __attribute__((naked))
#define __section(_sec)         __attribute__((section(_sec)))
#define __used                  __attribute__((used))
#define __printfmt(_fmt, _args) __attribute__((format(printf, (_fmt), (_args))))

#define ctz(_val) __builtin_ctz(_val)

#define same_type(_fst, _snd) \
	__builtin_types_compatible_p(typeof(_fst), typeof(_snd))
