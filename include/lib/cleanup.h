/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#pragma once

/**
 * @file  cleanup.h
 * @brief Automatic cleanup helpers.
 */

#include <lib/compiler.h>
#include <lib/preproc.h>
#include <lib/types.h>

#define _CLEANUP_NAME(_name)        CONCAT(__cleanup_, _name)
#define _CLEANUP_LOCK_NAME(_name)   CONCAT(__cleanup_lock_, _name)
#define _CLEANUP_UNLOCK_NAME(_name) CONCAT(__cleanup_unlock_, _name)

/**
 * Declare a default way of cleaning up a certain class of data.
 * @param _name Name of the cleanup class.
 * @param _type Type that is is cleaned up.
 * @param _func Function which is responsible for the cleanup.
 *
 * Keep in mind, that the @a _name is binding here - not the @a _type. There may
 * be several default cleanups for the same type and as long as they have
 * different names, they function as different classes of data.
 */
#define CLEANUP_DEFAULT(_name, _type, _func)                         \
	static inline __used void _CLEANUP_NAME(_name)(_type * data) \
	{                                                            \
		_func(*data);                                        \
	}

/**
 * Declare a variable as a subject to automatic cleanup.
 * @param _name Name of the cleanup class.
 *
 * @see CLEANUP_DEFAULT
 * @see CLEANUP_CANCEL
 */
#define CLEANUP(_name) __cleanup(_CLEANUP_NAME(_name))

/**
 * Declare a lock type that supports scoped locking.
 * @param _name   Name of the lock class.
 * @param _type   Underlying lock type.
 * @param _lock   Locking function.
 * @param _unlock Unlocking function.
 *
 * Much like in case of @ref CLEANUP_DEFAULT the @a _name is what differentiates
 * different classes of locks, not the @a _type. There may be several lock
 * classes defined for the same underlying lock type.
 *
 * The purpose of using this macro is to allow users to utilize the
 * @ref LOCKED_UNTIL_END and @ref LOCKED macros for a safer and cleaner lock
 * management.
 */
#define CLEANUP_LOCK(_name, _type, _lock, _unlock)                           \
	static inline __used _type *_CLEANUP_LOCK_NAME(_name)(_type * data)  \
	{                                                                    \
		_lock(data);                                                 \
		return data;                                                 \
	}                                                                    \
	static inline __used void _CLEANUP_UNLOCK_NAME(_name)(_type * *data) \
	{                                                                    \
		_unlock(*data);                                              \
	}

#define _CLEANUP_DECL_LOCK(_name, _plock) \
	__auto_type __p __cleanup(        \
	    _CLEANUP_UNLOCK_NAME(_name)) = _CLEANUP_LOCK_NAME(_name)(_plock)

/**
 * Protect all the following code with a lock.
 * @param _name  Name of the lock class.
 * @param _plock Pointer to the lock that is to be used for protection.
 *
 * This macro is a convenient way of marking remaining code in a function as a
 * critical section that is to be protected by a lock pointed to by @ref _plock.
 * The lock is automatically taken and released by the macro.
 *
 * @see LOCKED
 */
#define LOCKED_UNTIL_END(_name, _plock)    \
	_CLEANUP_DECL_LOCK(_name, _plock); \
	(void)__p

#define _LOCKED(_name, _plock, _var) \
	bool _var = true;            \
	for (_CLEANUP_DECL_LOCK(_name, _plock); _var; (void)__p, _var = false)

/**
 * Protect following statement with a lock.
 * @param _name  Name of the lock class.
 * @param _plock Pointer to the lock that is to be used for protection.
 *
 * This macro can be used to create a locked scope. Before the scope the lock is
 * taken, and once said scope is left - the lock is automatically released.
 *
 * @see LOCKED_UNTIL_END
 */
#define LOCKED(_name, _plock) \
	_LOCKED(_name, _plock, CONCAT(__locked_, __COUNTER__))
