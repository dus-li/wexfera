/* SPDX-License-Identifier: GPL-3.0-only */
/* SPDX-FileCopyrightText: Duszku */

#include <board.h>

#include <lib/array_size.h>
#include <lib/compiler.h>
#include <lib/error.h>
#include <lib/log.h>
#include <lib/types.h>

static int _log_puts(const char *s)
{
	for (char c = *s; c != '\0'; c = *++s)
		board_log_be(c);

	return ERR_NONE;
}

/**
 * String format specifier processor.
 * @param args Argument list.
 * @param beg  Start of the format specifier.
 * @param end  End of the format specifier.
 *
 * @return @a ERR_NONE on success.
 * @return Negative error code otherwise.
 */
static int log_proc_s(va_list *args, const char *beg, const char *end)
{
	char *s = va_arg(*args, char *);
	return _log_puts(s ? s : "(null)");
}

/**
 * Unsigned 32-bit integer decimal format specifier processor.
 * @param args Argument list.
 * @param beg  Start of the format specifier.
 * @param end  End of the format specifier.
 *
 * @return @a ERR_NONE on success.
 * @return Negative error code otherwise.
 */
static int log_proc_u(va_list *args, const char *beg, const char *end)
{
	char buf[10];
	u32  val = va_arg(*args, u32);
	int  i, width = 0;

	for (i = 0; i < end - beg; ++i) {
		if (!isdigit(beg[i]))
			return -ERR_ARG;

		width *= 10;
		width += beg[i] - '0';
	}

	if (width > ARRAY_SIZE(buf))
		return -ERR_ARG;

	for (i = 0; i < ARRAY_SIZE(buf) && val != 0; ++i, val /= 10)
		buf[i] = '0' + (val % 10);

	for (; i < width; ++i)
		buf[i] = ' ';

	if (i == 0)
		board_log_be('0');
	else {
		while (i-- >= 0)
			board_log_be(buf[i]);
	}

	return ERR_NONE;
}

/**
 * Unsigned 32-bit integer hexadecimal format specifiers processor.
 * @param str  Digit string.
 * @param args Argument list.
 * @param beg  Start of the format specifier.
 * @param end  End of the format specifier.
 *
 * Since format specifiers '%x' and '%X' only differ in regards to the case of
 * the hexadecimal digits, this function captures common logic between the two
 * and adjusts used characters depending on value of @a str.
 *
 * @return @a ERR_NONE on success.
 * @return Negative error code otherwise.
 */
static int _log_proc_x(const char str[16], va_list *args, const char *beg,
    const char *end)
{
	char buf[8 + 2];
	bool with_0x = false;
	int  i, width = 0;
	u32  val = va_arg(*args, u32);

	for (i = 0; i < end - beg; ++i) {
		if (beg[i] == '#') {
			with_0x = true;
			continue;
		}

		if (!isdigit(beg[i]))
			return -ERR_ARG;

		width *= 10;
		width += beg[i] - '0';
	}

	if (width > ARRAY_SIZE(buf))
		return -ERR_ARG;

	for (i = 0; i < ARRAY_SIZE(buf) && val != 0; ++i, val >>= 4)
		buf[i] = str[val & 0xF];

	if (with_0x) {
		buf[i++] = 'x';
		buf[i++] = '0';
	}

	for (; i < width; ++i)
		buf[i] = ' ';

	if (i == 0)
		board_log_be('0');
	else {
		while (i-- >= 0)
			board_log_be(buf[i]);
	}

	return ERR_NONE;
}

/**
 * Unsigned 32-bit integer hexadecimal lowercase format specifier processor.
 * @param args Argument list.
 * @param beg  Start of the format specifier.
 * @param end  End of the format specifier.
 *
 * @return @a ERR_NONE on success.
 * @return Negative error code otherwise.
 *
 * @see _log_proc_x
 */
static int log_proc_x_lower(va_list *args, const char *beg, const char *end)
{
	return _log_proc_x("0123456789abcdef", args, beg, end);
}

/**
 * Unsigned 32-bit integer hexadecimal uppercase format specifier processor.
 * @param args Argument list.
 * @param beg  Start of the format specifier.
 * @param end  End of the format specifier.
 *
 * @return @a ERR_NONE on success.
 * @return Negative error code otherwise.
 *
 * @see _log_proc_x
 */
static int log_proc_x_upper(va_list *args, const char *beg, const char *end)
{
	return _log_proc_x("0123456789ABCDEF", args, beg, end);
}

/** Mapping of format specifiers to their processor functions. */
static const struct {
	char spec;
	int (*proc)(va_list *, const char *, const char *);
} _log_specifier_map[] = {
	{ 'x', log_proc_x_lower },
	{ 'X', log_proc_x_upper },
	{ 'u', log_proc_u },
	{ 's', log_proc_s },
};

static inline int _log_get_spec(char c)
{
	for (int i = 0; i < ARRAY_SIZE(_log_specifier_map); ++i) {
		if (_log_specifier_map[i].spec == c)
			return i;
	}

	return -1;
}

/**
 * Print formatted output.
 * @param fmt  Format string.
 * @param args List of arguments for conversions.
 *
 * This serves a similar purpose to standard C libraries' vprintf, however it
 * does not return number of successful conversions and the format specifiers
 * are limited to what is defined in @ref _log_specifier_map. The specifiers
 * themselves may not support some of the options that may be passed to them.
 *
 * @return @a ERR_NONE on success.
 * @return Negative error code otherwise.
 */
static int log_vprintf(const char *fmt, va_list args)
{
	const char *beg, *end;
	int         ret, spec;

	while (*fmt != '\0') {
		if (*fmt == '\n')
			board_log_be('\r');

		if (*fmt != '%') {
			board_log_be(*fmt++);
			continue;
		}

		beg = ++fmt;
		for (end = beg; (spec = _log_get_spec(*end)) < 0; end++) {
			if (*end == '\0')
				return -ERR_ARG;
		}

		ret = _log_specifier_map[spec].proc(&args, beg, end);
		if (ret != ERR_NONE)
			return ret;

		fmt = end + 1;
	}

	return ERR_NONE;
}

static const char *const _log_pfx_map[] = {
	[LOG_DEBUG]  = "\033[32m", // Green
	[LOG_INFO]   = "\033[36m", // Light blue
	[LOG_WARN]   = "\033[33m", // Yellow
	[LOG_ERROR]  = "\033[31m", // Red
	[LOG_ALWAYS] = "",
};

int __printfmt(2, 3) _log(enum log_levels lvl, const char *fmt, ...)
{
	va_list args;
	int     ret;

	if (lvl < BOARD_LOG_LEVEL)
		return ERR_NONE;

	_log_puts(_log_pfx_map[lvl]);
	va_start(args, fmt);
	ret = log_vprintf(fmt, args);
	va_end(args);
	_log_puts("\033[0m");

	return ret;
}
