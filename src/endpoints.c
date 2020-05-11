/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * endpoints.c - Make Tax Digital - Self Assessment API
 *
 * Copyright (C) 2020		Andrew Clayton <andrew@digital-domain.net>
 */

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "endpoints.h"
#include "auth.h"

#define BASE_URL		"https://test-api.service.hmrc.gov.uk"

static const struct _endpoint {
	enum endpoint ep;
	const char *template;
} endpoints[] = {
	{ SA_LIST_SELF_EMPLOYMENTS,
	 "/self-assessment/ni/{nino}/self-employments" },
	{ SA_GET_SELF_EMPLOYMENT,
	 "/self-assessment/ni/{nino}/self-employments/{selfEmploymentId}" },
	{ SA_LIST_OBLIGATIONS,
	 "/self-assessment/ni/{nino}/self-employments/{selfEmploymentId}/obligations" },
	{ SA_LIST_PERIODS,
	 "/self-assessment/ni/{nino}/self-employments/{selfEmploymentId}/periods" },
	{ SA_CREATE_PERIOD,
	 "/self-assessment/ni/{nino}/self-employments/{selfEmploymentId}/periods" },
	{ SA_GET_PERIOD,
	 "/self-assessment/ni/{nino}/self-employments/{selfEmploymentId}/periods/{periodId}" },
	{ SA_UPDATE_PERIOD,
	 "/self-assessment/ni/{nino}/self-employments/{selfEmploymentId}/periods/{periodId}" },
	{ SA_GET_ANNUAL_SUMMARY,
	 "/self-assessment/ni/{nino}/self-employments/{selfEmploymentId}/{taxYear}" },
	{ SA_UPDATE_ANNUAL_SUMMARY,
	 "/self-assessment/ni/{nino}/self-employments/{selfEmploymentId}/{taxYear}" },
	{ SA_SUBMIT_END_OF_PERIOD_STATEMENT,
	 "/self-assessment/ni/{nino}/self-employments/{selfEmploymentId}/end-of-period-statements/from/{start}/to/{end}" },
	{ SA_GET_END_OF_PERIOD_STATEMENT,
	 "/self-assessment/ni/{nino}/self-employments/{selfEmploymentId}/end-of-period-statements/obligations/{optional_query_params}" },
	{ OA_REFRESH_TOKEN, "/oauth/token" },
	{ OA_EXCHANGE_AUTH_CODE, "/oauth/token" },
};

char *ep_make_url(enum endpoint ep, const char **params, char *url)
{
	char *nino = load_token("nino", FT_CONFIG);
	char *seid = load_token("seid", FT_CONFIG);
	char *string;
	char *ptr;
	int len;
	int p = 0;

	string = strdup(endpoints[ep].template + 1); /* skip past first '/' */
	ptr = string;

	len = snprintf(url, URL_LEN + 1, BASE_URL);

	for (;;) {
		char *token;

		token = strsep(&ptr, "/");
		if (!token)
			break;

		if (*token != '{')
			len += snprintf(url + len, URL_LEN+1 - len, "/%s",
					token);
		else if (strcmp(token, "{nino}") == 0)
			len += snprintf(url + len, URL_LEN+1 - len, "/%s",
					nino);
		else if (strcmp(token, "{selfEmploymentId}") == 0)
			len += snprintf(url + len, URL_LEN+1 - len, "/%s",
					seid);
		else if (strcmp(token, "{optional_query_params}") == 0)
			len += snprintf(url + len, URL_LEN+1 - len, "%s",
					params[p] ? params[p++] : "");
		else if (*token == '{')
			len += snprintf(url + len, URL_LEN+1 - len, "/%s",
					params[p++]);
	}

	free(string);
	free(nino);
	free(seid);

	return url;
}
