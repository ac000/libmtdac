/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * endpoints.h - Make Tax Digital - Self Assessment API
 *
 * Copyright (C) 2020		Andrew Clayton <andrew@digital-domain.net>
 */

#ifndef _ENDPOINTS_H_
#define _ENDPOINTS_H_

#ifdef __cplusplus
extern "C" {
#endif

#define URL_LEN			1024

/*
 * The order of these entries must match the order in endpoints[]
 * in endpoints.c
 */
enum endpoint {
	/* Self-Assessment */
	SA_LIST_SELF_EMPLOYMENTS = 0,
	SA_GET_SELF_EMPLOYMENT,
	SA_LIST_OBLIGATIONS,
	SA_LIST_PERIODS,
	SA_CREATE_PERIOD,
	SA_GET_PERIOD,
	SA_UPDATE_PERIOD,
	SA_GET_ANNUAL_SUMMARY,
	SA_UPDATE_ANNUAL_SUMMARY,
	SA_SUBMIT_END_OF_PERIOD_STATEMENT,
	SA_GET_END_OF_PERIOD_STATEMENT,

	/* Self-Assessment Accounts */
	SAAC_GET_BALANCE,
	SAAC_LIST_TRANSACTIONS,
	SAAC_GET_TRANSACTION,
	SAAC_LIST_CHARGES,
	SAAC_GET_CHARGE,
	SAAC_LIST_PAYMENTS,
	SAAC_GET_PAYMENT,

	/* Individual Calculations */
	IC_LIST_CALCULATIONS,
	IC_TRIGGER_CALCULATION,
	IC_GET_CALCULATION_META,
	IC_GET_INCOME_TAX_NICS_CALC,
	IC_GET_TAXABLE_INCOME,
	IC_GET_ALLOWANCES_DEDUCT_RELIEFS,
	IC_GET_EOY_EST,
	IC_GET_MSGS,

	/* National Insurance */
	NI_GET_ANNUAL_SUMMARY,

	/* Create Test User */
	TEST_CU_CREATE_INDIVIDUAL,
	TEST_CU_CREATE_ORGANISATION,
	TEST_CU_CREATE_AGENT,
	TEST_CU_LIST_SERVICES,

	/* Test Fraud Prevention Headers */
	TEST_FPH_VALIDATE,

	/* OAuth */
	OA_REFRESH_TOKEN,
	OA_EXCHANGE_AUTH_CODE,
};

extern char *ep_make_url(enum endpoint ep, const char **params, char *url);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _ENDPOINTS_H_ */
