/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * mtd-ii.c - Make Tax Digital - Individual Income API
 *
 * Copyright (C) 2020		Andrew Clayton <andrew@digital-domain.net>
 */

#include <stddef.h>

#include "mtd-ii.h"		/* for default (public) visibility */
#include "endpoints.h"

#define API_VER			"Accept: application/vnd.hmrc.1.1+json"

/*
 * [GET ]
 * /individual-income/sa/{utr}/annual-summary/{taxYear}
 */
int mtd_ii_get_annual_summary(const char *utr, const char *tax_year,
			      char **buf)
{
	return do_ep(II_GET_ANNUAL_SUMMARY, API_VER,
		     NULL, buf, utr, tax_year, (char *)NULL);
}
