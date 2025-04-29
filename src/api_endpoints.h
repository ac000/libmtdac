/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * api_endpoints.h - Make Tax Digital - API
 *
 * Copyright (C) 2025		Andrew Clayton <ac@sigsegv.uk>
 */

#ifndef _API_ENDPOINTS_H_
#define _API_ENDPOINTS_H_

#define _GNU_SOURCE

#include "mtd.h"
#include "curler.h"

enum oauth_authz {
	AUTHZ_UNSET = 0,
	AUTHZ_NONE,
	AUTHZ_APPLICATION,
	AUTHZ_USER,
};

enum ep_api {
	EP_API_NULL = 0,

	EP_API_BD,
	EP_API_BSAS,
	EP_API_ICAL,
	EP_API_ILOS,
	EP_API_ISI,
	EP_API_OB,
	EP_API_PB,
	EP_API_SEB,

	EP_API_TEST_CU,
	EP_API_TEST_FPH,
};

static const struct {
	const char *api_version;
	enum oauth_authz authz;
	enum mtd_api_scope scope;
} api_default_values[] = {
	[EP_API_BD] = {
		.api_version	= "1.0",
		.authz		= AUTHZ_USER,
		.scope		= MTD_API_SCOPE_ITSA,
	},
	[EP_API_BSAS] = {
		.api_version	= "6.0",
		.authz		= AUTHZ_USER,
		.scope		= MTD_API_SCOPE_ITSA,
	},
	[EP_API_ICAL] = {
		.api_version	= "7.0",
		.authz		= AUTHZ_USER,
		.scope		= MTD_API_SCOPE_ITSA,
	},
	[EP_API_ILOS] = {
		.api_version	= "5.0",
		.authz		= AUTHZ_USER,
		.scope		= MTD_API_SCOPE_ITSA,
	},
	[EP_API_ISI] = {
		.api_version	= "1.0",
		.authz		= AUTHZ_USER,
		.scope		= MTD_API_SCOPE_ITSA,
	},
	[EP_API_OB] = {
		.api_version	= "3.0",
		.authz		= AUTHZ_USER,
		.scope		= MTD_API_SCOPE_ITSA,
	},
	[EP_API_PB] = {
		.api_version	= "5.0",
		.authz		= AUTHZ_USER,
		.scope		= MTD_API_SCOPE_ITSA,
	},
	[EP_API_SEB] = {
		.api_version	= "4.0",
		.authz		= AUTHZ_USER,
		.scope		= MTD_API_SCOPE_ITSA,
	},

	[EP_API_TEST_CU] = {
		.api_version	= "1.0",
		.authz		= AUTHZ_APPLICATION,
		.scope		= MTD_API_SCOPE_NULL,
	},
	[EP_API_TEST_FPH] = {
		.api_version	= "1.0",
		.authz		= AUTHZ_APPLICATION,
		.scope		= MTD_API_SCOPE_NULL,
	}
};

/*
 * The order of these entries must match the order in the
 * mtd_api_endpoint enum in include/libmtdac/mtd.h
 */
static const struct {
	const char *tmpl;
	enum http_method method;
	enum content_type ctype;
	enum ep_api api;
	enum oauth_authz authz;
	enum mtd_api_scope scope;
} endpoints[] = {
	/* Business Details */
	[MTD_API_EP_BD_LIST] = {
		.tmpl	= "/individuals/business/details/{nino}/list",
		.method	= M_GET,
		.api	= EP_API_BD,
	},
	[MTD_API_EP_BD_GET] = {
		.tmpl	= "/individuals/business/details/{nino}/{businessId}",
		.method	= M_GET,
		.api	= EP_API_BD,
	},
	[MTD_API_EP_BD_AMEND_QPT] = {
		.tmpl	= "/individuals/business/details/{nino}/{businessId}/{taxYear}",
		.method	= M_PUT,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= EP_API_BD,
	},

	/* Business Source Adjustable Summary */
	[MTD_API_EP_BSAS_LIST] = {
		.tmpl	= "/individuals/self-assessment/adjustable-summary/{nino}/{taxYear}/{query_params}",
		.method	= M_GET,
		.api	= EP_API_BSAS,
	},
	[MTD_API_EP_BSAS_TRIGGER] = {
		.tmpl	= "/individuals/self-assessment/adjustable-summary/{nino}/trigger",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= EP_API_BSAS,
	},
	/* Self-Employment */
	[MTD_API_EP_BSAS_SE_GET] = {
		.tmpl	= "/individuals/self-assessment/adjustable-summary/{nino}/self-employment/{calculationId}/{taxYear}",
		.method	= M_GET,
		.api	= EP_API_BSAS,
	},
	[MTD_API_EP_BSAS_SE_SUBMIT] = {
		.tmpl	= "/individuals/self-assessment/adjustable-summary/{nino}/self-employment/{calculationId}/adjust/{taxYear}",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= EP_API_BSAS,
	},
	/* UK Property */
	[MTD_API_EP_BSAS_PB_GET] = {
		.tmpl	= "/individuals/self-assessment/adjustable-summary/{nino}/uk-property/{calculationId}/{taxYear}",
		.method	= M_GET,
		.api	= EP_API_BSAS,
	},
	[MTD_API_EP_BSAS_PB_SUBMIT] = {
		.tmpl	= "/individuals/self-assessment/adjustable-summary/{nino}/uk-property/{calculationId}/{taxYear}",
		.method	= M_GET,
		.api	= EP_API_BSAS,
	},
	/* Foreign Property */
	[MTD_API_EP_BSAS_FP_GET] = {
		.tmpl	= "/individuals/self-assessment/adjustable-summary/{nino}/foreign-property/{calculationId}/{taxYear}",
		.method	= M_GET,
		.api	= EP_API_BSAS,
	},
	[MTD_API_EP_BSAS_FP_SUBMIT] = {
		.tmpl	= "/individuals/self-assessment/adjustable-summary/{nino}/foreign-property/{calculationId}/adjust/{taxYear}",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= EP_API_BSAS,
	},

	/* Individual Calculations - Tax Calculations */
	[MTD_API_EP_ICAL_TRIGGER] = {
		.tmpl	= "/individuals/calculations/{nino}/self-assessment/{taxYear}/trigger/{calculationType}",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_NONE,
		.api	= EP_API_ICAL,
	},
	[MTD_API_EP_ICAL_LIST_OLD] = {
		.tmpl	= "/individuals/calculations/{nino}/self-assessment/{query_params}",
		.method	= M_GET,
		.api	= EP_API_ICAL,
	},
	[MTD_API_EP_ICAL_LIST] = {
		.tmpl	= "/individuals/calculations/{nino}/self-assessment/{taxYear}/{optional_query_params}",
		.method	= M_GET,
		.api	= EP_API_ICAL,
	},
	[MTD_API_EP_ICAL_GET] = {
		.tmpl	= "/individuals/calculations/{nino}/self-assessment/{taxYear}/{calculationId}",
		.method	= M_GET,
		.api	= EP_API_ICAL,
	},
	/* Final Declaration */
	[MTD_API_EP_ICAL_FINAL_DECLARATION] = {
		.tmpl	= "/individuals/calculations/{nino}/self-assessment/{taxYear}/{calculationId}/{calculationType}",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_NONE,
		.api	= EP_API_ICAL,
	},

	/* Individual Losses - Brought Forward */
	[MTD_API_EP_ILOS_BF_CREATE] = {
		.tmpl	= "/individuals/losses/{nino}/brought-forward-losses",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= EP_API_ILOS,
	},
	[MTD_API_EP_ILOS_BF_AMEND_AMNT] = {
		.tmpl	= "/individuals/losses/{nino}/brought-forward-losses/{lossId}/change-loss-amount",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= EP_API_ILOS,
	},
	[MTD_API_EP_ILOS_BF_LIST] = {
		.tmpl	= "/individuals/losses/{nino}/brought-forward-losses/tax-year/{taxYearBroughtForwardFrom}/{query_params}",
		.method	= M_GET,
		.api	= EP_API_ILOS,
	},
	[MTD_API_EP_ILOS_BF_GET] = {
		.tmpl	= "/individuals/losses/{nino}/brought-forward-losses/{lossId}",
		.method	= M_GET,
		.api	= EP_API_ILOS,
	},
	[MTD_API_EP_ILOS_BF_DELETE] = {
		.tmpl	= "/individuals/losses/{nino}/brought-forward-losses/{lossId}",
		.method	= M_DELETE,
		.api	= EP_API_ILOS,
	},
	/* Loss Claims */
	[MTD_API_EP_ILOS_LC_CREATE] = {
		.tmpl	= "/individuals/losses/{nino}/loss-claims",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= EP_API_ILOS,
	},
	[MTD_API_EP_ILOS_LC_LIST] = {
		.tmpl	= "/individuals/losses/{nino}/loss-claims/tax-year/{taxYearClaimedFor}/{query_params}",
		.method	= M_GET,
		.api	= EP_API_ILOS,
	},
	[MTD_API_EP_ILOS_LC_GET] = {
		.tmpl	= "/individuals/losses/{nino}/loss-claims/{claimId}",
		.method	= M_GET,
		.api	= EP_API_ILOS,
	},
	[MTD_API_EP_ILOS_LC_DELETE] = {
		.tmpl	= "/individuals/losses/{nino}/loss-claims/{claimId}",
		.method	= M_DELETE,
		.api	= EP_API_ILOS,
	},
	[MTD_API_EP_ILOS_LC_AMEND_TYPE] = {
		.tmpl	= "https://test-api.service.hmrc.gov.uk/individuals/losses/{nino}/loss-claims/{claimId}/change-type-of-claim",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= EP_API_ILOS,
	},
	[MTD_API_EP_ILOS_LC_AMEND_ORDER] = {
		.tmpl	= "https://test-api.service.hmrc.gov.uk/individuals/losses/{nino}/loss-claims/order/{taxYearClaimedFor}",
		.method	= M_PUT,
		.ctype  = CONTENT_TYPE_JSON,
		.api	= EP_API_ILOS,
	},

	/* Individuals Savings Income - UK Savings Account */
	[MTD_API_EP_ISI_SI_UK_LIST] = {
		.tmpl	= "/individuals/savings-income/uk-accounts/{nino}/{optional_query_params}",
		.method	= M_GET,
		.api	= EP_API_ISI,
	},
	[MTD_API_EP_ISI_SI_UK_ADD] = {
		.tmpl	= "/individuals/savings-income/uk-accounts/{nino}",
		.method	= M_POST,
		.ctype  = CONTENT_TYPE_JSON,
		.api	= EP_API_ISI,
	},
	[MTD_API_EP_ISI_SI_UK_GET_AS] = {
		.tmpl	= "/individuals/savings-income/uk-accounts/{nino}/{taxYear}/{savingsAccountId}",
		.method	= M_GET,
		.api	= EP_API_ISI,
	},
	[MTD_API_EP_ISI_SI_UK_UPDATE_AS] = {
		.tmpl	= "/individuals/savings-income/uk-accounts/{nino}/{taxYear}/{savingsAccountId}",
		.method	= M_PUT,
		.ctype  = CONTENT_TYPE_JSON,
		.api	= EP_API_ISI,
	},
	/* Savings Income */
	[MTD_API_EP_ISI_SI_O_GET] = {
		.tmpl	= "/individuals/savings-income/other/{nino}/{taxYear}",
		.method	= M_GET,
		.api	= EP_API_ISI,
	},
	[MTD_API_EP_ISI_SI_O_UPDATE] = {
		.tmpl	= "/individuals/savings-income/other/{nino}/{taxYear}",
		.method	= M_PUT,
		.ctype  = CONTENT_TYPE_JSON,
		.api	= EP_API_ISI,
	},
	[MTD_API_EP_ISI_SI_O_DELETE] = {
		.tmpl	= "/individuals/savings-income/other/{nino}/{taxYear}",
		.method = M_DELETE,
		.api	= EP_API_ISI,
	},

	/* Obligations */
	[MTD_API_EP_OB_GET_IEO] = {
		.tmpl	= "/obligations/details/{nino}/income-and-expenditure/{query_params}",
		.method	= M_GET,
		.api	= EP_API_OB,
	},
	[MTD_API_EP_OB_GET_FDO] = {
		.tmpl	= "/obligations/details/{nino}/crystallisation}/{query_params}",
		.method	= M_GET,
		.api	= EP_API_OB,
	},
	[MTD_API_EP_OB_GET_EPSO] = {
		.tmpl	= "/obligations/details/{nino}/end-of-period-statement/{query_params}",
		.method	= M_GET,
		.api	= EP_API_OB,
	},

	/* Property Business - UK Property Business Annual Submission */
	[MTD_API_EP_PB_UKPBAS_GET] = {
		.tmpl	= "/individuals/business/property/uk/{nino}/{businessId}/annual/{taxYear}",
		.method	= M_GET,
		.api	= EP_API_PB,
	},
	[MTD_API_EP_PB_UKPBAS_CREATE] = {
		.tmpl	= "/individuals/business/property/uk/{nino}/{businessId}/annual/{taxYear}",
		.method	= M_PUT,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= EP_API_PB,
	},
	/* UK Property Income & Expenses Period Summary */
	[MTD_API_EP_PB_UKPIEPS_CREATE] = {
		.tmpl	= "/individuals/business/property/uk/{nino}/{businessId}/period/{taxYear}",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= EP_API_PB,
	},
	[MTD_API_EP_PB_UKPIEPS_GET] = {
		.tmpl	= "/individuals/business/property/uk/{nino}/{businessId}/period/{taxYear}/{submissionId}",
		.method	= M_GET,
		.api	= EP_API_PB,
	},
	[MTD_API_EP_PB_UKPIEPS_AMEND] = {
		.tmpl	= "/individuals/business/property/uk/{nino}/{businessId}/period/{taxYear}/{submissionId}",
		.method	= M_PUT,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= EP_API_PB,
	},
	/* UK Property Cumulative Period Summary */
	[MTD_API_EP_PB_UKPCPS_GET] = {
		.tmpl	= "/individuals/business/property/uk/{nino}/{businessId}/cumulative/{taxYear}",
		.method	= M_GET,
		.api	= EP_API_PB,
	},
	[MTD_API_EP_PB_UKPCPS_CREATE] = {
		.tmpl	= "/individuals/business/property/uk/{nino}/{businessId}/cumulative/{taxYear}",
		.method	= M_PUT,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= EP_API_PB,
	},
	/* Historic FHL UK Property Business Annual Submission */
	[MTD_API_EP_PB_HFHL_UKPBAS_CREATE] = {
		.tmpl	= "/individuals/business/property/uk/annual/furnished-holiday-lettings/{nino}/{taxYear}",
		.method	= M_PUT,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= EP_API_PB,
	},
	[MTD_API_EP_PB_HFHL_UKPBAS_GET] = {
		.tmpl	= "/individuals/business/property/uk/annual/furnished-holiday-lettings/{nino}/{taxYear}",
		.method	= M_GET,
		.api	= EP_API_PB,
	},
	[MTD_API_EP_PB_HFHL_UKPBAS_DELETE] = {
		.tmpl	= "/individuals/business/property/uk/annual/furnished-holiday-lettings/{nino}/{taxYear}",
		.method	= M_DELETE,
		.api	= EP_API_PB,
	},
	/* Historic non-FHL UK Property Business Annual Submission */
	[MTD_API_EP_PB_HNFHL_UKPBAS_CREATE] = {
		.tmpl	= "/individuals/business/property/uk/annual/non-furnished-holiday-lettings/{nino}/{taxYear}",
		.method	= M_PUT,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= EP_API_PB,
	},
	[MTD_API_EP_PB_HNFHL_UKPBAS_GET] = {
		.tmpl	= "/individuals/business/property/uk/annual/non-furnished-holiday-lettings/{nino}/{taxYear}",
		.method	= M_GET,
		.api	= EP_API_PB,
	},
	[MTD_API_EP_PB_HNFHL_UKPBAS_DELETE] = {
		.tmpl	= "/individuals/business/property/uk/annual/non-furnished-holiday-lettings/{nino}/{taxYear}",
		.method	= M_DELETE,
		.api	= EP_API_PB,
	},
	/* Historic FHL UK Property Income & Expenses Period Summary */
	[MTD_API_EP_PB_HFHL_UKPIEPS_LIST] = {
		.tmpl	= "/individuals/business/property/uk/period/furnished-holiday-lettings/{nino}",
		.method	= M_GET,
		.api	= EP_API_PB,
	},
	[MTD_API_EP_PB_HFHL_UKPIEPS_CREATE] = {
		.tmpl	= "/individuals/business/property/uk/period/furnished-holiday-lettings/{nino}",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= EP_API_PB,
	},
	[MTD_API_EP_PB_HFHL_UKPIEPS_AMEND] = {
		.tmpl	= "/individuals/business/property/uk/period/furnished-holiday-lettings/{nino}/{periodId}",
		.method	= M_PUT,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= EP_API_PB,
	},
	[MTD_API_EP_PB_HFHL_UKPIEPS_GET] = {
		.tmpl	= "/individuals/business/property/uk/period/furnished-holiday-lettings/{nino}/{periodId}",
		.method	= M_GET,
		.api	= EP_API_PB,
	},
	/* Historic non-FHL UK Property Income & Expenses Period Summary */
	[MTD_API_EP_PB_HNFHL_UKPIEPS_LIST] = {
		.tmpl	= "/individuals/business/property/uk/period/non-furnished-holiday-lettings/{nino}",
		.method	= M_GET,
		.api	= EP_API_PB,
	},
	[MTD_API_EP_PB_HNFHL_UKPIEPS_CREATE] = {
		.tmpl	= "/individuals/business/property/uk/period/non-furnished-holiday-lettings/{nino}",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= EP_API_PB,
	},
	[MTD_API_EP_PB_HNFHL_UKPIEPS_GET] = {
		.tmpl	= "/individuals/business/property/uk/period/non-furnished-holiday-lettings/{nino}/{periodId}",
		.method	= M_GET,
		.api	= EP_API_PB,
	},
	[MTD_API_EP_PB_HNFHL_UKPIEPS_AMEND] = {
		.tmpl	= "/individuals/business/property/uk/period/non-furnished-holiday-lettings/{nino}/{periodId}",
		.method	= M_PUT,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= EP_API_PB,
	},
	/* Foreign Property Income & Expenses Period Summary */
	[MTD_API_EP_PB_FPIEPS_CREATE] = {
		.tmpl	= "/individuals/business/property/foreign/{nino}/{businessId}/period/{taxYear}",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= EP_API_PB,
	},
	[MTD_API_EP_PB_FPIEPS_GET] = {
		.tmpl	= "/individuals/business/property/foreign/{nino}/{businessId}/period/{taxYear}/{submissionId}",
		.method	= M_GET,
		.api	= EP_API_PB,
	},
	[MTD_API_EP_PB_FPIEPS_AMEND] = {
		.tmpl	= "/individuals/business/property/foreign/{nino}/{businessId}/period/{taxYear}/{submissionId}",
		.method	= M_PUT,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= EP_API_PB,
	},
	/* Foreign Property Cumulative Period Summary */
	[MTD_API_EP_PB_FPCPS_GET] = {
		.tmpl	= "/individuals/business/property/foreign/{nino}/{businessId}/cumulative/{taxYear}",
		.method	= M_GET,
		.api	= EP_API_PB,
	},
	[MTD_API_EP_PB_FPCPS_AMEND] = {
		.tmpl	= "/individuals/business/property/foreign/{nino}/{businessId}/cumulative/{taxYear}",
		.method	= M_PUT,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= EP_API_PB,
	},
	/* Foreign Property Annual Submission */
	[MTD_API_EP_PB_FPAS_GET] = {
		.tmpl	= "/individuals/business/property/foreign/{nino}/{businessId}/annual/{taxYear}",
		.method	= M_GET,
		.api	= EP_API_PB,
	},
	[MTD_API_EP_PB_FPAS_AMEND] = {
		.tmpl	= "/individuals/business/property/foreign/{nino}/{businessId}/annual/{taxYear}",
		.method	= M_PUT,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= EP_API_PB,
	},
	/* UK or Foreign Property Annual Submission Deletion */
	[MTD_API_EP_PB_AS_DELETE] = {
		.tmpl	= "/individuals/business/property/{nino}/{businessId}/annual/{taxYear}",
		.method	= M_DELETE,
		.api	= EP_API_PB,
	},
	/* UK or Foreign Property Income and Expenses Period Summaries List */
	[MTD_API_EP_PB_PIEPS_LIST] = {
		.tmpl	= "/individuals/business/property/{nino}/{businessId}/period/{taxYear}",
		.method	= M_GET,
		.api	= EP_API_PB,
	},

	/* Self Employment Business - Self-Employment Annual Submission */
	[MTD_API_EP_SEB_SEAS_AMEND] = {
		.tmpl	= "/individuals/business/self-employment/{nino}/{businessId}/annual/{taxYear}",
		.method	= M_PUT,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= EP_API_SEB,
	},
	[MTD_API_EP_SEB_SEAS_GET] = {
		.tmpl	= "/individuals/business/self-employment/{nino}/{businessId}/annual/{taxYear}",
		.method	= M_GET,
		.api	= EP_API_SEB,
	},
	[MTD_API_EP_SEB_SEAS_DELETE] = {
		.tmpl	= "/individuals/business/self-employment/{nino}/{businessId}/annual/{taxYear}",
		.method	= M_DELETE,
		.api	= EP_API_SEB,
	},
	/* Self-Employment Period Summaries */
	[MTD_API_EP_SEB_SEPS_CREATE] = {
		.tmpl	= "/individuals/business/self-employment/{nino}/{businessId}/period",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= EP_API_SEB,
	},
	[MTD_API_EP_SEB_SEPS_LIST] = {
		.tmpl	= "/individuals/business/self-employment/{nino}/{businessId}/period/{taxYear}",
		.method	= M_GET,
		.api	= EP_API_SEB,
	},
	[MTD_API_EP_SEB_SEPS_AMEND] = {
		.tmpl	= "/individuals/business/self-employment/{nino}/{businessId}/period/{taxYear}/{periodId}",
		.method	= M_PUT,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= EP_API_SEB,
	},
	[MTD_API_EP_SEB_SEPS_GET] = {
		.tmpl	= "/individuals/business/self-employment/{nino}/{businessId}/period/{taxYear}/{periodId}",
		.method	= M_GET,
		.api	= EP_API_SEB,
	},
	/* Self-Employment Cumulative Period Summary */
	[MTD_API_EP_SEB_SECPS_AMEND] = {
		.tmpl	= "/individuals/business/self-employment/{nino}/{businessId}/cumulative/{taxYear}",
		.method	= M_PUT,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= EP_API_SEB,
	},
	[MTD_API_EP_SEB_SECPS_GET] = {
		.tmpl	= "/individuals/business/self-employment/{nino}/{businessId}/cumulative/{taxYear}",
		.method	= M_GET,
		.api	= EP_API_SEB,
	},


	/* Create Test User */
	[MTD_API_EP_TEST_CU_CREATE_INDIVIDUAL] = {
		.tmpl	= "/create-test-user/individuals",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= EP_API_TEST_CU,
	},
	[MTD_API_EP_TEST_CU_CREATE_ORGANISATION] = {
		.tmpl	= "/create-test-user/organisations",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= EP_API_TEST_CU,
	},
	[MTD_API_EP_TEST_CU_CREATE_AGENT] = {
		.tmpl	= "/create-test-user/agents",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= EP_API_TEST_CU,
	},
	[MTD_API_EP_TEST_CU_LIST_SERVICES] = {
		.tmpl	= "/create-test-user/services",
		.method	= M_GET,
		.api	= EP_API_TEST_CU,
	},

	/* Test Fraud Prevention Headers */
	[MTD_API_EP_TEST_FPH_VALIDATE] = {
		.tmpl	= "/test/fraud-prevention-headers/validate",
		.method	= M_GET,
		.api	= EP_API_TEST_FPH,
	},
	[MTD_API_EP_TEST_FPH_FEEDBACK] = {
		.tmpl	= "/test/fraud-prevention-headers/{api}/validation-feedback/{query_params}",
		.method	= M_GET,
		.api	= EP_API_TEST_FPH,
	},

	/* OAuth */
	[MTD_API_EP_OA_REFRESH_TOKEN] = {
		.tmpl	= "/oauth/token",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_URL_ENCODED,
		.api	= EP_API_NULL,
		.authz	= AUTHZ_NONE,
		.scope	= MTD_API_SCOPE_NULL
	},
	[MTD_API_EP_OA_EXCHANGE_AUTH_CODE] = {
		.tmpl	= "/oauth/token",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_URL_ENCODED,
		.api	= EP_API_NULL,
		.authz	= AUTHZ_NONE,
		.scope	= MTD_API_SCOPE_NULL
	},
	[MTD_API_EP_OA_APPLICATION_TOKEN] = {
		.tmpl	= "/oauth/token",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_URL_ENCODED,
		.api	= EP_API_NULL,
		.authz	= AUTHZ_NONE,
		.scope	= MTD_API_SCOPE_NULL
	},
};

#endif /* _API_ENDPOINTS_H_ */
