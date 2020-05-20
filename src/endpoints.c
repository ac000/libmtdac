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

#include "mtd-priv.h"
#include "endpoints.h"
#include "auth.h"

/*
 * The order of these entries must match the order in enum endpoint
 * in endpoints.h
 */
static const struct _endpoint {
	const enum endpoint ep;
	const char *template;
} endpoints[] = {
	/* Self-Assement */
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

	/* Self-Assessment Accounts */
	{ SAAC_GET_BALANCE, "/accounts/self-assessment/{nino}/balance" },
	{ SAAC_LIST_TRANSACTIONS,
	 "/accounts/self-assessment/{nino}/transactions/{query_params}" },
	{ SAAC_GET_TRANSACTION,
	 "/accounts/self-assessment/{nino}/transactions/{transactionId}" },
	{ SAAC_LIST_CHARGES,
	 "/accounts/self-assessment/{nino}/charges/{query_params}" },
	{ SAAC_GET_CHARGE,
	 "/accounts/self-assessment/{nino}/charges/{transactionId}" },
	{ SAAC_LIST_PAYMENTS,
	 "/accounts/self-assessment/{nino}/payments/{query_params}" },
	{ SAAC_GET_PAYMENT,
	 "/accounts/self-assessment/{nino}/payments/{paymentId}" },

	/* Individual Calculations */
	{ IC_LIST_CALCULATIONS,
	 "/individuals/calculations/{nino}/self-assessment" },
	{ IC_TRIGGER_CALCULATION,
	 "/individuals/calculations/{nino}/self-assessment" },
	{ IC_GET_CALCULATION_META,
	 "/individuals/calculations/{nino}/self-assessment/{calculationId}" },
	{ IC_GET_INCOME_TAX_NICS_CALC,
	 "/individuals/calculations/{nino}/self-assessment/{calculationId}/income-tax-nics-calculated" },
	{ IC_GET_TAXABLE_INCOME,
	 "/individuals/calculations/{nino}/self-assessment/{calculationId}/taxable-income" },
	{ IC_GET_ALLOWANCES_DEDUCT_RELIEFS,
	 "/individuals/calculations/{nino}/self-assessment/{calculationId}/allowances-deductions-reliefs" },
	{ IC_GET_EOY_EST,
	 "/individuals/calculations/{nino}/self-assessment/{calculationId}/end-of-year-estimate" },
	{ IC_GET_MSGS,
	 "/individuals/calculations/{nino}/self-assessment/{calculationId}/messages" },

	/* National Insurance */
	{ NI_GET_ANNUAL_SUMMARY,
	 "/national-insurance/sa/{utr}/annual-summary/{taxYear}" },

	/* Create Test User */
	{ TEST_CU_CREATE_INDIVIDUAL, "/create-test-user/individuals" },
	{ TEST_CU_CREATE_ORGANISATION, "/create-test-user/organisations" },
	{ TEST_CU_CREATE_AGENT, "/create-test-user/agents" },
	{ TEST_CU_LIST_SERVICES, "/create-test-user/services" },

	/* OAuth */
	{ OA_REFRESH_TOKEN, "/oauth/token" },
	{ OA_EXCHANGE_AUTH_CODE, "/oauth/token" },
};

char *ep_make_url(enum endpoint ep, const char **params, char *url)
{
	char *nino = load_token("nino", FT_CONFIG);
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
					params[p++]);
		else if (strstr(token, "query_params"))
			len += snprintf(url + len, URL_LEN+1 - len, "%s",
					params[p] ? params[p++] : "");
		else if (*token == '{')
			len += snprintf(url + len, URL_LEN+1 - len, "/%s",
					params[p++]);
	}

	free(string);
	free(nino);

	return url;
}
