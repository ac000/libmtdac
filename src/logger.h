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

enum log_level {
	MTD_LOG_ERR = 0,
	MTD_LOG_INFO,
	MTD_LOG_DEBUG,
};

static const struct logger {
	const enum log_level log_level;
	const char *str;
} logger_map[] = {
	{ MTD_LOG_ERR, LIBNAME " ERROR" },
	{ MTD_LOG_INFO, LIBNAME " INFO" },
	{ MTD_LOG_DEBUG, LIBNAME " DEBUG" },
};

extern unsigned mtd_log_level;
static inline void logger(enum log_level log_level, const char *fmt, ...)
{
	int len;
	char *logbuf;
	va_list ap;
	FILE *out = log_level == MTD_LOG_ERR ? stderr : stdout;

	if (log_level > mtd_log_level)
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
		fprintf(out, "[%s] %s", logger_map[log_level].str, logbuf);

	free(logbuf);
}

#endif /* _LOGGER_H_ */
