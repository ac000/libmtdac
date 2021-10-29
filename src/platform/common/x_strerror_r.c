/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * platform/common/g_strerror_r.c - wrapper around XSI strerror_r(3)
 *
 * Copyright (C) 2021		Andrew Clayton <andrew@digital-domain.net>
 */

#include <string.h>

#include "../../platform.h"

/*
 * This is a wrapper around the POSIX (XSI) version of strerror_r(3)
 * which provides the semantics of the GNU version.
 *
 * FreeBSD only has the XSI version so it seemed simplest to just use
 * the XSI version on both Linux and FrewBSD and wrap it to provide
 * the GNU semantics of returning a pointer to the result buffer which
 * is useful.
 *
 * Getting the XSI version on Linux/glibc requires _not_ having
 * _GNU_SOURCE defined.
 *
 * For unknown errors it will return "Unknown error XXXX"
 * If buf is too small it will return a truncated message.
 */
char *x_strerror_r(int errnum, char *buf, size_t buflen)
{
	strerror_r(errnum, buf, buflen);
	return buf;
}
