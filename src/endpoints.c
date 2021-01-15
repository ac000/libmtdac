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

enum oauth_scope {
	SCOPE_NONE = 0,
	SCOPE_APPLICATION,
	SCOPE_USER,
};

/*
 * The order of these entries must match the order in enum endpoint
 * in endpoints.h
 */
static const struct _endpoint {
	const char *tmpl;
	const enum http_method method;
	const enum content_type ctype;
	const enum oauth_scope scope;
} endpoints[] = {
	/* Self-Assessment - Self-Employment */
	[SA_SE_LIST_SELF_EMPLOYMENTS] = {
		.tmpl	= "/self-assessment/ni/{nino}/self-employments",
		.method	= M_GET,
		.scope	= SCOPE_USER
	},
	[SA_SE_CREATE_SELF_EMPLOYMENT] = {
		.tmpl	= "/self-assessment/ni/{nino}/self-employments",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_JSON,
		.scope	= SCOPE_USER
	},
	[SA_SE_GET_SELF_EMPLOYMENT] = {
		.tmpl	= "/self-assessment/ni/{nino}/self-employments/{selfEmploymentId}",
		.method	= M_GET,
		.scope	= SCOPE_USER
	},
	[SA_SE_LIST_OBLIGATIONS] = {
		.tmpl	= "/self-assessment/ni/{nino}/self-employments/{selfEmploymentId}/obligations/{optional_query_params}",
		.method	= M_GET,
		.scope	= SCOPE_USER
	},
	[SA_SE_LIST_PERIODS] = {
		.tmpl	= "/self-assessment/ni/{nino}/self-employments/{selfEmploymentId}/periods",
		.method	= M_GET,
		.scope	= SCOPE_USER
	},
	[SA_SE_CREATE_PERIOD] = {
		.tmpl	= "/self-assessment/ni/{nino}/self-employments/{selfEmploymentId}/periods",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_JSON,
		.scope	= SCOPE_USER
	},
	[SA_SE_GET_PERIOD] = {
		.tmpl	= "/self-assessment/ni/{nino}/self-employments/{selfEmploymentId}/periods/{periodId}",
		.method	= M_GET,
		.scope	= SCOPE_USER
	},
	[SA_SE_UPDATE_PERIOD] = {
		.tmpl	= "/self-assessment/ni/{nino}/self-employments/{selfEmploymentId}/periods/{periodId}",
		.method	= M_PUT,
		.ctype	= CONTENT_TYPE_JSON,
		.scope	= SCOPE_USER
	},
	[SA_SE_GET_ANNUAL_SUMMARY] = {
		.tmpl	= "/self-assessment/ni/{nino}/self-employments/{selfEmploymentId}/{taxYear}",
		.method	= M_GET,
		.scope	= SCOPE_USER
	},
	[SA_SE_UPDATE_ANNUAL_SUMMARY] = {
		.tmpl	= "/self-assessment/ni/{nino}/self-employments/{selfEmploymentId}/{taxYear}",
		.method	= M_PUT,
		.ctype	= CONTENT_TYPE_JSON,
		.scope	= SCOPE_USER
	},
	[SA_SE_SUBMIT_END_OF_PERIOD_STATEMENT] = {
		.tmpl	= "/self-assessment/ni/{nino}/self-employments/{selfEmploymentId}/end-of-period-statements/from/{start}/to/{end}",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_JSON,
		.scope	= SCOPE_USER
	},
	[SA_SE_GET_END_OF_PERIOD_STATEMENT] = {
		.tmpl	= "/self-assessment/ni/{nino}/self-employments/{selfEmploymentId}/end-of-period-statements/obligations/{optional_query_params}",
		.method	= M_GET,
		.scope	= SCOPE_USER
	},
	/* Self-Assessment - UK Property Business */
	[SA_PB_GET_PROPERTY] = {
		.tmpl	= "/self-assessment/ni/{nino}/uk-properties",
		.method	= M_GET,
		.scope	= SCOPE_USER
	},
	[SA_PB_CREATE_PROPERTY] = {
		.tmpl	= "/self-assessment/ni/{nino}/uk-properties",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_JSON,
		.scope	= SCOPE_USER
	},
	[SA_PB_LIST_OBLIGATIONS] = {
		.tmpl	= "/self-assessment/ni/{nino}/uk-properties/obligations",
		.method	= M_GET,
		.scope	= SCOPE_USER
	},
	[SA_PB_LIST_NON_FHL_PERIODS] = {
		.tmpl	= "/self-assessment/ni/{nino}/uk-properties/other/periods",
		.method	= M_GET,
		.scope	= SCOPE_USER
	},
	[SA_PB_CREATE_NON_FHL_PERIOD] = {
		.tmpl	= "/self-assessment/ni/{nino}/uk-properties/other/periods",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_JSON,
		.scope	= SCOPE_USER
	},
	[SA_PB_GET_NON_FHL_PERIOD] = {
		.tmpl	= "/self-assessment/ni/{nino}/uk-properties/other/periods/{periodId}",
		.method	= M_GET,
		.scope	= SCOPE_USER
	},
	[SA_PB_UPDATE_NON_FHL_PERIOD] = {
		.tmpl	= "/self-assessment/ni/{nino}/uk-properties/other/periods/{periodId}",
		.method	= M_PUT,
		.ctype	= CONTENT_TYPE_JSON,
		.scope	= SCOPE_USER
	},
	[SA_PB_GET_NON_FHL_ANNUAL_SUMMARY] = {
		.tmpl	= "/self-assessment/ni/{nino}/uk-properties/other/{taxYear}",
		.method	= M_GET,
		.scope	= SCOPE_USER
	},
	[SA_PB_UPDATE_NON_FHL_ANNUAL_SUMMARY] = {
		.tmpl	= "/self-assessment/ni/{nino}/uk-properties/other/{taxYear}",
		.method	= M_PUT,
		.ctype	= CONTENT_TYPE_JSON,
		.scope	= SCOPE_USER
	},
	[SA_PB_LIST_FHL_PERIODS] = {
		.tmpl	= "/self-assessment/ni/{nino}/uk-properties/furnished-holiday-lettings/periods",
		.method	= M_GET,
		.scope	= SCOPE_USER
	},
	[SA_PB_CREATE_FHL_PERIOD] = {
		.tmpl	= "/self-assessment/ni/{nino}/uk-properties/furnished-holiday-lettings/periods",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_JSON,
		.scope	= SCOPE_USER
	},
	[SA_PB_GET_FHL_PERIOD] = {
		.tmpl	= "/self-assessment/ni/{nino}/uk-properties/furnished-holiday-lettings/periods/{periodId}",
		.method	= M_GET,
		.scope	= SCOPE_USER
	},
	[SA_PB_UPDATE_FHL_PERIOD] = {
		.tmpl	= "/self-assessment/ni/{nino}/uk-properties/furnished-holiday-lettings/periods/{periodId}",
		.method	= M_PUT,
		.ctype	= CONTENT_TYPE_JSON,
		.scope	= SCOPE_USER
	},
	[SA_PB_GET_FHL_ANNUAL_SUMMARY] = {
		.tmpl	= "/self-assessment/ni/{nino}/uk-properties/furnished-holiday-lettings/{taxYear}",
		.method	= M_GET,
		.scope	= SCOPE_USER
	},
	[SA_PB_UPDATE_FHL_ANNUAL_SUMMARY] = {
		.tmpl	= "/self-assessment/ni/{nino}/uk-properties/furnished-holiday-lettings/{taxYear}",
		.method	= M_PUT,
		.ctype	= CONTENT_TYPE_JSON,
		.scope	= SCOPE_USER
	},
	[SA_PB_SUBMIT_END_OF_PERIOD_STATEMENT] = {
		.tmpl	= "/self-assessment/ni/{nino}/uk-properties/end-of-period-statements/from/{start}/to/{end}",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_JSON,
		.scope	= SCOPE_USER
	},
	[SA_PB_GET_END_OF_PERIOD_STATEMENT] = {
		.tmpl	= "/self-assessment/ni/{nino}/uk-properties/end-of-period-statements/obligations/{query_params}",
		.method	= M_GET,
		.scope	= SCOPE_USER
	},
	/* Self-Assessment - Dividends Income */
	[SA_DI_GET_ANNUAL_SUMMARY] = {
		.tmpl	= "/self-assessment/ni/{nino}/dividends/{taxYear}",
		.method	= M_GET,
		.scope	= SCOPE_USER
	},
	[SA_DI_UPDATE_ANNUAL_SUMMARY] = {
		.tmpl	= "/self-assessment/ni/{nino}/dividends/{taxYear}",
		.method	= M_PUT,
		.ctype	= CONTENT_TYPE_JSON,
		.scope	= SCOPE_USER
	},
	/* Self-Assessment - Savings Accounts */
	[SA_SA_LIST_ACCOUNTS] = {
		.tmpl	= "/self-assessment/ni/{nino}/savings-accounts",
		.method	= M_GET,
		.scope	= SCOPE_USER
	},
	[SA_SA_CREATE_ACCOUNT] = {
		.tmpl	= "/self-assessment/ni/{nino}/savings-accounts",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_JSON,
		.scope	= SCOPE_USER
	},
	[SA_SA_GET_ACCOUNT] = {
		.tmpl	= "/self-assessment/ni/{nino}/savings-accounts/{savingsAccountId}",
		.method	= M_GET,
		.scope	= SCOPE_USER
	},
	[SA_SA_GET_ANNUAL_SUMMARY] = {
		.tmpl	= "/self-assessment/ni/{nino}/savings-accounts/{savingsAccountId}/{taxYear}",
		.method	= M_GET,
		.scope	= SCOPE_USER
	},
	[SA_SA_UPDATE_ANNUAL_SUMMARY] = {
		.tmpl	= "/self-assessment/ni/{nino}/savings-accounts/{savingsAccountId}/{taxYear}",
		.method	= M_PUT,
		.ctype	= CONTENT_TYPE_JSON,
		.scope	= SCOPE_USER
	},
	/* Self-Assessment - Charitable Giving */
	[SA_CG_GET_CHARITABLE_GIVING] = {
		.tmpl	= "/self-assessment/ni/{nino}/charitable-giving/{taxYear}",
		.method	= M_GET,
		.scope	= SCOPE_USER
	},
	[SA_CG_UPDATE_CHARITABLE_GIVING] = {
		.tmpl	= "/self-assessment/ni/{nino}/charitable-giving/{taxYear}",
		.method	= M_PUT,
		.ctype	= CONTENT_TYPE_JSON,
		.scope	= SCOPE_USER
	},
	/* Self-Assessment - Crystallisation */
	[SA_CR_INTENT_TO_CRYSTALLISE] = {
		.tmpl	= "/self-assessment/ni/{nino}/{taxYear}/intent-to-crystallise",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_JSON,
		.scope	= SCOPE_USER
	},
	[SA_CR_CRYSTALLISE] = {
		.tmpl	= "/self-assessment/ni/{nino}/{taxYear}/crystallisation",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_JSON,
		.scope	= SCOPE_USER
	},
	[SA_CR_LIST_OBLIGATIONS] = {
		.tmpl	= "/self-assessment/ni/{nino}/crystallisation/obligations/{query_params}",
		.method	= M_GET,
		.scope	= SCOPE_USER
	},

	/* Self-Assessment Accounts */
	[SAAC_GET_BALANCE] = {
		.tmpl	= "/accounts/self-assessment/{nino}/balance",
		.method	= M_GET,
		.scope	= SCOPE_USER
	},
	[SAAC_LIST_TRANSACTIONS] ={
		.tmpl	= "/accounts/self-assessment/{nino}/transactions/{query_params}",
		.method	= M_GET,
		.scope	= SCOPE_USER
	},
	[SAAC_GET_TRANSACTION] = {
		.tmpl	= "/accounts/self-assessment/{nino}/transactions/{transactionId}",
		.method	= M_GET,
		.scope	= SCOPE_USER
	},
	[SAAC_LIST_CHARGES] = {
		.tmpl	= "/accounts/self-assessment/{nino}/charges/{query_params}",
		.method	= M_GET,
		.scope	= SCOPE_USER
	},
	[SAAC_GET_CHARGE] = {
		.tmpl	= "/accounts/self-assessment/{nino}/charges/{transactionId}",
		.method	= M_GET,
		.scope	= SCOPE_USER
	},
	[SAAC_LIST_PAYMENTS] = {
		.tmpl	= "/accounts/self-assessment/{nino}/payments/{query_params}",
		.method	= M_GET,
		.scope	= SCOPE_USER
	},
	[SAAC_GET_PAYMENT] = {
		.tmpl	= "/accounts/self-assessment/{nino}/payments/{paymentId}",
		.method	= M_GET,
		.scope	= SCOPE_USER
	},

	/* Individual Calculations */
	[IC_LIST_CALCULATIONS] = {
		.tmpl	= "/individuals/calculations/{nino}/self-assessment/{optional_query_params}",
		.method	= M_GET,
		.scope	= SCOPE_USER
	},
	[IC_TRIGGER_CALCULATION] = {
		.tmpl	= "/individuals/calculations/{nino}/self-assessment",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_JSON,
		.scope	= SCOPE_USER
	},
	[IC_GET_CALCULATION_META] = {
		.tmpl	= "/individuals/calculations/{nino}/self-assessment/{calculationId}",
		.method	= M_GET,
		.scope	= SCOPE_USER
	},
	[IC_GET_INCOME_TAX_NICS_CALC] = {
		.tmpl	= "/individuals/calculations/{nino}/self-assessment/{calculationId}/income-tax-nics-calculated",
		.method	= M_GET,
		.scope	= SCOPE_USER,
	},
	[IC_GET_TAXABLE_INCOME] = {
		.tmpl	= "/individuals/calculations/{nino}/self-assessment/{calculationId}/taxable-income",
		.method	= M_GET,
		.scope	= SCOPE_USER
	},
	[IC_GET_ALLOWANCES_DEDUCT_RELIEFS] = {
		.tmpl	= "/individuals/calculations/{nino}/self-assessment/{calculationId}/allowances-deductions-reliefs",
		.method	= M_GET,
		.scope	= SCOPE_USER
	},
	[IC_GET_EOY_EST] = {
		.tmpl	= "/individuals/calculations/{nino}/self-assessment/{calculationId}/end-of-year-estimate",
		.method	= M_GET,
		.scope	= SCOPE_USER
	},
	[IC_GET_MSGS] = {
		.tmpl	= "/individuals/calculations/{nino}/self-assessment/{calculationId}/messages",
		.method	= M_GET,
		.scope	= SCOPE_USER
	},

	/* Individual Loses - Brought Forward */
	[IL_BF_LIST_LOSES] = {
		.tmpl	= "/individuals/losses/{nino}/brought-forward-losses/{optional_query_params}",
		.method	= M_GET,
		.scope	= SCOPE_USER
	},
	[IL_BF_CREATE_LOSS] = {
		.tmpl	= "/individuals/losses/{nino}/brought-forward-losses",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_JSON,
		.scope	= SCOPE_USER
	},
	[IL_BF_GET_LOSS] = {
		.tmpl	= "/individuals/losses/{nino}/brought-forward-losses/{lossId}",
		.method	= M_GET,
		.scope	= SCOPE_USER
	},
	[IL_BF_DELETE_LOSS] = {
		.tmpl	= "/individuals/losses/{nino}/brought-forward-losses/{lossId}",
		.method	= M_DELETE,
		.scope	= SCOPE_USER
	},
	[IL_BF_UPDATE_LOSS_AMNT] = {
		.tmpl	= "/individuals/losses/{nino}/brought-forward-losses/{lossId}/change-loss-amount",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_JSON,
		.scope	= SCOPE_USER
	},
	/* Individual Loses - Loss Claims */
	[IL_LC_LIST_LOSES] = {
		.tmpl	= "/individuals/losses/{nino}/loss-claims/{optional_query_params}",
		.method	= M_GET,
		.scope	= SCOPE_USER
	},
	[IL_LC_CREATE_LOSS] = {
		.tmpl	= "/individuals/losses/{nino}/loss-claims",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_JSON,
		.scope	= SCOPE_USER
	},
	[IL_LC_GET_LOSS] = {
		.tmpl	= "/individuals/losses/{nino}/loss-claims/{claimId}",
		.method	= M_GET,
		.scope	= SCOPE_USER
	},
	[IL_LC_DELETE_LOSS] = {
		.tmpl	= "/individuals/losses/{nino}/loss-claims/{claimId}",
		.method	= M_DELETE,
		.scope	= SCOPE_USER
	},
	[IL_LC_UPDATE_LOSS_TYPE] = {
		.tmpl	= "/individuals/losses/{nino}/loss-claims/{claimId}/change-type-of-claim",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_JSON,
		.scope	= SCOPE_USER
	},
	[IL_LC_UPDATE_LOSS_ORDER] = {
		.tmpl	= "/individuals/losses/{nino}/loss-claims/order/{optional_query_params}",
		.method	= M_PUT,
		.ctype	= CONTENT_TYPE_JSON,
		.scope	= SCOPE_USER
	},

	/* National Insurance */
	[NI_GET_ANNUAL_SUMMARY] = {
		.tmpl	= "/national-insurance/sa/{utr}/annual-summary/{taxYear}",
		.method	= M_GET,
		.scope	= SCOPE_USER
	},

	/* Business Income Source Summary */
	[BISS_GET_SELF_EMPLOYMNET] = {
		.tmpl	= "/individuals/self-assessment/income-summary/{nino}/self-employment/{query_params}",
		.method	= M_GET,
		.scope	= SCOPE_USER
	},
	[BISS_GET_PROPERTY] = {
		.tmpl	= "/individuals/self-assessment/income-summary/{nino}/uk-property/{query_params}",
		.method	= M_GET,
		.scope	= SCOPE_USER
	},

	/* Business Source Adjustable Summary */
	[BSAS_LIST_SUMMARIES] = {
		.tmpl	= "/individuals/self-assessment/adjustable-summary/{nino}/{query_params}",
		.method	= M_GET,
		.scope	= SCOPE_USER
	},
	[BSAS_TRIGGER_SUMMARY] = {
		.tmpl	= "/individuals/self-assessment/adjustable-summary/{nino}/trigger",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_JSON,
		.scope	= SCOPE_USER
	},
	[BSAS_SE_GET_SUMMARY] = {
		.tmpl	= "/individuals/self-assessment/adjustable-summary/{nino}/self-employment/{bsasId}/{optional_query_params}",
		.method	= M_GET,
		.scope	= SCOPE_USER
	},
	[BSAS_SE_LIST_SUMMARY_ADJUSTMENTS] = {
		.tmpl	= "/individuals/self-assessment/adjustable-summary/{nino}/self-employment/{bsasId}/adjust",
		.method = M_GET,
		.scope	= SCOPE_USER
	},
	[BSAS_SE_UPDATE_SUMMARY_ADJUSTMENTS] = {
		.tmpl	= "/individuals/self-assessment/adjustable-summary/{nino}/self-employment/{bsasId}/adjust",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_JSON,
		.scope	= SCOPE_USER
	},
	[BSAS_PB_GET_SUMMARY] = {
		.tmpl	= "/individuals/self-assessment/adjustable-summary/{nino}/property/{bsasId}/{optional_query_params}",
		.method	= M_GET,
		.scope	= SCOPE_USER
	},
	[BSAS_PB_LIST_SUMMARY_ADJUSTMENTS] = {
		.tmpl	= "/individuals/self-assessment/adjustable-summary/{nino}/property/{bsasId}/adjust",
		.method	= M_GET,
		.scope	= SCOPE_USER
	},
	[BSAS_PB_UPDATE_SUMMARY_ADJUSTMENTS] = {
		.tmpl	= "/individuals/self-assessment/adjustable-summary/{nino}/property/{bsasId}/adjust",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_JSON,
		.scope	= SCOPE_USER
	},

	/* VAT */
	[VAT_LIST_OBLIGATIONS] = {
		.tmpl	= "/organisations/vat/{vrn}/obligations/{optional_query_params}",
		.method	= M_GET,
		.scope	= SCOPE_USER
	},
	[VAT_SUBMIT_PERIOD] = {
		.tmpl	= "/organisations/vat/{vrn}/returns",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_JSON,
		.scope	= SCOPE_USER
	},
	[VAT_GET_PERIOD] = {
		.tmpl	= "/organisations/vat/{vrn}/returns/{periodKey}",
		.method	= M_GET,
		.scope	= SCOPE_USER
	},
	[VAT_LIST_LIABILITIES] = {
		.tmpl	= "/organisations/vat/{vrn}/liabilities/{query_params}",
		.method	= M_GET,
		.scope	= SCOPE_USER
	},
	[VAT_LIST_PAYMENTS] = {
		.tmpl	= "/organisations/vat/{vrn}/payments/{query_params}",
		.method	= M_GET,
		.scope	= SCOPE_USER
	},

	/* Create Test User */
	[TEST_CU_CREATE_INDIVIDUAL] = {
		.tmpl	= "/create-test-user/individuals",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_JSON,
		.scope	= SCOPE_APPLICATION
	},
	[TEST_CU_CREATE_ORGANISATION] = {
		.tmpl	= "/create-test-user/organisations",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_JSON,
		.scope	= SCOPE_APPLICATION
	},
	[TEST_CU_CREATE_AGENT] = {
		.tmpl	= "/create-test-user/agents",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_JSON,
		.scope	= SCOPE_APPLICATION
	},
	[TEST_CU_LIST_SERVICES] = {
		.tmpl	= "/create-test-user/services",
		.method	= M_GET,
		.scope	= SCOPE_APPLICATION
	},

	/* Test Fraud Prevention Headers */
	[TEST_FPH_VALIDATE] = {
		.tmpl	= "/test/fraud-prevention-headers/validate",
		.method	= M_GET,
		.scope	= SCOPE_APPLICATION
	},

	/* National Insurance Test Support */
	[TEST_NI_CREATE_ANNUAL_SUMMARY] = {
		.tmpl	= "/national-insurance-test-support/sa/{utr}/annual-summary/{taxYear}",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_JSON,
		.scope	= SCOPE_APPLICATION
	},

	/* OAuth */
	[OA_REFRESH_TOKEN] = {
		.tmpl	= "/oauth/token",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_URL_ENCODED,
		.scope	= SCOPE_NONE
	},
	[OA_EXCHANGE_AUTH_CODE] = {
		.tmpl	= "/oauth/token",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_URL_ENCODED,
		.scope	= SCOPE_NONE
	},
	[OA_APPLICATION_TOKEN] = {
		.tmpl	= "/oauth/token",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_URL_ENCODED,
		.scope	= SCOPE_NONE
	},
};

int do_ep(enum endpoint ep, const char *api_ver,
	  const struct mtd_dsrc_ctx *dsctx, char **buf, ...)
{
	va_list ap;
	struct curl_ctx ctx = { 0 };
	const char *params[MAX_PARAMS] = { NULL };
	int i = 0;

	va_start(ap, buf);
	for ( ; ; i++) {
		if (i == MAX_PARAMS) {
			logger(MTD_LOG_ERR, "MAX_PARAMS (%d) reached\n",
			       MAX_PARAMS);
			va_end(ap);
			return -1;
		}

		params[i] = va_arg(ap, const char *);
		if (!params[i])
			break;
	}
	va_end(ap);

	ctx.mtd_api_ver = api_ver;
	ctx.endpoint = ep;
	ctx.params = params;
	ctx.content_type = endpoints[ep].ctype;
	ctx.http_method = endpoints[ep].method;
	ctx.dsctx = dsctx;

	*buf = NULL;

	switch (ctx.http_method) {
	case M_GET:
		return do_get(&ctx, buf);
	case M_POST:
		return do_post(&ctx, buf);
	case M_PUT:
		return do_put(&ctx, buf);
	case M_DELETE:
		return do_delete(&ctx, buf);
	default:
		return -1;
	}
}

int (*ep_set_oauther(enum endpoint ep))(void)
{
	switch (endpoints[ep].scope) {
	case SCOPE_USER:
		return oauther_refresh_access_token;
	case SCOPE_APPLICATION:
		return oauther_get_application_token;
	default:
		return oauther_dummy;
	}
}

char *ep_get_token(enum endpoint ep)
{
	switch(endpoints[ep].scope) {
	case SCOPE_USER:
		return load_token("access_token", FT_AUTH);
	case SCOPE_APPLICATION:
		return load_token("access_token", FT_AUTH_APPLICATION);
	default:
		return NULL;
	}
}

extern __thread struct mtd_ctx mtd_ctx;
char *ep_make_url(enum endpoint ep, const char * const params[], char *url)
{
	char *nino = NULL;
	char *string;
	char *ptr;
	int len;
	int p = 0;

	string = strdup(endpoints[ep].tmpl + 1); /* skip past first '/' */
	ptr = string;

	len = snprintf(url, URL_LEN + 1, "%s", mtd_ctx.api_url);

	if (strstr(string,  "{nino}"))
		nino = load_token("nino", FT_NINO);

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
		else
			len += snprintf(url + len, URL_LEN+1 - len, "/%s",
					params[p++]);
	}

	free(string);
	free(nino);

	return url;
}
