/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * mtd-sa.c - Make Tax Digital - Self Assessment API
 *
 * Copyright (C) 2020		Andrew Clayton <andrew@digital-domain.net>
 */

#include "mtd-sa.h"		/* for default (public) visibility */
#include "endpoints.h"
#include "curler.h"

#define API_VER			"Accept: application/vnd.hmrc.2.0+json"

/*
 * [POST]
 * /self-assessment/ni/{nino}/crystallisation/obligations
 */
int mtd_sa_cr_list_obligations(char **buf)
{
	struct curl_ctx ctx = { 0 };

	ctx.mtd_api_ver = API_VER;
	ctx.endpoint = SA_CR_LIST_OBLIGATIONS;

	return do_post(&ctx, NULL, NULL, buf);
}

/*
 * [POST]
 * /self-assessment/ni/{nino}/{taxYear}/crystallisation
 */
int mtd_sa_cr_crystallise(const char *tax_year, char **buf)
{
	struct curl_ctx ctx = { 0 };
	const char *params[] = { tax_year };

	ctx.mtd_api_ver = API_VER;
	ctx.endpoint = SA_CR_CRYSTALLISE;
	ctx.params = params;

	return do_post(&ctx, NULL, NULL, buf);
}

/*
 * [POST]
 * /self-assessment/ni/{nino}/{taxYear}/intent-to-crystallise
 */
int mtd_sa_cr_intent_to_crystallise(const char *tax_year, char **buf)
{
	struct curl_ctx ctx = { 0 };
	const char *params[] = { tax_year };

	ctx.mtd_api_ver = API_VER;
	ctx.endpoint = SA_CR_INTENT_TO_CRYSTALLISE;
	ctx.params = params;

	return do_post(&ctx, NULL, NULL, buf);
}

/*
 * [GET ]
 * /self-assessment/ni/{nino}/calculations/{calculationId}/validation-messages
 */
int mtd_sa_tc_get_validation_msgs(const char *cid, char **buf)
{
	struct curl_ctx ctx = { 0 };
	const char *params[] = { cid };

	ctx.mtd_api_ver = API_VER;
	ctx.endpoint = SA_TC_GET_VALIDATION_MSGS;
	ctx.params = params;

	return do_get(&ctx, buf);
}

/*
 * [GET ]
 * /self-assessment/ni/{nino}/calculations/{calculationId}
 */
int mtd_sa_tc_get_calculation(const char *cid, char **buf)
{
	struct curl_ctx ctx = { 0 };
	const char *params[] = { cid };

	ctx.mtd_api_ver = API_VER;
	ctx.endpoint = SA_TC_GET_CALCULATION;
	ctx.params = params;

	return do_get(&ctx, buf);
}

/*
 * [POST]
 * /self-assessment/ni/{nino}/calculations
 */
int mtd_sa_tc_calculate(const char *src_file, char **buf)
{
	struct curl_ctx ctx = { 0 };

	ctx.mtd_api_ver = API_VER;
	ctx.endpoint = SA_TC_CALCULATE;

	return do_post(&ctx, src_file, NULL, buf);
}

/*
 * [PUT ]
 * /self-assessment/ni/{nino}/charitable-giving/{taxYear}
 */
int mtd_sa_cg_update_charitable_giving(const char *src_file,
				       const char *tax_year, char **buf)
{
	struct curl_ctx ctx = { 0 };
	const char *params[] = { tax_year };

	ctx.mtd_api_ver = API_VER;
	ctx.endpoint = SA_CG_UPDATE_CHARITABLE_GIVING;
	ctx.params = params;

	return do_put(&ctx, src_file, NULL, buf);
}

/*
 * [GET ]
 * /self-assessment/ni/{nino}/charitable-giving/{taxYear}
 */
int mtd_sa_cg_get_charitable_giving(const char *tax_year, char **buf)
{
	struct curl_ctx ctx = { 0 };
	const char *params[] = { tax_year };

	ctx.mtd_api_ver = API_VER;
	ctx.endpoint = SA_CG_GET_CHARITABLE_GIVING;
	ctx.params = params;

	return do_get(&ctx, buf);
}

/*
 * [PUT ]
 * /self-assessment/ni/{nino}/savings-accounts/{savingsAccountId}/{taxYear}
 */
int mtd_sa_sa_update_annual_summary(const char *src_file, const char *said,
				    const char *tax_year, char **buf)
{
	struct curl_ctx ctx = { 0 };
	const char *params[] = { said, tax_year };

	ctx.mtd_api_ver = API_VER;
	ctx.endpoint = SA_SA_UPDATE_ANNUAL_SUMMARY;
	ctx.params = params;

	return do_put(&ctx, src_file, NULL, buf);
}

/*
 * [GET ]
 * /self-assessment/ni/{nino}/savings-accounts/{savingsAccountId}/{taxYear}
 */
int mtd_sa_sa_get_annual_summary(const char *said, const char *tax_year,
				 char **buf)
{
	struct curl_ctx ctx = { 0 };
	const char *params[] = { said, tax_year };

	ctx.mtd_api_ver = API_VER;
	ctx.endpoint = SA_SA_GET_ANNUAL_SUMMARY;
	ctx.params = params;

	return do_get(&ctx, buf);
}

/*
 * [GET ]
 * /self-assessment/ni/{nino}/savings-accounts/{savingsAccountId}
 */
int mtd_sa_sa_get_account(const char *said, char **buf)
{
	struct curl_ctx ctx = { 0 };
	const char *params[] = { said };

	ctx.mtd_api_ver = API_VER;
	ctx.endpoint = SA_SA_GET_ACCOUNT;
	ctx.params = params;

	return do_get(&ctx, buf);
}

/*
 * [POST]
 * /self-assessment/ni/{nino}/savings-accounts
 */
int mtd_sa_sa_create_account(const char *src_file, char **buf)
{
	struct curl_ctx ctx = { 0 };

	ctx.mtd_api_ver = API_VER;
	ctx.endpoint = SA_SA_CREATE_ACCOUNT;

	return do_post(&ctx, src_file, NULL, buf);
}

/*
 * [GET ]
 * /self-assessment/ni/{nino}/savings-accounts
 */
int mtd_sa_sa_list_accounts(char **buf)
{
	struct curl_ctx ctx = { 0 };

	ctx.mtd_api_ver = API_VER;
	ctx.endpoint = SA_SA_LIST_ACCOUNTS;

	return do_get(&ctx, buf);
}

/*
 * [PUT ]
 * /self-assessment/ni/{nino}/dividends/{taxYear}
 */
int mtd_sa_di_update_annual_summary(const char *src_file, const char *tax_year,
				    char **buf)
{
	struct curl_ctx ctx = { 0 };
	const char *params[] = { tax_year };

	ctx.mtd_api_ver = API_VER;
	ctx.endpoint = SA_DI_UPDATE_ANNUAL_SUMMARY;
	ctx.params = params;

	return do_put(&ctx, src_file, NULL, buf);
}

/*
 * [GET ]
 * /self-assessment/ni/{nino}/dividends/{taxYear}
 */
int mtd_sa_di_get_annual_summary(const char *tax_year, char **buf)
{
	struct curl_ctx ctx = { 0 };
	const char *params[] = { tax_year };

	ctx.mtd_api_ver = API_VER;
	ctx.endpoint = SA_DI_GET_ANNUAL_SUMMARY;
	ctx.params = params;

	return do_get(&ctx, buf);
}

/*
 * [GET ]
 * /self-assessment/ni/{nino}/self-employments/{selfEmploymentId}/end-of-period-statements/obligations
 *
 * Optional query string:
 *
 * 	?from=YYYY-MM-DD&to=YYYY-MM-DD
 */
int mtd_sa_se_get_end_of_period_statement(const char *seid,
					  const char *query_string, char **buf)
{
	struct curl_ctx ctx = { 0 };
	const char *params[] = { seid, query_string };

	ctx.mtd_api_ver = API_VER;
	ctx.endpoint = SA_SE_GET_END_OF_PERIOD_STATEMENT;
	ctx.params = params;

	return do_get(&ctx, buf);
}

/*
 * [POST]
 * /self-assessment/ni/{nino}/self-employments/{selfEmploymentId}/end-of-period-statements/from/{start}/to/{end}
 */
int mtd_sa_se_submit_end_of_period_statement(const char *src_file,
					     const char *seid,
					     const char *start,
					     const char *end, char **buf)
{
	struct curl_ctx ctx = { 0 };
	const char *params[] = { seid, start, end };

	ctx.mtd_api_ver = API_VER;
	ctx.endpoint = SA_SE_SUBMIT_END_OF_PERIOD_STATEMENT;
	ctx.params = params;

	return do_post(&ctx, src_file, NULL, buf);
}

/*
 * [PUT ]
 * /self-assessment/ni/{nino}/self-employments/{selfEmploymentId}/{taxYear}
 */
int mtd_sa_se_update_annual_summary(const char *src_file, const char *seid,
				    const char *tax_year, char **buf)
{
	struct curl_ctx ctx = { 0 };
	const char *params[] = { seid, tax_year };

	ctx.mtd_api_ver = API_VER;
	ctx.endpoint = SA_SE_UPDATE_ANNUAL_SUMMARY;
	ctx.params = params;

	return do_put(&ctx, src_file, NULL, buf);
}

/*
 * [GET ]
 * /self-assessment/ni/{nino}/self-employments/{selfEmploymentId}/{taxYear}
 */
int mtd_sa_se_get_annual_summary(const char *seid, const char *tax_year,
				 char **buf)
{
	struct curl_ctx ctx = { 0 };
	const char *params[] = { seid, tax_year };

	ctx.mtd_api_ver = API_VER;
	ctx.endpoint = SA_SE_GET_ANNUAL_SUMMARY;
	ctx.params = params;

	return do_get(&ctx, buf);
}

/*
 * [PUT ]
 * /self-assessment/ni/{nino}/self-employments/{selfEmploymentId}/periods/{periodId}
 */
int mtd_sa_se_update_period(const char *src_file, const char *seid,
			    const char *period_id, char **buf)
{
	struct curl_ctx ctx = { 0 };
	const char *params[] = { seid, period_id };

	ctx.mtd_api_ver = API_VER;
	ctx.endpoint = SA_SE_UPDATE_PERIOD;
	ctx.params = params;

	return do_put(&ctx, src_file, NULL, buf);
}

/*
 * [GET ]
 * /self-assessment/ni/{nino}/self-employments/{selfEmploymentId}/periods/{periodId}
 */
int mtd_sa_se_get_period(const char *seid, const char *period_id, char **buf)
{
	struct curl_ctx ctx = { 0 };
	const char *params[] = { seid, period_id };

	ctx.mtd_api_ver = API_VER;
	ctx.endpoint = SA_SE_GET_PERIOD;
	ctx.params = params;

	return do_get(&ctx, buf);
}

/*
 * [POST]
 * /self-assessment/ni/{nino}/self-employments/{selfEmploymentId}/periods
 */
int mtd_sa_se_create_period(const char *src_file, const char *seid, char **buf)
{
	struct curl_ctx ctx = { 0 };
	const char *params[] = { seid };

	ctx.mtd_api_ver = API_VER;
	ctx.endpoint = SA_SE_CREATE_PERIOD;
	ctx.params = params;

	return do_post(&ctx, src_file, NULL, buf);
}

/*
 * [GET ]
 * /self-assessment/ni/{nino}/self-employments/{selfEmploymentId}/periods
 */
int mtd_sa_se_list_periods(const char *seid, char **buf)
{
	struct curl_ctx ctx = { 0 };
	const char *params[] = { seid };

	ctx.mtd_api_ver = API_VER;
	ctx.endpoint = SA_SE_LIST_PERIODS;
	ctx.params = params;

	return do_get(&ctx, buf);
}

/*
 * [GET ]
 * /self-assessment/ni/{nino}/self-employments/{selfEmploymentId}/obligations
 */
int mtd_sa_se_list_obligations(const char *seid, char **buf)
{
	struct curl_ctx ctx = { 0 };
	const char *params[] = { seid };

	ctx.mtd_api_ver = API_VER;
	ctx.endpoint = SA_SE_LIST_OBLIGATIONS;
	ctx.params = params;

	return do_get(&ctx, buf);
}

/*
 * [GET ]
 * /self-assessment/ni/{nino}/self-employments/{selfEmploymentId}
 */
int mtd_sa_se_get_employment(const char *seid, char **buf)
{
	struct curl_ctx ctx = { 0 };
	const char *params[] = { seid };

	ctx.mtd_api_ver = API_VER;
	ctx.endpoint = SA_SE_GET_SELF_EMPLOYMENT;
	ctx.params = params;

	return do_get(&ctx, buf);
}

/*
 * [GET ]
 * /self-assessment/ni/{nino}/self-employments
 */
int mtd_sa_se_list_employments(char **buf)
{
	struct curl_ctx ctx = { 0 };

	ctx.mtd_api_ver = API_VER;
	ctx.endpoint = SA_SE_LIST_SELF_EMPLOYMENTS;

	return do_get(&ctx, buf);
}
