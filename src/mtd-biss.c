/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * mtd-biss.c - Make Tax Digital - Business Income Source Summary API
 *
 * Copyright (C) 2020 - 2021	Andrew Clayton <andrew@digital-domain.net>
 */

#include <stddef.h>

#include "mtd-biss.h"		/* for default (public) visibility */
#include "endpoints.h"

#define API_VER			"Accept: application/vnd.hmrc.1.0+json"

/*
 * [GET ]
 * /individuals/self-assessment/income-summary/{nino}/foreign-property?businessId=&typeOfBusiness={foreign-property-fhl-eea,foreign-property}[&taxYear=YYYY-YY]
 */
int mtd_biss_get_foreign_property(const char *query_string, char **buf)
{
	return do_ep(BISS_GET_FOREIGN_PROPERTY, API_VER,
		     NULL, buf, query_string, (char *)NULL);
}

/*
 * [GET ]
 * /individuals/self-assessment/income-summary/{nino}/uk-property?typeOfBusiness={uk-property-non-fhl,uk-property-fhl}[&taxYear=YYYY-YY]
 */
int mtd_biss_get_uk_property(const char *query_string, char **buf)
{
	return do_ep(BISS_GET_UK_PROPERTY, API_VER,
		     NULL, buf, query_string, (char *)NULL);
}

/*
 * [GET ]
 * /individuals/self-assessment/income-summary/{nino}/self-employment?selfEmploymentId=[&taxYear=YYYY-YY]
 */
int mtd_biss_get_self_employment(const char *query_string, char **buf)
{
	return do_ep(BISS_GET_SELF_EMPLOYMENT, API_VER,
		     NULL, buf, query_string, (char *)NULL);
}
