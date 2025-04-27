/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * logger.h - Simple logging functon
 *
 * Copyright (C) 2020 - 2025	Andrew Clayton <ac@sigsegv.uk>
 */

#ifndef _LOGGER_H_
#define _LOGGER_H_

#define _GNU_SOURCE

#include "mtd-priv.h"

#define logger(log_level, fmt, ...) \
	_logger((const char *)__func__, log_level, fmt, ##__VA_ARGS__)

extern void _logger(const char *func, enum log_level log_level,
		    const char *fmt, ...);

#endif /* _LOGGER_H_ */
