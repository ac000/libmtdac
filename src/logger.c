/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * logger.c - Simple logging functon
 *
 * Copyright (C) 2020 - 2025	Andrew Clayton <ac@sigsegv.uk>
 */

#define _GNU_SOURCE		/* vasprintf(3) */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>

#include "mtd-priv.h"
#include "platform.h"
#include "logger.h"

#define OPT_SP	"%s"
#define OPT_NL	OPT_SP

static const char *logger_err_levels[] = {
	[MTD_LOG_ERRNO] = LIBNAME " ERROR",
	[MTD_LOG_ERR]	= LIBNAME " ERROR",
	[MTD_LOG_INFO]	= LIBNAME " INFO",
	[MTD_LOG_DEBUG]	= LIBNAME " DEBUG",
};

extern __thread struct mtd_ctx mtd_ctx;
__attribute__((format(printf, 3, 4)))
void _logger(const char *func, enum log_level log_level, const char *fmt, ...)
{
	int e = errno;
	char *logbuf = NULL;
	char *errp = NULL;
	FILE *out = log_level <= MTD_LOG_ERR ? stderr : stdout;

	if (log_level > mtd_ctx.log_level)
		return;

	if (fmt) {
		va_list ap;
		int len;

		va_start(ap, fmt);
		len = vasprintf(&logbuf, fmt, ap);
		va_end(ap);
		if (len == -1)
			goto out_free;
	}

	if (log_level == MTD_LOG_ERRNO) {
		char errbuf[1024] = "\0";

		errp = x_strerror_r(errno, errbuf, sizeof(errbuf));
	}

	if (logbuf && *logbuf == ' ') /* continuation line */
		fprintf(out, "%s", logbuf);
	else
		fprintf(out, "[%s] %s:" OPT_SP "%s" OPT_SP "%s" OPT_NL,
			logger_err_levels[log_level], func,
			logbuf ? " " : "", logbuf ? logbuf : "",
			errp ? " " : "", errp ? errp : "", errp ? "\n" : "");

	errno = e;

out_free:
	free(logbuf);
}
