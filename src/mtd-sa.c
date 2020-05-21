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
 * [GET ]
 * /self-assessment/ni/{nino}/self-employments/{selfEmploymentId}/end-of-period-statements/obligations
 *
 * Optional query string:
 *
 * 	?from=YYYY-MM-DD&to=YYYY-MM-DD
 */
int mtd_sa_get_end_of_period_statement(const char *seid,
				       const char *query_string, char **buf)
{
	struct curl_ctx ctx = { 0 };
	const char *params[] = { seid, query_string };

	ctx.mtd_api_ver = API_VER;
	ctx.endpoint = SA_GET_END_OF_PERIOD_STATEMENT;
	ctx.params = params;

	return do_get(&ctx, buf);
}

/*
 * [POST]
 * /self-assessment/ni/{nino}/self-employments/{selfEmploymentId}/end-of-period-statements/from/{start}/to/{end}
 */
int mtd_sa_submit_end_of_period_statement(const char *src_file,
					  const char *seid,
					  const char *start,
					  const char *end, char **buf)
{
	struct curl_ctx ctx = { 0 };
	const char *params[] = { seid, start, end };

	ctx.mtd_api_ver = API_VER;
	ctx.endpoint = SA_SUBMIT_END_OF_PERIOD_STATEMENT;
	ctx.params = params;

	return do_post(&ctx, src_file, NULL, buf);
}

/*
 * [PUT ]
 * /self-assessment/ni/{nino}/self-employments/{selfEmploymentId}/{taxYear}
 */
int mtd_sa_update_annual_summary(const char *src_file, const char *seid,
				 const char *tax_year, char **buf)
{
	struct curl_ctx ctx = { 0 };
	const char *params[] = { seid, tax_year };

	ctx.mtd_api_ver = API_VER;
	ctx.endpoint = SA_UPDATE_ANNUAL_SUMMARY;
	ctx.params = params;

	return do_put(&ctx, src_file, NULL, buf);
}

/*
 * [GET ]
 * /self-assessment/ni/{nino}/self-employments/{selfEmploymentId}/{taxYear}
 */
int mtd_sa_get_annual_summary(const char *seid, const char *tax_year, char **buf)
{
	struct curl_ctx ctx = { 0 };
	const char *params[] = { seid, tax_year };

	ctx.mtd_api_ver = API_VER;
	ctx.endpoint = SA_GET_ANNUAL_SUMMARY;
	ctx.params = params;

	return do_get(&ctx, buf);
}

/*
 * [PUT ]
 * /self-assessment/ni/{nino}/self-employments/{selfEmploymentId}/periods/{periodId}
 */
int mtd_sa_update_period(const char *src_file, const char *seid,
			 const char *period_id, char **buf)
{
	struct curl_ctx ctx = { 0 };
	const char *params[] = { seid, period_id };

	ctx.mtd_api_ver = API_VER;
	ctx.endpoint = SA_UPDATE_PERIOD;
	ctx.params = params;

	return do_put(&ctx, src_file, NULL, buf);
}

/*
 * [GET ]
 * /self-assessment/ni/{nino}/self-employments/{selfEmploymentId}/periods/{periodId}
 */
int mtd_sa_get_period(const char *seid, const char *period_id, char **buf)
{
	struct curl_ctx ctx = { 0 };
	const char *params[] = { seid, period_id };

	ctx.mtd_api_ver = API_VER;
	ctx.endpoint = SA_GET_PERIOD;
	ctx.params = params;

	return do_get(&ctx, buf);
}

/*
 * [POST]
 * /self-assessment/ni/{nino}/self-employments/{selfEmploymentId}/periods
 */
int mtd_sa_create_period(const char *src_file, const char *seid, char **buf)
{
	struct curl_ctx ctx = { 0 };
	const char *params[] = { seid };

	ctx.mtd_api_ver = API_VER;
	ctx.endpoint = SA_CREATE_PERIOD;
	ctx.params = params;

	return do_post(&ctx, src_file, NULL, buf);
}

/*
 * [GET ]
 * /self-assessment/ni/{nino}/self-employments/{selfEmploymentId}/periods
 */
int mtd_sa_list_periods(const char *seid, char **buf)
{
	struct curl_ctx ctx = { 0 };
	const char *params[] = { seid };

	ctx.mtd_api_ver = API_VER;
	ctx.endpoint = SA_LIST_PERIODS;
	ctx.params = params;

	return do_get(&ctx, buf);
}

/*
 * [GET ]
 * /self-assessment/ni/{nino}/self-employments/{selfEmploymentId}/obligations
 */
int mtd_sa_list_obligations(const char *seid, char **buf)
{
	struct curl_ctx ctx = { 0 };
	const char *params[] = { seid };

	ctx.mtd_api_ver = API_VER;
	ctx.endpoint = SA_LIST_OBLIGATIONS;
	ctx.params = params;

	return do_get(&ctx, buf);
}

/*
 * [GET ]
 * /self-assessment/ni/{nino}/self-employments/{selfEmploymentId}
 */
int mtd_sa_get_employment(const char *seid, char **buf)
{
	struct curl_ctx ctx = { 0 };
	const char *params[] = { seid };

	ctx.mtd_api_ver = API_VER;
	ctx.endpoint = SA_GET_SELF_EMPLOYMENT;
	ctx.params = params;

	return do_get(&ctx, buf);
}

/*
 * [GET ]
 * /self-assessment/ni/{nino}/self-employments
 */
int mtd_sa_list_employments(char **buf)
{
	struct curl_ctx ctx = { 0 };

	ctx.mtd_api_ver = API_VER;
	ctx.endpoint = SA_LIST_SELF_EMPLOYMENTS;

	return do_get(&ctx, buf);
}
