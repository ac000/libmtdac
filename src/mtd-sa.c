/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * mtd-sa.c - Make Tax Digital - Self Assessment API
 *
 * Copyright (C) 2020		Andrew Clayton <andrew@digital-domain.net>
 */

#include <stdio.h>

#include "mtd-sa.h"
#include "endpoints.h"
#include "curl.h"

#define MTD_SA_API_VER		"Accept: application/vnd.hmrc.2.0+json"

/*
 * [GET ]
 * /self-assessment/ni/{nino}/self-employments/{selfEmploymentId}/end-of-period-statements/obligations
 *
 * This is a bit of an odd one out, as it's the only one that
 * takes an *optional* query string.
 */
int mtd_sa_get_end_of_period_statement(const char *start, const char *end,
				       char **buf)
{
	struct curl_ctx ctx = { 0 };
	const char *params[1] = { NULL };
	char query_string[64];

	ctx.mtd_api_ver = MTD_SA_API_VER;
	ctx.endpoint = SA_GET_END_OF_PERIOD_STATEMENT;
	ctx.params = params;

	if (start || end) {
		snprintf(query_string, sizeof(query_string), "?%s%s%s%s%s",
			 start ? "from=" : "", start ? start : "",
			 (start && end) ? "&" : "",
			 end ? "to=" : "", end ? end : "");
		params[0] = query_string;
	}

	return do_get(&ctx, buf);
}

/*
 * [POST]
 * /self-assessment/ni/{nino}/self-employments/{selfEmploymentId}/end-of-period-statements/from/{start}/to/{end}
 */
int mtd_sa_submit_end_of_period_statement(const char *src_file,
					  const char *start,
					  const char *end, char **buf)
{
	struct curl_ctx ctx = { 0 };
	const char *params[2] = { start, end };

	ctx.mtd_api_ver = MTD_SA_API_VER;
	ctx.endpoint = SA_SUBMIT_END_OF_PERIOD_STATEMENT;
	ctx.params = params;

	return do_post(&ctx, src_file, NULL, buf);
}

/*
 * [PUT ]
 * /self-assessment/ni/{nino}/self-employments/{selfEmploymentId}/{taxYear}
 */
int mtd_sa_update_annual_summary(const char *src_file, const char *tax_year,
				 char **buf)
{
	struct curl_ctx ctx = { 0 };
	const char *params[1] = { tax_year };

	ctx.mtd_api_ver = MTD_SA_API_VER;
	ctx.endpoint = SA_UPDATE_ANNUAL_SUMMARY;
	ctx.params = params;

	return do_put(&ctx, src_file, NULL, buf);
}

/*
 * [GET ]
 * /self-assessment/ni/{nino}/self-employments/{selfEmploymentId}/{taxYear}
 */
int mtd_sa_get_annual_summary(const char *tax_year, char **buf)
{
	struct curl_ctx ctx = { 0 };
	const char *params[1] = { tax_year };

	ctx.mtd_api_ver = MTD_SA_API_VER;
	ctx.endpoint = SA_GET_ANNUAL_SUMMARY;
	ctx.params = params;

	return do_get(&ctx, buf);
}

/*
 * [PUT ]
 * /self-assessment/ni/{nino}/self-employments/{selfEmploymentId}/periods/{periodId}
 */
int mtd_sa_update_period(const char *src_file, const char *period_id,
			 char **buf)
{
	struct curl_ctx ctx = { 0 };
	const char *params[1] = { period_id };

	ctx.mtd_api_ver = MTD_SA_API_VER;
	ctx.endpoint = SA_UPDATE_PERIOD;
	ctx.params = params;

	return do_put(&ctx, src_file, NULL, buf);
}

/*
 * [GET ]
 * /self-assessment/ni/{nino}/self-employments/{selfEmploymentId}/periods/{periodId}
 */
int mtd_sa_get_period(const char *period_id, char **buf)
{
	struct curl_ctx ctx = { 0 };
	const char *params[1] = { period_id };

	ctx.mtd_api_ver = MTD_SA_API_VER;
	ctx.endpoint = SA_GET_PERIOD;
	ctx.params = params;

	return do_get(&ctx, buf);
}

/*
 * [POST]
 * /self-assessment/ni/{nino}/self-employments/{selfEmploymentId}/periods
 */
int mtd_sa_create_period(const char *src_file, char **buf)
{
	struct curl_ctx ctx = { 0 };

	ctx.mtd_api_ver = MTD_SA_API_VER;
	ctx.endpoint = SA_CREATE_PERIOD;

	return do_post(&ctx, src_file, NULL, buf);
}

/*
 * [GET ]
 * /self-assessment/ni/{nino}/self-employments/{selfEmploymentId}/periods
 */
int mtd_sa_list_periods(char **buf)
{
	struct curl_ctx ctx = { 0 };

	ctx.mtd_api_ver = MTD_SA_API_VER;
	ctx.endpoint = SA_LIST_PERIODS;

	return do_get(&ctx, buf);
}

/*
 * [GET ]
 * /self-assessment/ni/{nino}/self-employments/{selfEmploymentId}/obligations
 */
int mtd_sa_list_obligations(char **buf)
{
	struct curl_ctx ctx = { 0 };

	ctx.mtd_api_ver = MTD_SA_API_VER;
	ctx.endpoint = SA_LIST_OBLIGATIONS;

	return do_get(&ctx, buf);
}

/*
 * [GET ]
 * /self-assessment/ni/{nino}/self-employments/{selfEmploymentId}
 */
int mtd_sa_get_employment(const char *employment_id, char **buf)
{
	struct curl_ctx ctx = { 0 };
	const char *params[1] = { employment_id };

	ctx.mtd_api_ver = MTD_SA_API_VER;
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

	ctx.mtd_api_ver = MTD_SA_API_VER;
	ctx.endpoint = SA_LIST_SELF_EMPLOYMENTS;

	return do_get(&ctx, buf);
}
