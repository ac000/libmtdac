/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * logger.h - Simple logging functon
 *
 * Copyright (C) 2020		Andrew Clayton <andrew@digital-domain.net>
 */

#ifndef _LOGGER_H_
#define _LOGGER_H_

#define _GNU_SOURCE		/* vasprintf(3) */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "mtd-priv.h"

static const char *logger_err_levels[] = {
	[MTD_LOG_ERR]	= LIBNAME " ERROR",
	[MTD_LOG_INFO]	= LIBNAME " INFO",
	[MTD_LOG_DEBUG]	= LIBNAME " DEBUG",
};

#define logger(log_level, fmt, ...) \
	_logger((const char *)__func__, log_level, fmt, ##__VA_ARGS__)

extern __thread struct mtd_ctx mtd_ctx;
__attribute__((format(printf, 3, 4)))
static inline void _logger(const char *func, enum log_level log_level,
			   const char *fmt, ...)
{
	int len;
	char *logbuf;
	va_list ap;
	FILE *out = log_level == MTD_LOG_ERR ? stderr : stdout;

	if (log_level > mtd_ctx.log_level)
		return;

	va_start(ap, fmt);
	len = vasprintf(&logbuf, fmt, ap);
	if (len == -1) {
		va_end(ap);
		return;
	}
	va_end(ap);

	if (*logbuf == ' ') /* continuation line */
		fprintf(out, "%s", logbuf);
	else
		fprintf(out, "[%s] %s: %s", logger_err_levels[log_level], func,
			logbuf);

	free(logbuf);
}

#endif /* _LOGGER_H_ */
