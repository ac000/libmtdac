/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * mtd-test-ni.c - Make Tax Digital - National Insurance Test Support API
 *
 * Copyright (C) 2020		Andrew Clayton <andrew@digital-domain.net>
 */

#include <stddef.h>

#include "mtd-test-ni.h"		/* for default (public) visibility */
#include "endpoints.h"

#define API_VER			"Accept: application/vnd.hmrc.1.0+json"

/*
 * [POST]
 * /national-insurance-test-support/sa/{utr}/annual-summary/{taxYear}
 */
int mtd_test_ni_create_annual_summary(const struct mtd_dsrc_ctx *dsctx,
				      const char *utr, const char *tax_year,
				      char **buf)
{
	return do_ep(TEST_NI_CREATE_ANNUAL_SUMMARY, API_VER,
		     dsctx, buf, utr, tax_year, (char *)NULL);
}
