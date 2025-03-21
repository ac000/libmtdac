/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * endpoints.h - Make Tax Digital - Self Assessment API
 *
 * Copyright (C) 2020 - 2025	Andrew Clayton <ac@sigsegv.uk>
 */

#ifndef _ENDPOINTS_H_
#define _ENDPOINTS_H_

#include "mtd.h"

#define URL_LEN			1024

/*
 * The order of these entries must match the order in endpoints[]
 * in endpoints.c
 */
enum endpoint {
	/* Business Details */
	BD_LIST = 0,
	BD_GET,

	/* Business Income Source Summary */
	BISS_GET_SUMMARY,

	/* Business Source Adjustable Summary */
	BSAS_LIST_SUMMARIES,
	BSAS_TRIGGER_SUMMARY,
	/* Business Source Adjustable Summary - Self-Employment */
	BSAS_SE_GET_SUMMARY,
	BSAS_SE_UPDATE_SUMMARY_ADJUSTMENTS,
	/* Business Source Adjustable Summary - UK Property Business */
	BSAS_PB_GET_SUMMARY,
	BSAS_PB_UPDATE_SUMMARY_ADJUSTMENTS,
	/* Business Source Adjustable Summary - Foreign Property Business */
	BSAS_FP_GET_SUMMARY,
	BSAS_FP_UPDATE_SUMMARY_ADJUSTMENTS,

	/* Individuals Business End of Period Statement */
	IBEOPS_SUBMIT_END_OF_PERIOD_STATEMENT,

	/* Individual Calculations - Self-Assessment */
	IC_LIST_CALCULATIONS,
	IC_TRIGGER_CALCULATION,
	IC_GET_CALCULATION,
	/* Individual Calculations - Final Declaration */
	IC_FINAL_DECLARATION,

	/* Individuals Disclosures */
	ID_DELETE,
	ID_SET,
	ID_GET,
	/* Individuals Disclosures - Marriage Allowance */
	ID_MA_CREATE,

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

	/* Obligations */
	OB_LIST_INC_AND_EXPEND_OBLIGATIONS,
	OB_LIST_CRYSTALLISATION_OBLIGATIONS,
	OB_LIST_END_OF_PERIOD_OBLIGATIONS,

	/* Self-Assessment - Self-Employment */
	SA_SE_CREATE_SELF_EMPLOYMENT,
	SA_SE_LIST_PERIODS,
	SA_SE_CREATE_PERIOD,
	SA_SE_GET_PERIOD,
	SA_SE_UPDATE_PERIOD,
	SA_SE_GET_ANNUAL_SUMMARY,
	SA_SE_UPDATE_ANNUAL_SUMMARY,
	/* Self-Assessment - UK Property Business */
	SA_PB_CREATE_PROPERTY,
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

	/* Self-Assessment Accounts - Payments and Liabilities */
	SAAC_GET_BALANCE,
	SAAC_LIST_TRANSACTIONS,
	SAAC_GET_TRANSACTION,
	SAAC_LIST_CHARGES,
	SAAC_GET_CHARGE,
	SAAC_LIST_PAYMENTS,
	SAAC_GET_PAYMENT,
	/* Self-Assessment Accounts - Coding Out Underpayments and Debts */
	SAAC_GET_CODING_OUT_UDA,
	SAAC_SET_CODING_OUT_UDA,
	SAAC_DELETE_CODING_OUT_UDA,

	/* VAT */
	VAT_LIST_OBLIGATIONS,
	VAT_SUBMIT_PERIOD,
	VAT_GET_PERIOD,
	VAT_LIST_LIABILITIES,
	VAT_LIST_PAYMENTS,

	/* Create Test User */
	TEST_CU_CREATE_INDIVIDUAL,
	TEST_CU_CREATE_ORGANISATION,
	TEST_CU_CREATE_AGENT,
	TEST_CU_LIST_SERVICES,

	/* Test Fraud Prevention Headers */
	TEST_FPH_VALIDATE,
	TEST_FPH_FEEDBACK,

	/* OAuth */
	OA_REFRESH_TOKEN,
	OA_EXCHANGE_AUTH_CODE,
	OA_APPLICATION_TOKEN,
};

static const struct {
	const char *name;
	const char *fname;
} ep_api_map[] = {
	[MTD_EP_API_ITSA]	= { "itsa", "Income Tax Self-Assessment" },
	[MTD_EP_API_VAT]	= { "vat", "VAT"			 },
};

extern int do_ep(enum endpoint ep, const char *api_ver,
		 const struct mtd_dsrc_ctx *dsctx, char **buf, ...);
extern int (*ep_set_oauther(enum endpoint ep))(enum mtd_ep_api);
extern char *ep_get_token(enum endpoint ep);
extern char *ep_make_url(enum endpoint ep, const char * const params[],
			 char *url);

#endif /* _ENDPOINTS_H_ */
