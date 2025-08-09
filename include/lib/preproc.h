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
