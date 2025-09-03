/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#pragma once

/**
 * @file  preproc.h
 * @brief Preprocessor-related utilities.
 */

#define _CONCAT(a, b) a##b
#define CONCAT(a, b)  _CONCAT(a, b)

#define _STRINGIFY(a) #a
#define STRINGIFY(a)  _STRINGIFY(a)

#define ___VA_ARGS_COUNT(_1, _2, _3, _4, _5, _6, _7, _8, _9, N, ...) N

#define __VA_ARGS_COUNT()   9, 8, 7, 6, 5, 4, 3, 2, 1, 0
#define _VA_ARGS_COUNT(...) ___VA_ARGS_COUNT(__VA_ARGS__)
#define VA_ARGS_COUNT(...)  _VA_ARGS_COUNT(__VA_ARGS__, __VA_ARGS_COUNT())
