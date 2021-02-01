/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * endpoints.h - Make Tax Digital - Self Assessment API
 *
 * Copyright (C) 2020 - 2021	Andrew Clayton <andrew@digital-domain.net>
 */

#include "mtd.h"

#ifndef _ENDPOINTS_H_
#define _ENDPOINTS_H_

#define URL_LEN			1024

/*
 * The order of these entries must match the order in endpoints[]
 * in endpoints.c
 */
enum endpoint {
	/* Self-Assessment - Self-Employment */
	SA_SE_LIST_SELF_EMPLOYMENTS = 0,
	SA_SE_CREATE_SELF_EMPLOYMENT,
	SA_SE_GET_SELF_EMPLOYMENT,
	SA_SE_LIST_OBLIGATIONS,
	SA_SE_LIST_PERIODS,
	SA_SE_CREATE_PERIOD,
	SA_SE_GET_PERIOD,
	SA_SE_UPDATE_PERIOD,
	SA_SE_GET_ANNUAL_SUMMARY,
	SA_SE_UPDATE_ANNUAL_SUMMARY,
	SA_SE_SUBMIT_END_OF_PERIOD_STATEMENT,
	SA_SE_GET_END_OF_PERIOD_STATEMENT,
	/* Self-Assessment - UK Property Business */
	SA_PB_GET_PROPERTY,
	SA_PB_CREATE_PROPERTY,
	SA_PB_LIST_OBLIGATIONS,
	SA_PB_LIST_NON_FHL_PERIODS,
	SA_PB_CREATE_NON_FHL_PERIOD,
	SA_PB_GET_NON_FHL_PERIOD,
	SA_PB_UPDATE_NON_FHL_PERIOD,
	SA_PB_GET_NON_FHL_ANNUAL_SUMMARY,
	SA_PB_UPDATE_NON_FHL_ANNUAL_SUMMARY,
	SA_PB_LIST_FHL_PERIODS,
	SA_PB_CREATE_FHL_PERIOD,
	SA_PB_GET_FHL_PERIOD,
	SA_PB_UPDATE_FHL_PERIOD,
	SA_PB_GET_FHL_ANNUAL_SUMMARY,
	SA_PB_UPDATE_FHL_ANNUAL_SUMMARY,
	SA_PB_SUBMIT_END_OF_PERIOD_STATEMENT,
	SA_PB_GET_END_OF_PERIOD_STATEMENT,
	/* Self-Assessment - Dividends Income */
	SA_DI_GET_ANNUAL_SUMMARY,
	SA_DI_UPDATE_ANNUAL_SUMMARY,
	/* Self-Assessment - Savings Accounts */
	SA_SA_LIST_ACCOUNTS,
	SA_SA_CREATE_ACCOUNT,
	SA_SA_GET_ACCOUNT,
	SA_SA_GET_ANNUAL_SUMMARY,
	SA_SA_UPDATE_ANNUAL_SUMMARY,
	/* Self-Assessment - Charitable Giving */
	SA_CG_GET_CHARITABLE_GIVING,
	SA_CG_UPDATE_CHARITABLE_GIVING,
	/* Self-Assessment - Crystallisation */
	SA_CR_INTENT_TO_CRYSTALLISE,
	SA_CR_CRYSTALLISE,
	SA_CR_LIST_OBLIGATIONS,

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

	/* Individual Loses - Brought Forward */
	IL_BF_LIST_LOSES,
	IL_BF_CREATE_LOSS,
	IL_BF_GET_LOSS,
	IL_BF_DELETE_LOSS,
	IL_BF_UPDATE_LOSS_AMNT,
	/* Individual Loses - Loss Claims */
	IL_LC_LIST_LOSES,
	IL_LC_CREATE_LOSS,
	IL_LC_GET_LOSS,
	IL_LC_DELETE_LOSS,
	IL_LC_UPDATE_LOSS_TYPE,
	IL_LC_UPDATE_LOSS_ORDER,

	/* Individuals Disclosures */
	ID_DELETE,
	ID_SET,
	ID_GET,

	/* Business Income Source Summary */
	BISS_GET_SELF_EMPLOYMENT,
	BISS_GET_UK_PROPERTY,

	/* Business Source Adjustable Summary */
	BSAS_LIST_SUMMARIES,
	BSAS_TRIGGER_SUMMARY,
	/* Business Source Adjustable Summary - Self-Employment */
	BSAS_SE_GET_SUMMARY,
	BSAS_SE_LIST_SUMMARY_ADJUSTMENTS,
	BSAS_SE_UPDATE_SUMMARY_ADJUSTMENTS,
	/* Business Source Adjustable Summary - UK Property Business */
	BSAS_PB_GET_SUMMARY,
	BSAS_PB_LIST_SUMMARY_ADJUSTMENTS,
	BSAS_PB_UPDATE_SUMMARY_ADJUSTMENTS,

	/* VAT */
	VAT_LIST_OBLIGATIONS,
	VAT_SUBMIT_PERIOD,
	VAT_GET_PERIOD,
	VAT_LIST_LIABILITIES,
	VAT_LIST_PAYMENTS,

	/* National Insurance */
	NI_GET_ANNUAL_SUMMARY,

	/* Create Test User */
	TEST_CU_CREATE_INDIVIDUAL,
	TEST_CU_CREATE_ORGANISATION,
	TEST_CU_CREATE_AGENT,
	TEST_CU_LIST_SERVICES,

	/* Test Fraud Prevention Headers */
	TEST_FPH_VALIDATE,

	/* National Insurance Test Support */
	TEST_NI_CREATE_ANNUAL_SUMMARY,

	/* OAuth */
	OA_REFRESH_TOKEN,
	OA_EXCHANGE_AUTH_CODE,
	OA_APPLICATION_TOKEN,
};

extern int do_ep(enum endpoint ep, const char *api_ver,
		 const struct mtd_dsrc_ctx *dsctx, char **buf, ...);
extern int (*ep_set_oauther(enum endpoint ep))(void);
extern char *ep_get_token(enum endpoint ep);
extern char *ep_make_url(enum endpoint ep, const char * const params[],
			 char *url);

#endif /* _ENDPOINTS_H_ */
