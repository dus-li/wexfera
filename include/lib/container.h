/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#pragma once

/**
 * @file  container.h
 * @brief Utilities for working with embedded struct types.
 */

/**
 * Obtain offset of a struct field.
 * @param _type  Struct type within which lies the field.
 * @param _field Name of the field whose offset is to be found.
 */
#define offsetof(_type, _field) __builtin_offsetof(_type, _field)

/**
 * Obtain address of a containing struct of an embedded type.
 * @param _ptr   Address of the embedded element.
 * @param _type  Struct type within which lies the field.
 * @param _field Name of the field pointed to by @a _ptr.
 */
#define containerof(_ptr, _type, _field) \
	((_type *)((void *)(_ptr) - offsetof(_type, _field)))
