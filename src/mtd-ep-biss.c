/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * mtd-ep-biss.c - Make Tax Digital - Business Income Source Summary API
 *
 * Copyright (C) 2020 - 2022	Andrew Clayton <andrew@digital-domain.net>
 */

#include <stddef.h>

#include "mtd-biss.h"		/* for default (public) visibility */
#include "endpoints.h"

#define VERSION		"2.0"
#define API_VER		"Accept: application/vnd.hmrc." VERSION "+json"

/*
 * [GET ]
 * /individuals/self-assessment/income-summary/{nino}/{typeOfBusiness}/{taxYear}/{businessId}
 */
int mtd_biss_get_summary(const char *business_type, const char *tax_year,
			 const char *bid, char **buf)
{
	return do_ep(BISS_GET_SUMMARY, API_VER,
		     NULL, buf, business_type, tax_year, bid, (char *)NULL);
}
