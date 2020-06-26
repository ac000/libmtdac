/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * mtd-ic.c - Make Tax Digital - Individual Calculations API
 *
 * Copyright (C) 2020		Andrew Clayton <andrew@digital-domain.net>
 */

#include <stddef.h>

#include "mtd-ic.h"		/* for default (public) visibility */
#include "endpoints.h"

#define API_VER			"Accept: application/vnd.hmrc.1.0+json"

/*
 * [GET ]
 * /individuals/calculations/{nino}/self-assessment/{calculationId}/messages
 */
int mtd_ic_get_messages(const char *cid, char **buf)
{
	return do_ep(IC_GET_MSGS, API_VER,
		     NULL, buf, cid, (char *)NULL);
}

/*
 * [GET ]
 * /individuals/calculations/{nino}/self-assessment/{calculationId}/end-of-year-estimate
 */
int mtd_ic_get_end_of_year_est(const char *cid, char **buf)
{
	return do_ep(IC_GET_EOY_EST, API_VER,
		     NULL, buf, cid, (char *)NULL);
}

/*
 * [GET ]
 * /individuals/calculations/{nino}/self-assessment/{calculationId}/allowances-deductions-reliefs
 */
int mtd_ic_get_allowances_deductions_reliefs(const char *cid, char **buf)
{
	return do_ep(IC_GET_ALLOWANCES_DEDUCT_RELIEFS, API_VER,
		     NULL, buf, cid, (char *)NULL);
}

/*
 * [GET ]
 * /individuals/calculations/{nino}/self-assessment/{calculationId}/taxable-income
 */
int mtd_ic_get_taxable_income(const char *cid, char **buf)
{
	return do_ep(IC_GET_TAXABLE_INCOME, API_VER,
		     NULL, buf, cid, (char *)NULL);
}

/*
 * [GET ]
 * /individuals/calculations/{nino}/self-assessment/{calculationId}/income-tax-nics-calculated
 */
int mtd_ic_get_income_tax_nics_calc(const char *cid, char **buf)
{
	return do_ep(IC_GET_INCOME_TAX_NICS_CALC, API_VER,
		     NULL, buf, cid, (char *)NULL);
}

/*
 * [GET ]
 * /individuals/calculations/{nino}/self-assessment/{calculationId}
 */
int mtd_ic_get_calculation_meta(const char *cid, char **buf)
{
	return do_ep(IC_GET_CALCULATION_META, API_VER,
		     NULL, buf, cid, (char *)NULL);
}

/*
 * [POST]
 * /individuals/calculations/{nino}/self-assessment
 */
int mtd_ic_trigger_calculation(const struct mtd_dsrc_ctx *dsctx, char **buf)
{
	return do_ep(IC_TRIGGER_CALCULATION, API_VER,
		     dsctx, buf, (char *)NULL);
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
