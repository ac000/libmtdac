/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * mtd-ep-ic.c - Make Tax Digital - Individual Calculations API
 *
 * Copyright (C) 2020 - 2022	Andrew Clayton <andrew@digital-domain.net>
 */

#include <stddef.h>

#include "mtd-ic.h"		/* for default (public) visibility */
#include "endpoints.h"

#define VERSION		"3.0"
#define API_VER		"Accept: application/vnd.hmrc." VERSION "+json"

/*
 * [POST]
 * /individuals/calculations/{nino}/self-assessment/{taxYear}/{calculationId}
 */
int mtd_ic_final_decl(const char *tax_year, const char *cid, char **buf)
{
	return do_ep(IC_FINAL_DECLARATION, API_VER,
		     NULL, buf, tax_year, cid, (char *)NULL);
}

/*
 * [GET ]
 * /individuals/calculations/{nino}/self-assessment/{taxYear}/{calculationId}
 */
int mtd_ic_get_calculation(const char *tax_year, const char *cid,
			      char **buf)
{
	return do_ep(IC_GET_CALCULATION, API_VER,
		     NULL, buf, tax_year, cid, (char *)NULL);
}

/*
 * [POST]
 * /individuals/calculations/{nino}/self-assessment/{taxYear}[?finalDeclaration={true,false}]
 */
int mtd_ic_trigger_calculation(const char *tax_year, const char *query_string,
			       char **buf)
{
	return do_ep(IC_TRIGGER_CALCULATION, API_VER,
		     NULL, buf, tax_year, query_string, (char *)NULL);
}

/*
 * [GET ]
 * /individuals/calculations/{nino}/self-assessment[?taxYear=YYYY-YY]
 */
int mtd_ic_list_calculations(const char *query_string, char **buf)
{
	return do_ep(IC_LIST_CALCULATIONS, API_VER,
		     NULL, buf, query_string, (char *)NULL);
}
