/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * mtd-ni.c - Make Tax Digital - National Insurance API
 *
 * Copyright (C) 2020		Andrew Clayton <andrew@digital-domain.net>
 */

#include "mtd-ni.h"		/* for default (public) visibility */
#include "endpoints.h"
#include "curler.h"

#define API_VER			"Accept: application/vnd.hmrc.1.1+json"

/*
 * [GET ]
 * /national-insurance/sa/{utr}/annual-summary/{taxYear}
 */
int mtd_ni_get_annual_summary(const char *utr, const char *tax_year,
			      char **buf)
{
	return do_ep(NI_GET_ANNUAL_SUMMARY, API_VER,
		     NULL, NULL, buf, utr, tax_year, (char *)NULL);
}
