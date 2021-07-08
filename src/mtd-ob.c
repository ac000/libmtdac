/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * mtd-ob.c - Make Tax Digital - Obligations API
 *
 * Copyright (C) 2021		Andrew Clayton <andrew@digital-domain.net>
 */

#include <stddef.h>

#include "mtd-ob.h"		/* for default (public) visibility */
#include "endpoints.h"

#define API_VER			"Accept: application/vnd.hmrc.1.0+json"

/*
 * [GET ]
 * /obligations/details/{nino}/end-of-period-statement[?[typeOfBusiness={self-employment,uk-property,foreign-property}][&[businessId=]][&[fromDate=YYYY-MM-DD]][&[toDate=YYYY-MM-DD]][&[status={Open,Fulfilled}]]]
 */
int mtd_ob_list_end_of_period_obligations(const char *query_string, char **buf)
{
	return do_ep(OB_LIST_END_OF_PERIOD_OBLIGATIONS, API_VER,
		     NULL, buf, query_string, (char *)NULL);
}

/*
 * [GET ]
 * /obligations/details/{nino}/crystallisation[?taxYear=YYYY-MM]
 */
int mtd_ob_list_crystallisation_obligations(const char *query_string,
					    char **buf)
{
	return do_ep(OB_LIST_CRYSTALLISATION_OBLIGATIONS, API_VER,
		     NULL, buf, query_string, (char *)NULL);
}

/*
 * [GET ]
 * /obligations/details/{nino}/income-and-expenditure[?[typeOfBusiness={self-employment,uk-property,foreign-property}][&[businessId=]][&[fromDate=YYYY-MM-DD]][&[toDate=YYYY-MM-DD]][&[status={Open,Fulfilled}]]]
 */
int mtd_ob_list_inc_and_expend_obligations(const char *query_string,
					   char **buf)
{
	return do_ep(OB_LIST_INC_AND_EXPEND_OBLIGATIONS, API_VER,
		     NULL, buf, query_string, (char *)NULL);
}

