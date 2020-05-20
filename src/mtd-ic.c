/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * mtd-ic.c - Make Tax Digital - Individual Calculations API
 *
 * Copyright (C) 2020		Andrew Clayton <andrew@digital-domain.net>
 */

#include <stdio.h>

#include "mtd-ic.h"		/* for default (public) visibility */
#include "endpoints.h"
#include "curler.h"

#define API_VER			"Accept: application/vnd.hmrc.1.0+json"

/*
 * [GET ]
 * /individuals/calculations/{nino}/self-assessment/{calculationId}/messages
 */
int mtd_ic_get_messages(const char *cid, char **buf)
{
	struct curl_ctx ctx = { 0 };
	const char *params[1] = { cid };

	ctx.mtd_api_ver = API_VER;
	ctx.endpoint = IC_GET_MSGS;
	ctx.params = params;

	return do_get(&ctx, buf);
}

/*
 * [GET ]
 * /individuals/calculations/{nino}/self-assessment/{calculationId}/end-of-year-estimate
 */
int mtd_ic_get_end_of_year_est(const char *cid, char **buf)
{
	struct curl_ctx ctx = { 0 };
	const char *params[1] = { cid };

	ctx.mtd_api_ver = API_VER;
	ctx.endpoint = IC_GET_EOY_EST;;
	ctx.params = params;

	return do_get(&ctx, buf);
}

/*
 * [GET ]
 * /individuals/calculations/{nino}/self-assessment/{calculationId}/allowances-deductions-reliefs
 */
int mtd_ic_get_allowances_deductions_reliefs(const char *cid, char **buf)
{
	struct curl_ctx ctx = { 0 };
	const char *params[1] = { cid };

	ctx.mtd_api_ver = API_VER;
	ctx.endpoint = IC_GET_ALLOWANCES_DEDUCT_RELIEFS;
	ctx.params = params;

	return do_get(&ctx, buf);
}

/*
 * [GET ]
 * /individuals/calculations/{nino}/self-assessment/{calculationId}/taxable-income
 */
int mtd_ic_get_taxable_income(const char *cid, char **buf)
{
	struct curl_ctx ctx = { 0 };
	const char *params[1] = { cid };

	ctx.mtd_api_ver = API_VER;
	ctx.endpoint = IC_GET_TAXABLE_INCOME;
	ctx.params = params;

	return do_get(&ctx, buf);
}

/*
 * [GET ]
 * /individuals/calculations/{nino}/self-assessment/{calculationId}/income-tax-nics-calculated
 */
int mtd_ic_get_income_tax_nics_calc(const char *cid, char **buf)
{
	struct curl_ctx ctx = { 0 };
	const char *params[1] = { cid };

	ctx.mtd_api_ver = API_VER;
	ctx.endpoint = IC_GET_INCOME_TAX_NICS_CALC;
	ctx.params = params;

	return do_get(&ctx, buf);
}

/*
 * [GET ]
 * /individuals/calculations/{nino}/self-assessment/{calculationId}
 */
int mtd_ic_get_calculation_meta(const char *cid, char **buf)
{
	struct curl_ctx ctx = { 0 };
	const char *params[1] = { cid };

	ctx.mtd_api_ver = API_VER;
	ctx.endpoint = IC_GET_CALCULATION_META;
	ctx.params = params;

	return do_get(&ctx, buf);
}

/*
 * [POST]
 * /individuals/calculations/{nino}/self-assessment
 */
int mtd_ic_trigger_calculation(const char *src_file, char **buf)
{
	struct curl_ctx ctx = { 0 };

	ctx.mtd_api_ver = API_VER;
	ctx.endpoint = IC_TRIGGER_CALCULATION;

	return do_post(&ctx, src_file, NULL, buf);
}

/*
 * [GET ]
 * /individuals/calculations/{nino}/self-assessment
 */
int mtd_ic_list_calculations(char **buf)
{
	struct curl_ctx ctx = { 0 };

	ctx.mtd_api_ver = API_VER;
	ctx.endpoint = IC_LIST_CALCULATIONS;

	return do_get(&ctx, buf);
}
