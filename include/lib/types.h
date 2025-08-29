/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#pragma once

/**
 * @file  types.h
 * @brief Type definitions.
 */

#define NULL 0

typedef unsigned char u8;
typedef unsigned int  u32;

typedef int i32;

typedef u8 bool;
#define false 0
#define true  1

typedef u32 size_t;
typedef u32 uintptr_t;

typedef __builtin_va_list va_list;
#define va_start(_list, _last) __builtin_va_start(_list, _last)
#define va_arg(_list, _type)   __builtin_va_arg(_list, _type)
#define va_end(_list)          __builtin_va_end(_list)

int isdigit(int c);
