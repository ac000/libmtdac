/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * mtd-ic.c - Make Tax Digital - Individual Calculations API
 *
 * Copyright (C) 2020 - 2021	Andrew Clayton <andrew@digital-domain.net>
 */

#include <stddef.h>

#include "mtd-ic.h"		/* for default (public) visibility */
#include "endpoints.h"

#define API_VER			"Accept: application/vnd.hmrc.1.0+json"

/*
 * [POST]
 * /individuals/calculations/crystallisation/{nino}/{taxYear}/crystallise
 */
int mtd_ic_cr_crystallise(const struct mtd_dsrc_ctx *dsctx,
			  const char *tax_year, char **buf)
{
	return do_ep(IC_CR_CRYSTALLISE, API_VER,
		     dsctx, buf, tax_year, (char *)NULL);
}

/*
 * [POST]
 * /individuals/calculations/crystallisation/{nino}/{taxYear}/intent-to-crystallise
 */
int mtd_ic_cr_intent_to_crystallise(const char *tax_year, char **buf)
{
	return do_ep(IC_CR_INTENT_TO_CRYSTALLISE, API_VER,
		     NULL, buf, tax_year, (char *)NULL);
}

/*
 * [GET ]
 * /individuals/calculations/{nino}/self-assessment/{calculationId}/messages
 */
int mtd_ic_sa_get_messages(const char *cid, char **buf)
{
	return do_ep(IC_SA_GET_MSGS, API_VER,
		     NULL, buf, cid, (char *)NULL);
}

/*
 * [GET ]
 * /individuals/calculations/{nino}/self-assessment/{calculationId}/end-of-year-estimate
 */
int mtd_ic_sa_get_end_of_year_est(const char *cid, char **buf)
{
	return do_ep(IC_SA_GET_EOY_EST, API_VER,
		     NULL, buf, cid, (char *)NULL);
}

/*
 * [GET ]
 * /individuals/calculations/{nino}/self-assessment/{calculationId}/allowances-deductions-reliefs
 */
int mtd_ic_sa_get_allowances_deductions_reliefs(const char *cid, char **buf)
{
	return do_ep(IC_SA_GET_ALLOWANCES_DEDUCT_RELIEFS, API_VER,
		     NULL, buf, cid, (char *)NULL);
}

/*
 * [GET ]
 * /individuals/calculations/{nino}/self-assessment/{calculationId}/taxable-income
 */
int mtd_ic_sa_get_taxable_income(const char *cid, char **buf)
{
	return do_ep(IC_SA_GET_TAXABLE_INCOME, API_VER,
		     NULL, buf, cid, (char *)NULL);
}

/*
 * [GET ]
 * /individuals/calculations/{nino}/self-assessment/{calculationId}/income-tax-nics-calculated
 */
int mtd_ic_sa_get_income_tax_nics_calc(const char *cid, char **buf)
{
	return do_ep(IC_SA_GET_INCOME_TAX_NICS_CALC, API_VER,
		     NULL, buf, cid, (char *)NULL);
}

/*
 * [GET ]
 * /individuals/calculations/{nino}/self-assessment/{calculationId}
 */
int mtd_ic_sa_get_calculation_meta(const char *cid, char **buf)
{
	return do_ep(IC_SA_GET_CALCULATION_META, API_VER,
		     NULL, buf, cid, (char *)NULL);
}

/*
 * [POST]
 * /individuals/calculations/{nino}/self-assessment
 */
int mtd_ic_sa_trigger_calculation(const struct mtd_dsrc_ctx *dsctx, char **buf)
{
	return do_ep(IC_SA_TRIGGER_CALCULATION, API_VER,
		     dsctx, buf, (char *)NULL);
}

/*
 * [GET ]
 * /individuals/calculations/{nino}/self-assessment[?taxYear=YYYY-YY]
 */
int mtd_ic_sa_list_calculations(const char *query_string, char **buf)
{
	return do_ep(IC_SA_LIST_CALCULATIONS, API_VER,
		     NULL, buf, query_string, (char *)NULL);
}
