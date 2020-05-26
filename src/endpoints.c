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
#include <stdarg.h>

#include "mtd-priv.h"
#include "endpoints.h"
#include "curler.h"
#include "auth.h"
#include "logger.h"

#define MAX_PARAMS	7

/*
 * The order of these entries must match the order in enum endpoint
 * in endpoints.h
 */
static const struct _endpoint {
	const enum endpoint ep;
	const char *tmpl;
	const enum http_method method;
} endpoints[] = {
	/* Self-Assessment - Self-Employment */
	{
		SA_SE_LIST_SELF_EMPLOYMENTS,
		"/self-assessment/ni/{nino}/self-employments",
		M_GET
	},
	{
		SA_SE_GET_SELF_EMPLOYMENT,
		"/self-assessment/ni/{nino}/self-employments/{selfEmploymentId}",
		M_GET
	},
	{
		SA_SE_LIST_OBLIGATIONS,
		"/self-assessment/ni/{nino}/self-employments/{selfEmploymentId}/obligations",
		M_GET
	},
	{
		SA_SE_LIST_PERIODS,
		"/self-assessment/ni/{nino}/self-employments/{selfEmploymentId}/periods",
		M_GET
	},
	{
		SA_SE_CREATE_PERIOD,
		"/self-assessment/ni/{nino}/self-employments/{selfEmploymentId}/periods",
		M_POST
	},
	{
		SA_SE_GET_PERIOD,
		"/self-assessment/ni/{nino}/self-employments/{selfEmploymentId}/periods/{periodId}",
		M_GET
	},
	{
		SA_SE_UPDATE_PERIOD,
		"/self-assessment/ni/{nino}/self-employments/{selfEmploymentId}/periods/{periodId}",
		M_PUT
	},
	{
		SA_SE_GET_ANNUAL_SUMMARY,
		"/self-assessment/ni/{nino}/self-employments/{selfEmploymentId}/{taxYear}",
		M_GET
	},
	{
		SA_SE_UPDATE_ANNUAL_SUMMARY,
		"/self-assessment/ni/{nino}/self-employments/{selfEmploymentId}/{taxYear}",
		M_PUT
	},
	{
		SA_SE_SUBMIT_END_OF_PERIOD_STATEMENT,
		"/self-assessment/ni/{nino}/self-employments/{selfEmploymentId}/end-of-period-statements/from/{start}/to/{end}",
		M_POST
	},
	{
		SA_SE_GET_END_OF_PERIOD_STATEMENT,
		"/self-assessment/ni/{nino}/self-employments/{selfEmploymentId}/end-of-period-statements/obligations/{optional_query_params}",
		M_GET
	},
	/* Self-Assessment - Dividends Income */
	{
		SA_DI_GET_ANNUAL_SUMMARY,
		"/self-assessment/ni/{nino}/dividends/{taxYear}",
		M_GET
	},
	{
		SA_DI_UPDATE_ANNUAL_SUMMARY,
		"/self-assessment/ni/{nino}/dividends/{taxYear}",
		M_PUT
	},
	/* Self-Assessment - Savings Accounts */
	{
		SA_SA_LIST_ACCOUNTS,
		"/self-assessment/ni/{nino}/savings-accounts",
		M_GET
	},
	{
		SA_SA_CREATE_ACCOUNT,
		"/self-assessment/ni/{nino}/savings-accounts",
		M_POST
	},
	{
		SA_SA_GET_ACCOUNT,
		"/self-assessment/ni/{nino}/savings-accounts/{savingsAccountId}",
		M_GET
	},
	{
		SA_SA_GET_ANNUAL_SUMMARY,
		"/self-assessment/ni/{nino}/savings-accounts/{savingsAccountId}/{taxYear}",
		M_GET
	},
	{
		SA_SA_UPDATE_ANNUAL_SUMMARY,
		"/self-assessment/ni/{nino}/savings-accounts/{savingsAccountId}/{taxYear}",
		M_PUT
	},
	/* Self-Assessment - Charitable Giving */
	{
		SA_CG_GET_CHARITABLE_GIVING,
		"/self-assessment/ni/{nino}/charitable-giving/{taxYear}",
		M_GET
	},
	{
		SA_CG_UPDATE_CHARITABLE_GIVING,
		"/self-assessment/ni/{nino}/charitable-giving/{taxYear}",
		M_PUT
	},
	/* Self-Assessment - Tax Calculations - EOL Jul 2020 */
	{
		SA_TC_CALCULATE,
		"/self-assessment/ni/{nino}/calculations",
		M_POST
	},
	{
		SA_TC_GET_CALCULATION,
		"/self-assessment/ni/{nino}/calculations/{calculationId}",
		M_GET
	},
	{
		SA_TC_GET_VALIDATION_MSGS,
		"/self-assessment/ni/{nino}/calculations/{calculationId}/validation-messages",
		M_GET
	},
	/* Self-Assessment - Crystallisation */
	{
		SA_CR_INTENT_TO_CRYSTALLISE,
		"/self-assessment/ni/{nino}/{taxYear}/intent-to-crystallise",
		M_POST
	},
	{
		SA_CR_CRYSTALLISE,
		"/self-assessment/ni/{nino}/{taxYear}/crystallisation",
		M_POST
	},
	{
		SA_CR_LIST_OBLIGATIONS,
		"/self-assessment/ni/{nino}/crystallisation/obligations",
		M_POST
	},

	/* Self-Assessment Accounts */
	{
		SAAC_GET_BALANCE,
		"/accounts/self-assessment/{nino}/balance",
		M_GET
	},
	{
		SAAC_LIST_TRANSACTIONS,
		"/accounts/self-assessment/{nino}/transactions/{query_params}",
		M_GET
	},
	{
		SAAC_GET_TRANSACTION,
		"/accounts/self-assessment/{nino}/transactions/{transactionId}",
		M_GET
	},
	{
		SAAC_LIST_CHARGES,
		"/accounts/self-assessment/{nino}/charges/{query_params}",
		M_GET
	},
	{
		SAAC_GET_CHARGE,
		"/accounts/self-assessment/{nino}/charges/{transactionId}",
		M_GET
	},
	{
		SAAC_LIST_PAYMENTS,
		"/accounts/self-assessment/{nino}/payments/{query_params}",
		M_GET
	},
	{
		SAAC_GET_PAYMENT,
		"/accounts/self-assessment/{nino}/payments/{paymentId}",
		M_GET
	},

	/* Individual Calculations */
	{
		IC_LIST_CALCULATIONS,
		"/individuals/calculations/{nino}/self-assessment",
		M_GET
	},
	{
		IC_TRIGGER_CALCULATION,
		"/individuals/calculations/{nino}/self-assessment",
		M_POST
	},
	{
		IC_GET_CALCULATION_META,
		"/individuals/calculations/{nino}/self-assessment/{calculationId}",
		M_GET
	},
	{
		IC_GET_INCOME_TAX_NICS_CALC,
		"/individuals/calculations/{nino}/self-assessment/{calculationId}/income-tax-nics-calculated",
		M_GET
	},
	{
		IC_GET_TAXABLE_INCOME,
		"/individuals/calculations/{nino}/self-assessment/{calculationId}/taxable-income",
		M_GET
	},
	{
		IC_GET_ALLOWANCES_DEDUCT_RELIEFS,
		"/individuals/calculations/{nino}/self-assessment/{calculationId}/allowances-deductions-reliefs",
		M_GET
	},
	{
		IC_GET_EOY_EST,
		"/individuals/calculations/{nino}/self-assessment/{calculationId}/end-of-year-estimate",
		M_GET
	},
	{
		IC_GET_MSGS,
		"/individuals/calculations/{nino}/self-assessment/{calculationId}/messages",
		M_GET
	},

	/* Individual Loses - Brought Forward */
	{
		IL_BF_LIST_LOSES,
		"/individuals/losses/{nino}/brought-forward-losses/{optional_query_params}",
		M_GET
	},
	{
		IL_BF_CREATE_LOSS,
		"/individuals/losses/{nino}/brought-forward-losses",
		M_POST
	},
	{
		IL_BF_GET_LOSS,
		"/individuals/losses/{nino}/brought-forward-losses/{lossId}",
		M_GET
	},
	{
		IL_BF_DELETE_LOSS,
		"/individuals/losses/{nino}/brought-forward-losses/{lossId}",
		M_DELETE
	},
	{
		IL_BF_UPDATE_LOSS_AMNT,
		"/individuals/losses/{nino}/brought-forward-losses/{lossId}/change-loss-amount",
		M_POST
	},
	/* Individual Loses - Loss Claims */
	{
		IL_LC_LIST_LOSES,
		"/individuals/losses/{nino}/loss-claims/{optional_query_params}",
		M_GET
	},
	{
		IL_LC_CREATE_LOSS,
		"/individuals/losses/{nino}/loss-claims",
		M_POST
	},
	{
		IL_LC_GET_LOSS,
		"/individuals/losses/{nino}/loss-claims/{claimId}",
		M_GET
	},
	{
		IL_LC_DELETE_LOSS,
		"/individuals/losses/{nino}/loss-claims/{claimId}",
		M_DELETE
	},
	{
		IL_LC_UPDATE_LOSS_TYPE,
		"/individuals/losses/{nino}/loss-claims/{claimId}/change-type-of-claim",
		M_POST
	},
	{
		IL_LC_UPDATE_LOSS_ORDER,
		"/individuals/losses/{nino}/loss-claims/order",
		M_PUT
	},

	/* National Insurance */
	{
		NI_GET_ANNUAL_SUMMARY,
		"/national-insurance/sa/{utr}/annual-summary/{taxYear}",
		M_GET
	},

	/* Create Test User */
	{
		TEST_CU_CREATE_INDIVIDUAL,
		"/create-test-user/individuals",
		M_POST
	},
	{
		TEST_CU_CREATE_ORGANISATION,
		"/create-test-user/organisations",
		M_POST
	},
	{
		TEST_CU_CREATE_AGENT,
		"/create-test-user/agents",
		M_POST
	},
	{
		TEST_CU_LIST_SERVICES,
		"/create-test-user/services",
		M_GET
	},

	/* Test Fraud Prevention Headers */
	{
		TEST_FPH_VALIDATE,
		"/test/fraud-prevention-headers/validate",
		M_GET
	},

	/* OAuth */
	{
		OA_REFRESH_TOKEN,
		"/oauth/token",
		M_POST
	},
	{
		OA_EXCHANGE_AUTH_CODE,
		"/oauth/token",
		M_POST
	},
};

int do_ep(enum endpoint ep, const char *api_ver, const char *file,
	  const char *data, char **buf, ...)
{
	va_list ap;
	struct curl_ctx ctx = { 0 };
	const char *params[MAX_PARAMS] = { NULL };
	int i = 0;

	va_start(ap, buf);
	for ( ; ; i++) {
		if (i == MAX_PARAMS) {
			logger(MTD_LOG_ERR, "%s: MAX_PARAMS (%d) reached\n",
			       __func__, MAX_PARAMS);
			return -1;
		}

		params[i] = va_arg(ap, const char *);
		if (!params[i])
			break;
	}
	va_end(ap);

	ctx.mtd_api_ver = api_ver;
	ctx.endpoint = ep;
	if (*params)
		ctx.params = params;

	switch (endpoints[ep].method) {
	case M_GET:
		return do_get(&ctx, buf);
	case M_POST:
		return do_post(&ctx, file, data, buf);
	case M_PUT:
		return do_put(&ctx, file, data, buf);
	case M_DELETE:
		return do_delete(&ctx, buf);
	default:
		return -1;
	}
}

char *ep_make_url(enum endpoint ep, const char **params, char *url)
{
	char *nino = load_token("nino", FT_CONFIG);
	char *string;
	char *ptr;
	int len;
	int p = 0;

	string = strdup(endpoints[ep].tmpl + 1); /* skip past first '/' */
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
