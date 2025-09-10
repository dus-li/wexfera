/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#pragma once

#include <board.h>

#include <lib/compiler.h>
#include <lib/error.h>
#include <lib/types.h>

/**
 * @file  log.h
 * @brief Logging and printing.
 */

#ifndef LOG_FMT
  #define LOG_FMT(_fmt) __FILE_NAME__ ": " _fmt
#endif

/** Possible log levels. */
enum __closed log_levels {
	LOG_DEBUG,
	LOG_INFO,
	LOG_WARN,
	LOG_ERROR,
	LOG_ALWAYS,

	__LOG_MAX,
};

#if defined(BOARD_LOG_BACKEND)

err_t __printfmt(2, 3) _log(enum log_levels lvl, const char *fmt, ...);

  /**
   * Log a message.
   * @param _fmt Format string.
   * @param ...  Optional arguments.
   *
   * To obtain a consistent output for all prints in a file, one can
   * define @a LOG_FMT before including @ref log.h.
   */
  #define log_always(_fmt, ...) _log(LOG_ALWAYS, LOG_FMT(_fmt), ##__VA_ARGS__)

  /**
   * Log an error message.
   * @param _fmt Format string.
   * @param ...  Optional arguments.
   *
   * @see log_always
   */
  #define log_error(_fmt, ...) _log(LOG_ERROR, LOG_FMT(_fmt), ##__VA_ARGS__)

  /**
   * Log a warning message.
   * @param _fmt Format string.
   * @param ...  Optional arguments.
   *
   * @see log_always
   */
  #define log_warn(_fmt, ...) _log(LOG_WARN, LOG_FMT(_fmt), ##__VA_ARGS__)

  /**
   * Log an info message.
   * @param _fmt Format string.
   * @param ...  Optional arguments.
   *
   * @see log_always
   */
  #define log_info(_fmt, ...) _log(LOG_INFO, LOG_FMT(_fmt), ##__VA_ARGS__)

  /**
   * Log a debug message.
   * @param _fmt Format string.
   * @param ...  Optional arguments.
   *
   * @see log_always
   */
  #define log_debug(_fmt, ...) _log(LOG_DEBUG, LOG_FMT(_fmt), ##__VA_ARGS__)

#else
  #define _log(...)
  #define log_debug(...)
  #define log_info(...)
  #define log_warn(...)
  #define log_error(...)
  #define log_always(...)
#endif

#if (BOARD_DEBUG_FEATURES == 1)
  #define __log_dbgspot \
	  log_debug("%s (%u@%s)\n", __FUNCTION__, __LINE__, __FILE__)
#endif
