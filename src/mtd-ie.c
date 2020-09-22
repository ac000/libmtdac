/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * mtd-ie.c - Make Tax Digital - Individual Employment API
 *
 * Copyright (C) 2020		Andrew Clayton <andrew@digital-domain.net>
 */

#include <stddef.h>

#include "mtd-ie.h"		/* for default (public) visibility */
#include "endpoints.h"

#define API_VER			"Accept: application/vnd.hmrc.1.1+json"

/*
 * [GET ]
 * /individual-employment/sa/{utr}/annual-summary/{taxYear}
 */
int mtd_ie_get_annual_summary(const char *utr, const char *tax_year,
			      char **buf)
{
	return do_ep(IE_GET_ANNUAL_SUMMARY, API_VER,
		     NULL, buf, utr, tax_year, (char *)NULL);
}
