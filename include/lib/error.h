/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#pragma once

/**
 * @file  error.h
 * @brief Error codes and error pointers.
 */

enum errors {
	ERR_NONE, ///< No error ocurred.

	ERR_ARG,      ///< Invalid argument.
	ERR_CORRUPT,  ///< Corrupted structure.
	ERR_DONE,     ///< Operation was done already and cannot be repeated.
	ERR_ILLEGAL,  ///< Not allowed.
	ERR_INTERNAL, ///< It failed, but its (probably) not your fault.
	ERR_MEM,      ///< Not enough memory.
	ERR_TAKEN,    ///< Someone had a quicker hand, partner.
	ERR_UNIMPL,   ///< Not implemented.
	ERR_DNE,      ///< Does not exist.

	__ERR_MAX,
};

/**
 * Default error type.
 *
 * If a function returns this type, it is a clear indicator that it commits to
 * following a certain convention. The return value is either @ref ERR_NONE in
 * which case the operation succeeded, or a negative error code in case of
 * failure.
 *
 * Note, that other functions may still choose to return error codes defined
 * in @ref errors, but for instance reserve positive numbers as legal return
 * values. Such functions should be declared as @a int or @a ssize_t or whatnot.
 */
typedef enum errors err_t;

/**
 * Encode an error in a pointer.
 * @param _err Error code that is to be encoded.
 */
#define ERR_PENC(_err) (void *)(_err)

/**
 * Decode an error code from an error pointer.
 * @param _ptr Error pointer.
 */
#define ERR_PDEC(_ptr) (u32)(_ptr)

/**
 * Check if a pointer is an error pointer.
 * @param _ptr Pointer that is to be checked.
 */
#define ERR_IS_PERR(_ptr) ((u32)(_ptr) < __ERR_MAX)
