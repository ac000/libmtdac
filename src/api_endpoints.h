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

static const struct {
	const char *api_version;
	const char *name;
	enum oauth_authz authz;
	enum mtd_api_scope scope;
} api_default_values[] = {
	[MTD_EP_API_BD] = {
		.api_version	= "1.0",
		.name		= "Business Details",
		.authz		= AUTHZ_USER,
		.scope		= MTD_API_SCOPE_SA,
	},
	[MTD_EP_API_BISS] = {
		.api_version	= "3.0",
		.name		= "Business Income Source Summary",
		.authz		= AUTHZ_USER,
		.scope		= MTD_API_SCOPE_SA,
	},
	[MTD_EP_API_BSAS] = {
		.api_version	= "7.0",
		.name		= "Business Source Adjustable Summary",
		.authz		= AUTHZ_USER,
		.scope		= MTD_API_SCOPE_SA,
	},
	[MTD_EP_API_ICAL] = {
		.api_version	= "7.0",
		.name		= "Individual Calculations",
		.authz		= AUTHZ_USER,
		.scope		= MTD_API_SCOPE_SA,
	},
	[MTD_EP_API_ID] = {
		.api_version	= "2.0",
		.name		= "Individuals Disclosures",
		.authz		= AUTHZ_USER,
		.scope		= MTD_API_SCOPE_SA,
	},
	[MTD_EP_API_IE] = {
		.api_version	= "3.0",
		.name		= "Individuals Expenses",
		.authz		= AUTHZ_USER,
		.scope		= MTD_API_SCOPE_SA,
	},
	[MTD_EP_API_IEI] = {
		.api_version	= "1.0",
		.name		= "Individuals Employments Income",
		.authz		= AUTHZ_USER,
		.scope		= MTD_API_SCOPE_SA,
	},
	[MTD_EP_API_ILOS] = {
		.api_version	= "5.0",
		.name		= "Individual Losses",
		.authz		= AUTHZ_USER,
		.scope		= MTD_API_SCOPE_SA,
	},
	[MTD_EP_API_IPI] = {
		.api_version	= "1.0",
		.name		= "Individuals Pensions Income",
		.authz		= AUTHZ_USER,
		.scope		= MTD_API_SCOPE_SA,
	},
	[MTD_EP_API_ISB] = {
		.api_version	= "2.0",
		.name		= "Individuals State Benefits",
		.authz		= AUTHZ_USER,
		.scope		= MTD_API_SCOPE_SA,
	},
	[MTD_EP_API_ISI] = {
		.api_version	= "1.0",
		.name		= "Individuals Savings Income",
		.authz		= AUTHZ_USER,
		.scope		= MTD_API_SCOPE_SA,
	},
	[MTD_EP_API_OB] = {
		.api_version	= "3.0",
		.name		= "Obligations",
		.authz		= AUTHZ_USER,
		.scope		= MTD_API_SCOPE_SA,
	},
	[MTD_EP_API_PB] = {
		.api_version	= "5.0",
		.name		= "Property Business",
		.authz		= AUTHZ_USER,
		.scope		= MTD_API_SCOPE_SA,
	},
	[MTD_EP_API_SAA] = {
		.api_version	= "3.0",
		.name		= "Self Assessment Accounts",
		.authz		= AUTHZ_USER,
		.scope		= MTD_API_SCOPE_SA,
	},
	[MTD_EP_API_SAASS] = {
		.api_version	= "1.0",
		.name		= "Self Assessment Assist",
		.authz		= AUTHZ_USER,
		.scope		= MTD_API_SCOPE_SAASS,
	},
	[MTD_EP_API_SAID] = {
		.api_version	= "2.0",
		.name		= "Self Assessment Individual Details",
		.authz		= AUTHZ_USER,
		.scope		= MTD_API_SCOPE_SA,
	},
	[MTD_EP_API_SEB] = {
		.api_version	= "4.0",
		.name		= "Self Employment Business",
		.authz		= AUTHZ_USER,
		.scope		= MTD_API_SCOPE_SA,
	},

	[MTD_EP_API_VAT] = {
		.api_version	= "1.0",
		.name		= "VAT",
		.authz		= AUTHZ_USER,
		.scope		= MTD_API_SCOPE_VAT,
	},

	[MTD_EP_API_TEST_CU] = {
		.api_version	= "1.0",
		.name		= "Create Test User",
		.authz		= AUTHZ_APPLICATION,
		.scope		= MTD_API_SCOPE_NULL,
	},
	[MTD_EP_API_TEST_FPH] = {
		.api_version	= "1.0",
		.name		= "Test Fraud Prevention Headers",
		.authz		= AUTHZ_APPLICATION,
		.scope		= MTD_API_SCOPE_NULL,
	},
	[MTD_EP_API_TEST_SATS] = {
		.api_version	= "1.0",
		.name		= "Self Assessment Test Support",
		.authz		= AUTHZ_APPLICATION,
		.scope		= MTD_API_SCOPE_SA,
	},
};

/*
 * The order of these entries must match the order in the
 * mtd_api_endpoint enum in include/libmtdac/mtd.h
 */
static const struct {
	const char *epstr;
	const char *tmpl;
	enum http_method method;
	enum content_type ctype;
	enum mtd_ep_api api;
	enum oauth_authz authz;
	enum mtd_api_scope scope;
} endpoints[] = {
	/* Business Details */
	[MTD_API_EP_BD_LIST] = {
		.epstr	= "MTD_API_EP_BD_LIST",
		.tmpl	= "/individuals/business/details/{nino}/list",
		.method	= M_GET,
		.api	= MTD_EP_API_BD,
	},
	[MTD_API_EP_BD_GET] = {
		.epstr	= "MTD_API_EP_BD_GET",
		.tmpl	= "/individuals/business/details/{nino}/{businessId}",
		.method	= M_GET,
		.api	= MTD_EP_API_BD,
	},
	[MTD_API_EP_BD_AMEND_QPT] = {
		.epstr	= "MTD_API_EP_BD_AMEND_QPT",
		.tmpl	= "/individuals/business/details/{nino}/{businessId}/{taxYear}",
		.method	= M_PUT,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= MTD_EP_API_BD,
	},

	/* Business Income Source Summary */
	[MTD_API_EP_BISS_GET] = {
		.epstr	= "MTD_API_EP_BISS_GET",
		.tmpl	= "/individuals/self-assessment/income-summary/{nino}/{typeOfBusiness}/{taxYear}/{businessId}",
		.method = M_GET,
		.api	= MTD_EP_API_BISS,
	},

	/* Business Source Adjustable Summary */
	[MTD_API_EP_BSAS_LIST] = {
		.epstr	= "MTD_API_EP_BSAS_LIST",
		.tmpl	= "/individuals/self-assessment/adjustable-summary/{nino}/{taxYear}/{query_params}",
		.method	= M_GET,
		.api	= MTD_EP_API_BSAS,
	},
	[MTD_API_EP_BSAS_TRIGGER] = {
		.epstr	= "MTD_API_EP_BSAS_TRIGGER",
		.tmpl	= "/individuals/self-assessment/adjustable-summary/{nino}/trigger",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= MTD_EP_API_BSAS,
	},
	/* Self-Employment */
	[MTD_API_EP_BSAS_SE_GET] = {
		.epstr	= "MTD_API_EP_BSAS_SE_GET",
		.tmpl	= "/individuals/self-assessment/adjustable-summary/{nino}/self-employment/{calculationId}/{taxYear}",
		.method	= M_GET,
		.api	= MTD_EP_API_BSAS,
	},
	[MTD_API_EP_BSAS_SE_SUBMIT] = {
		.epstr	= "MTD_API_EP_BSAS_SE_SUBMIT",
		.tmpl	= "/individuals/self-assessment/adjustable-summary/{nino}/self-employment/{calculationId}/adjust/{taxYear}",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= MTD_EP_API_BSAS,
	},
	/* UK Property */
	[MTD_API_EP_BSAS_PB_GET] = {
		.epstr	= "MTD_API_EP_BSAS_PB_GET",
		.tmpl	= "/individuals/self-assessment/adjustable-summary/{nino}/uk-property/{calculationId}/{taxYear}",
		.method	= M_GET,
		.api	= MTD_EP_API_BSAS,
	},
	[MTD_API_EP_BSAS_PB_SUBMIT] = {
		.epstr	= "MTD_API_EP_BSAS_PB_SUBMIT",
		.tmpl	= "/individuals/self-assessment/adjustable-summary/{nino}/uk-property/{calculationId}/{taxYear}",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= MTD_EP_API_BSAS,
	},
	/* Foreign Property */
	[MTD_API_EP_BSAS_FP_GET] = {
		.epstr	= "MTD_API_EP_BSAS_FP_GET",
		.tmpl	= "/individuals/self-assessment/adjustable-summary/{nino}/foreign-property/{calculationId}/{taxYear}",
		.method	= M_GET,
		.api	= MTD_EP_API_BSAS,
	},
	[MTD_API_EP_BSAS_FP_SUBMIT] = {
		.epstr	= "MTD_API_EP_BSAS_FP_SUBMIT",
		.tmpl	= "/individuals/self-assessment/adjustable-summary/{nino}/foreign-property/{calculationId}/adjust/{taxYear}",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= MTD_EP_API_BSAS,
	},

	/* Individual Calculations - Tax Calculations */
	[MTD_API_EP_ICAL_TRIGGER] = {
		.epstr	= "MTD_API_EP_ICAL_TRIGGER",
		.tmpl	= "/individuals/calculations/{nino}/self-assessment/{taxYear}/trigger/{calculationType}",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_NONE,
		.api	= MTD_EP_API_ICAL,
	},
	[MTD_API_EP_ICAL_LIST_OLD] = {
		.epstr	= "MTD_API_EP_ICAL_LIST_OLD",
		.tmpl	= "/individuals/calculations/{nino}/self-assessment/{query_params}",
		.method	= M_GET,
		.api	= MTD_EP_API_ICAL,
	},
	[MTD_API_EP_ICAL_LIST] = {
		.epstr	= "MTD_API_EP_ICAL_LIST",
		.tmpl	= "/individuals/calculations/{nino}/self-assessment/{taxYear}/{query_params}",
		.method	= M_GET,
		.api	= MTD_EP_API_ICAL,
	},
	[MTD_API_EP_ICAL_GET] = {
		.epstr	= "MTD_API_EP_ICAL_GET",
		.tmpl	= "/individuals/calculations/{nino}/self-assessment/{taxYear}/{calculationId}",
		.method	= M_GET,
		.api	= MTD_EP_API_ICAL,
	},
	/* Final Declaration */
	[MTD_API_EP_ICAL_FINAL_DECLARATION] = {
		.epstr	= "MTD_API_EP_ICAL_FINAL_DECLARATION",
		.tmpl	= "/individuals/calculations/{nino}/self-assessment/{taxYear}/{calculationId}/{calculationType}",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_NONE,
		.api	= MTD_EP_API_ICAL,
	},

	/* Individuals Disclosures - Marriage Allowance */
	[MTD_API_EP_ID_MA_CREATE] = {
		.epstr	= "MTD_API_EP_ID_MA_CREATE",
		.tmpl	= "/individuals/disclosures/marriage-allowance/{nino}",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= MTD_EP_API_ID,
	},
	/* Disclosures */
	[MTD_API_EP_ID_D_GET] = {
		.epstr	= "MTD_API_EP_ID_D_GET",
		.tmpl	= "/individuals/disclosures/{nino}/{taxYear}",
		.method	= M_GET,
		.api	= MTD_EP_API_ID,
	},
	[MTD_API_EP_ID_D_AMEND] = {
		.epstr	= "MTD_API_EP_ID_D_AMEND",
		.tmpl	= "/individuals/disclosures/{nino}/{taxYear}",
		.method	= M_PUT,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= MTD_EP_API_ID,
	},
	[MTD_API_EP_ID_D_DELETE] = {
		.epstr	= "MTD_API_EP_ID_D_DELETE",
		.tmpl	= "/individuals/disclosures/{nino}/{taxYear}",
		.method	= M_DELETE,
		.api	= MTD_EP_API_ID,
	},

	/* Individuals State Benefits */
	[MTD_API_EP_ISB_CREATE] = {
		.epstr	= "MTD_API_EP_ISB_CREATE",
		.tmpl	= "/individuals/state-benefits/{nino}/{taxYear}",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= MTD_EP_API_ISB,
	},
	[MTD_API_EP_ISB_LIST] = {
		.epstr	= "MTD_API_EP_ISB_LIST",
		.tmpl	= "/individuals/state-benefits/{nino}/{taxYear}/{query_params}",
		.method	= M_GET,
		.api	= MTD_EP_API_ISB,
	},
	[MTD_API_EP_ISB_AMEND] = {
		.epstr	= "MTD_API_EP_ISB_AMEND",
		.tmpl	= "/individuals/state-benefits/{nino}/{taxYear}/{benefitId}",
		.method	= M_PUT,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= MTD_EP_API_ISB,
	},
	[MTD_API_EP_ISB_DELETE] = {
		.epstr	= "MTD_API_EP_ISB_DELETE",
		.tmpl	= "/individuals/state-benefits/{nino}/{taxYear}/{benefitId}",
		.method	= M_DELETE,
		.api	= MTD_EP_API_ISB,
	},
	[MTD_API_EP_ISB_AMEND_AMNTS] = {
		.epstr	= "MTD_API_EP_ISB_AMEND_AMNTS",
		.tmpl	= "/individuals/state-benefits/{nino}/{taxYear}/{benefitId}/amounts",
		.method	= M_PUT,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= MTD_EP_API_ISB,
	},
	[MTD_API_EP_ISB_DELETE_AMNTS] = {
		.epstr	= "MTD_API_EP_ISB_DELETE_AMNTS",
		.tmpl	= "/individuals/state-benefits/{nino}/{taxYear}/{benefitId}/amounts",
		.method	= M_DELETE,
		.api	= MTD_EP_API_ISB,
	},
	[MTD_API_EP_ISB_IGNORE] = {
		.epstr	= "MTD_API_EP_ISB_IGNORE",
		.tmpl	= "/individuals/state-benefits/{nino}/{taxYear}/{benefitId}/ignore",
		.method = M_POST,
		.ctype	= CONTENT_TYPE_NONE,
		.api	= MTD_EP_API_ISB,
	},
	[MTD_API_EP_ISB_UNIGNORE] = {
		.epstr	= "MTD_API_EP_ISB_UNIGNORE",
		.tmpl	= "/individuals/state-benefits/{nino}/{taxYear}/{benefitId}/unignore",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_NONE,
		.api	= MTD_EP_API_ISB,
	},

	/* Individuals Employments Income - Employments */
	[MTD_API_EP_IEI_E_LIST] = {
		.epstr	= "MTD_API_EP_IEI_E_LIST",
		.tmpl	= "/individuals/employments-income/{nino}/{taxYear}",
		.method	= M_GET,
		.api	= MTD_EP_API_IEI,
	},
	[MTD_API_EP_IEI_E_ADD] = {
		.epstr	= "MTD_API_EP_IEI_E_ADD",
		.tmpl	= "/individuals/employments-income/{nino}/{taxYear}",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= MTD_EP_API_IEI,
	},
	[MTD_API_EP_IEI_E_GET] = {
		.epstr	= "MTD_API_EP_IEI_E_GET",
		.tmpl	= "/individuals/employments-income/{nino}/{taxYear}/{employmentId}",
		.method	= M_GET,
		.api	= MTD_EP_API_IEI,
	},
	[MTD_API_EP_IEI_E_AMEND] = {
		.epstr	= "MTD_API_EP_IEI_E_AMEND",
		.tmpl	= "/individuals/employments-income/{nino}/{taxYear}/{employmentId}",
		.method	= M_PUT,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= MTD_EP_API_IEI,
	},
	[MTD_API_EP_IEI_E_DELETE] = {
	       .epstr	= "MTD_API_EP_IEI_E_DELETE",
	       .tmpl	= "/individuals/employments-income/{nino}/{taxYear}/{employmentId}",
	       .method	= M_DELETE,
	       .api	= MTD_EP_API_IEI,
	},
	[MTD_API_EP_IEI_E_IGNORE] = {
		.epstr	= "MTD_API_EP_IEI_E_IGNORE",
		.tmpl	= "/individuals/employments-income/{nino}/{taxYear}/{employmentId}/ignore",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_NONE,
		.api	= MTD_EP_API_IEI,
	},
	[MTD_API_EP_IEI_E_UNIGNORE] = {
		.epstr	= "MTD_API_EP_IEI_E_UNIGNORE",
		.tmpl	= "/individuals/employments-income/{nino}/{taxYear}/{employmentId}/unignore",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_NONE,
		.api	= MTD_EP_API_IEI,
	},
	[MTD_API_EP_IEI_E_EFD_GET] = {
		.epstr	= "MTD_API_EP_IEI_E_EFD_GET",
		.tmpl	= "/individuals/employments-income/{nino}/{taxYear}/{employmentId}/financial-details/{query_params}",
		.method	= M_GET,
		.api	= MTD_EP_API_IEI,
	},
	[MTD_API_EP_IEI_E_EFD_AMEND] = {
		.epstr	= "MTD_API_EP_IEI_E_EFD_AMEND",
		.tmpl	= "/individuals/employments-income/{nino}/{taxYear}/{employmentId}/financial-details",
		.method	= M_PUT,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= MTD_EP_API_IEI,
	},
	[MTD_API_EP_IEI_E_EFD_DELETE] = {
		.epstr	= "MTD_API_EP_IEI_E_EFD_DELETE",
		.tmpl	= "/individuals/employments-income/{nino}/{taxYear}/{employmentId}/financial-details",
		.method	= M_DELETE,
		.api	= MTD_EP_API_IEI,
	},
	/* Non-PAYE Employment Income */
	[MTD_API_EP_IEI_NPE_GET] = {
		.epstr	= "MTD_API_EP_IEI_NPE_GET",
		.tmpl	= "/individuals/employments-income/non-paye/{nino}/{taxYear}/{query_params}",
		.method	= M_GET,
		.api	= MTD_EP_API_IEI,
	},
	[MTD_API_EP_IEI_NPE_AMEND] = {
		.epstr	= "MTD_API_EP_IEI_NPE_AMEND",
		.tmpl	= "/individuals/employments-income/non-paye/{nino}/{taxYear}",
		.method	= M_PUT,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= MTD_EP_API_IEI,
	},
	[MTD_API_EP_IEI_NPE_DELETE] = {
		.epstr	= "MTD_API_EP_IEI_NPE_DELETE",
		.tmpl	= "/individuals/employments-income/non-paye/{nino}/{taxYear}",
		.method	= M_DELETE,
		.api	= MTD_EP_API_IEI,
	},
	/* Other Employment Income */
	[MTD_API_EP_IEI_OE_GET] = {
		.epstr	= "MTD_API_EP_IEI_OE_GET",
		.tmpl	= "/individuals/employments-income/other/{nino}/{taxYear}",
		.method	= M_GET,
		.api	= MTD_EP_API_IEI,
	},
	[MTD_API_EP_IEI_OE_AMEND] = {
		.epstr	= "MTD_API_EP_IEI_OE_AMEND",
		.tmpl	= "/individuals/employments-income/other/{nino}/{taxYear}",
		.method	= M_PUT,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= MTD_EP_API_IEI,
	},
	[MTD_API_EP_IEI_OE_DELETE] = {
		.epstr	= "MTD_API_EP_IEI_OE_DELETE",
		.tmpl	= "/individuals/employments-income/other/{nino}/{taxYear}",
		.method	= M_DELETE,
		.api	= MTD_EP_API_IEI,
	},

	/* Individuals Expenses - Employment Expenses */
	[MTD_API_EP_IE_EE_AMEND] = {
		.epstr	= "MTD_API_EP_IE_EE_AMEND",
		.tmpl	= "/individuals/expenses/employments/{nino}/{taxYear}",
		.method	= M_PUT,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= MTD_EP_API_IE,
	},
	[MTD_API_EP_IE_EE_GET] = {
		.epstr	= "MTD_API_EP_IE_EE_GET",
		.tmpl	= "/individuals/expenses/employments/{nino}/{taxYear}/{query_params}",
		.method	= M_GET,
		.api	= MTD_EP_API_IE,
	},
	[MTD_API_EP_IE_EE_DELETE] = {
		.epstr	= "MTD_API_EP_IE_EE_DELETE",
		.tmpl	= "/individuals/expenses/employments/{nino}/{taxYear}",
		.method	= M_DELETE,
		.api	= MTD_EP_API_IE,
	},
	[MTD_API_EP_IE_EE_IGNORE] = {
		.epstr	= "MTD_API_EP_IE_EE_IGNORE",
		.tmpl	= "/individuals/expenses/employments/{nino}/{taxYear}/ignore",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_NONE,
		.api	= MTD_EP_API_IE,
	},
	/* Other Expenses */
	[MTD_API_EP_IE_OE_AMEND] = {
		.epstr	= "MTD_API_EP_IE_OE_AMEND",
		.tmpl	= "/individuals/expenses/other/{nino}/{taxYear}",
		.method	= M_PUT,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= MTD_EP_API_IE,
	},
	[MTD_API_EP_IE_OE_GET] = {
		.epstr	= "MTD_API_EP_IE_OE_GET",
		.tmpl	= "/individuals/expenses/other/{nino}/{taxYear}",
		.method	= M_GET,
		.api	= MTD_EP_API_IE,
	},
	[MTD_API_EP_IE_OE_DELETE] = {
		.epstr	= "MTD_API_EP_IE_OE_DELETE",
		.tmpl	= "/individuals/expenses/other/{nino}/{taxYear}",
		.method	= M_DELETE,
		.api	= MTD_EP_API_IE,
	},

	/* Individual Losses - Brought Forward */
	[MTD_API_EP_ILOS_BF_CREATE] = {
		.epstr	= "MTD_API_EP_ILOS_BF_CREATE",
		.tmpl	= "/individuals/losses/{nino}/brought-forward-losses",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= MTD_EP_API_ILOS,
	},
	[MTD_API_EP_ILOS_BF_AMEND_AMNT] = {
		.epstr	= "MTD_API_EP_ILOS_BF_AMEND_AMNT",
		.tmpl	= "/individuals/losses/{nino}/brought-forward-losses/{lossId}/change-loss-amount",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= MTD_EP_API_ILOS,
	},
	[MTD_API_EP_ILOS_BF_LIST] = {
		.epstr	= "MTD_API_EP_ILOS_BF_LIST",
		.tmpl	= "/individuals/losses/{nino}/brought-forward-losses/tax-year/{taxYearBroughtForwardFrom}/{query_params}",
		.method	= M_GET,
		.api	= MTD_EP_API_ILOS,
	},
	[MTD_API_EP_ILOS_BF_GET] = {
		.epstr	= "MTD_API_EP_ILOS_BF_GET",
		.tmpl	= "/individuals/losses/{nino}/brought-forward-losses/{lossId}",
		.method	= M_GET,
		.api	= MTD_EP_API_ILOS,
	},
	[MTD_API_EP_ILOS_BF_DELETE] = {
		.epstr	= "MTD_API_EP_ILOS_BF_DELETE",
		.tmpl	= "/individuals/losses/{nino}/brought-forward-losses/{lossId}",
		.method	= M_DELETE,
		.api	= MTD_EP_API_ILOS,
	},
	/* Loss Claims */
	[MTD_API_EP_ILOS_LC_CREATE] = {
		.epstr	= "MTD_API_EP_ILOS_LC_CREATE",
		.tmpl	= "/individuals/losses/{nino}/loss-claims",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= MTD_EP_API_ILOS,
	},
	[MTD_API_EP_ILOS_LC_LIST] = {
		.epstr	= "MTD_API_EP_ILOS_LC_LIST",
		.tmpl	= "/individuals/losses/{nino}/loss-claims/tax-year/{taxYearClaimedFor}/{query_params}",
		.method	= M_GET,
		.api	= MTD_EP_API_ILOS,
	},
	[MTD_API_EP_ILOS_LC_GET] = {
		.epstr	= "MTD_API_EP_ILOS_LC_GET",
		.tmpl	= "/individuals/losses/{nino}/loss-claims/{claimId}",
		.method	= M_GET,
		.api	= MTD_EP_API_ILOS,
	},
	[MTD_API_EP_ILOS_LC_DELETE] = {
		.epstr	= "MTD_API_EP_ILOS_LC_DELETE",
		.tmpl	= "/individuals/losses/{nino}/loss-claims/{claimId}",
		.method	= M_DELETE,
		.api	= MTD_EP_API_ILOS,
	},
	[MTD_API_EP_ILOS_LC_AMEND_TYPE] = {
		.epstr	= "MTD_API_EP_ILOS_LC_AMEND_TYPE",
		.tmpl	= "/individuals/losses/{nino}/loss-claims/{claimId}/change-type-of-claim",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= MTD_EP_API_ILOS,
	},
	[MTD_API_EP_ILOS_LC_AMEND_ORDER] = {
		.epstr	= "MTD_API_EP_ILOS_LC_AMEND_ORDER",
		.tmpl	= "/individuals/losses/{nino}/loss-claims/order/{taxYearClaimedFor}",
		.method	= M_PUT,
		.ctype  = CONTENT_TYPE_JSON,
		.api	= MTD_EP_API_ILOS,
	},

	/* Individuals Pensions Income */
	[MTD_API_EP_IPI_GET] = {
		.epstr	= "MTD_API_EP_IPI_GET",
		.tmpl	= "/individuals/pensions-income/{nino}/{taxYear}",
		.method	= M_GET,
		.api	= MTD_EP_API_IPI,
	},
	[MTD_API_EP_IPI_AMEND] = {
		.epstr	= "MTD_API_EP_IPI_AMEND",
		.tmpl	= "/individuals/pensions-income/{nino}/{taxYear}",
		.method	= M_PUT,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= MTD_EP_API_IPI,
	},
	[MTD_API_EP_IPI_DELETE] = {
		.epstr	= "MTD_API_EP_IPI_DELETE",
		.tmpl	= "/individuals/pensions-income/{nino}/{taxYear}",
		.method	= M_DELETE,
		.api	= MTD_EP_API_IPI,
	},

	/* Individuals Savings Income - UK Savings Account */
	[MTD_API_EP_ISI_SI_UK_LIST] = {
		.epstr	= "MTD_API_EP_ISI_SI_UK_LIST",
		.tmpl	= "/individuals/savings-income/uk-accounts/{nino}/{query_params}",
		.method	= M_GET,
		.api	= MTD_EP_API_ISI,
	},
	[MTD_API_EP_ISI_SI_UK_ADD] = {
		.epstr	= "MTD_API_EP_ISI_SI_UK_ADD",
		.tmpl	= "/individuals/savings-income/uk-accounts/{nino}",
		.method	= M_POST,
		.ctype  = CONTENT_TYPE_JSON,
		.api	= MTD_EP_API_ISI,
	},
	[MTD_API_EP_ISI_SI_UK_GET_AS] = {
		.epstr	= "MTD_API_EP_ISI_SI_UK_GET_AS",
		.tmpl	= "/individuals/savings-income/uk-accounts/{nino}/{taxYear}/{savingsAccountId}",
		.method	= M_GET,
		.api	= MTD_EP_API_ISI,
	},
	[MTD_API_EP_ISI_SI_UK_UPDATE_AS] = {
		.epstr	= "MTD_API_EP_ISI_SI_UK_UPDATE_AS",
		.tmpl	= "/individuals/savings-income/uk-accounts/{nino}/{taxYear}/{savingsAccountId}",
		.method	= M_PUT,
		.ctype  = CONTENT_TYPE_JSON,
		.api	= MTD_EP_API_ISI,
	},
	/* Savings Income */
	[MTD_API_EP_ISI_SI_O_GET] = {
		.epstr	= "MTD_API_EP_ISI_SI_O_GET",
		.tmpl	= "/individuals/savings-income/other/{nino}/{taxYear}",
		.method	= M_GET,
		.api	= MTD_EP_API_ISI,
	},
	[MTD_API_EP_ISI_SI_O_UPDATE] = {
		.epstr	= "MTD_API_EP_ISI_SI_O_UPDATE",
		.tmpl	= "/individuals/savings-income/other/{nino}/{taxYear}",
		.method	= M_PUT,
		.ctype  = CONTENT_TYPE_JSON,
		.api	= MTD_EP_API_ISI,
	},
	[MTD_API_EP_ISI_SI_O_DELETE] = {
		.epstr	= "MTD_API_EP_ISI_SI_O_DELETE",
		.tmpl	= "/individuals/savings-income/other/{nino}/{taxYear}",
		.method = M_DELETE,
		.api	= MTD_EP_API_ISI,
	},

	/* Obligations */
	[MTD_API_EP_OB_GET_IEO] = {
		.epstr	= "MTD_API_EP_OB_GET_IEO",
		.tmpl	= "/obligations/details/{nino}/income-and-expenditure/{query_params}",
		.method	= M_GET,
		.api	= MTD_EP_API_OB,
	},
	[MTD_API_EP_OB_GET_FDO] = {
		.epstr	= "MTD_API_EP_OB_GET_FDO",
		.tmpl	= "/obligations/details/{nino}/crystallisation}/{query_params}",
		.method	= M_GET,
		.api	= MTD_EP_API_OB,
	},
	[MTD_API_EP_OB_GET_EPSO] = {
		.epstr	= "MTD_API_EP_OB_GET_EPSO",
		.tmpl	= "/obligations/details/{nino}/end-of-period-statement/{query_params}",
		.method	= M_GET,
		.api	= MTD_EP_API_OB,
	},

	/* Property Business - UK Property Business Annual Submission */
	[MTD_API_EP_PB_UKPBAS_GET] = {
		.epstr	= "MTD_API_EP_PB_UKPBAS_GET",
		.tmpl	= "/individuals/business/property/uk/{nino}/{businessId}/annual/{taxYear}",
		.method	= M_GET,
		.api	= MTD_EP_API_PB,
	},
	[MTD_API_EP_PB_UKPBAS_CREATE] = {
		.epstr	= "MTD_API_EP_PB_UKPBAS_CREATE",
		.tmpl	= "/individuals/business/property/uk/{nino}/{businessId}/annual/{taxYear}",
		.method	= M_PUT,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= MTD_EP_API_PB,
	},
	/* UK Property Income & Expenses Period Summary */
	[MTD_API_EP_PB_UKPIEPS_CREATE] = {
		.epstr	= "MTD_API_EP_PB_UKPIEPS_CREATE",
		.tmpl	= "/individuals/business/property/uk/{nino}/{businessId}/period/{taxYear}",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= MTD_EP_API_PB,
	},
	[MTD_API_EP_PB_UKPIEPS_GET] = {
		.epstr	= "MTD_API_EP_PB_UKPIEPS_GET",
		.tmpl	= "/individuals/business/property/uk/{nino}/{businessId}/period/{taxYear}/{submissionId}",
		.method	= M_GET,
		.api	= MTD_EP_API_PB,
	},
	[MTD_API_EP_PB_UKPIEPS_AMEND] = {
		.epstr	= "MTD_API_EP_PB_UKPIEPS_AMEND",
		.tmpl	= "/individuals/business/property/uk/{nino}/{businessId}/period/{taxYear}/{submissionId}",
		.method	= M_PUT,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= MTD_EP_API_PB,
	},
	/* UK Property Cumulative Period Summary */
	[MTD_API_EP_PB_UKPCPS_GET] = {
		.epstr	= "MTD_API_EP_PB_UKPCPS_GET",
		.tmpl	= "/individuals/business/property/uk/{nino}/{businessId}/cumulative/{taxYear}",
		.method	= M_GET,
		.api	= MTD_EP_API_PB,
	},
	[MTD_API_EP_PB_UKPCPS_CREATE] = {
		.epstr	= "MTD_API_EP_PB_UKPCPS_CREATE",
		.tmpl	= "/individuals/business/property/uk/{nino}/{businessId}/cumulative/{taxYear}",
		.method	= M_PUT,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= MTD_EP_API_PB,
	},
	/* Historic FHL UK Property Business Annual Submission */
	[MTD_API_EP_PB_HFHL_UKPBAS_CREATE] = {
		.epstr	= "MTD_API_EP_PB_HFHL_UKPBAS_CREATE",
		.tmpl	= "/individuals/business/property/uk/annual/furnished-holiday-lettings/{nino}/{taxYear}",
		.method	= M_PUT,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= MTD_EP_API_PB,
	},
	[MTD_API_EP_PB_HFHL_UKPBAS_GET] = {
		.epstr	= "MTD_API_EP_PB_HFHL_UKPBAS_GET",
		.tmpl	= "/individuals/business/property/uk/annual/furnished-holiday-lettings/{nino}/{taxYear}",
		.method	= M_GET,
		.api	= MTD_EP_API_PB,
	},
	[MTD_API_EP_PB_HFHL_UKPBAS_DELETE] = {
		.epstr	= "MTD_API_EP_PB_HFHL_UKPBAS_DELETE",
		.tmpl	= "/individuals/business/property/uk/annual/furnished-holiday-lettings/{nino}/{taxYear}",
		.method	= M_DELETE,
		.api	= MTD_EP_API_PB,
	},
	/* Historic non-FHL UK Property Business Annual Submission */
	[MTD_API_EP_PB_HNFHL_UKPBAS_CREATE] = {
		.epstr	= "MTD_API_EP_PB_HNFHL_UKPBAS_CREATE",
		.tmpl	= "/individuals/business/property/uk/annual/non-furnished-holiday-lettings/{nino}/{taxYear}",
		.method	= M_PUT,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= MTD_EP_API_PB,
	},
	[MTD_API_EP_PB_HNFHL_UKPBAS_GET] = {
		.epstr	= "MTD_API_EP_PB_HNFHL_UKPBAS_GET",
		.tmpl	= "/individuals/business/property/uk/annual/non-furnished-holiday-lettings/{nino}/{taxYear}",
		.method	= M_GET,
		.api	= MTD_EP_API_PB,
	},
	[MTD_API_EP_PB_HNFHL_UKPBAS_DELETE] = {
		.epstr	= "MTD_API_EP_PB_HNFHL_UKPBAS_DELETE",
		.tmpl	= "/individuals/business/property/uk/annual/non-furnished-holiday-lettings/{nino}/{taxYear}",
		.method	= M_DELETE,
		.api	= MTD_EP_API_PB,
	},
	/* Historic FHL UK Property Income & Expenses Period Summary */
	[MTD_API_EP_PB_HFHL_UKPIEPS_LIST] = {
		.epstr	= "MTD_API_EP_PB_HFHL_UKPIEPS_LIST",
		.tmpl	= "/individuals/business/property/uk/period/furnished-holiday-lettings/{nino}",
		.method	= M_GET,
		.api	= MTD_EP_API_PB,
	},
	[MTD_API_EP_PB_HFHL_UKPIEPS_CREATE] = {
		.epstr	= "MTD_API_EP_PB_HFHL_UKPIEPS_CREATE",
		.tmpl	= "/individuals/business/property/uk/period/furnished-holiday-lettings/{nino}",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= MTD_EP_API_PB,
	},
	[MTD_API_EP_PB_HFHL_UKPIEPS_AMEND] = {
		.epstr	= "MTD_API_EP_PB_HFHL_UKPIEPS_AMEND",
		.tmpl	= "/individuals/business/property/uk/period/furnished-holiday-lettings/{nino}/{periodId}",
		.method	= M_PUT,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= MTD_EP_API_PB,
	},
	[MTD_API_EP_PB_HFHL_UKPIEPS_GET] = {
		.epstr	= "MTD_API_EP_PB_HFHL_UKPIEPS_GET",
		.tmpl	= "/individuals/business/property/uk/period/furnished-holiday-lettings/{nino}/{periodId}",
		.method	= M_GET,
		.api	= MTD_EP_API_PB,
	},
	/* Historic non-FHL UK Property Income & Expenses Period Summary */
	[MTD_API_EP_PB_HNFHL_UKPIEPS_LIST] = {
		.epstr	= "MTD_API_EP_PB_HNFHL_UKPIEPS_LIST",
		.tmpl	= "/individuals/business/property/uk/period/non-furnished-holiday-lettings/{nino}",
		.method	= M_GET,
		.api	= MTD_EP_API_PB,
	},
	[MTD_API_EP_PB_HNFHL_UKPIEPS_CREATE] = {
		.epstr	= "MTD_API_EP_PB_HNFHL_UKPIEPS_CREATE",
		.tmpl	= "/individuals/business/property/uk/period/non-furnished-holiday-lettings/{nino}",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= MTD_EP_API_PB,
	},
	[MTD_API_EP_PB_HNFHL_UKPIEPS_GET] = {
		.epstr	= "MTD_API_EP_PB_HNFHL_UKPIEPS_GET",
		.tmpl	= "/individuals/business/property/uk/period/non-furnished-holiday-lettings/{nino}/{periodId}",
		.method	= M_GET,
		.api	= MTD_EP_API_PB,
	},
	[MTD_API_EP_PB_HNFHL_UKPIEPS_AMEND] = {
		.epstr	= "MTD_API_EP_PB_HNFHL_UKPIEPS_AMEND",
		.tmpl	= "/individuals/business/property/uk/period/non-furnished-holiday-lettings/{nino}/{periodId}",
		.method	= M_PUT,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= MTD_EP_API_PB,
	},
	/* Foreign Property Income & Expenses Period Summary */
	[MTD_API_EP_PB_FPIEPS_CREATE] = {
		.epstr	= "MTD_API_EP_PB_FPIEPS_CREATE",
		.tmpl	= "/individuals/business/property/foreign/{nino}/{businessId}/period/{taxYear}",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= MTD_EP_API_PB,
	},
	[MTD_API_EP_PB_FPIEPS_GET] = {
		.epstr	= "MTD_API_EP_PB_FPIEPS_GET",
		.tmpl	= "/individuals/business/property/foreign/{nino}/{businessId}/period/{taxYear}/{submissionId}",
		.method	= M_GET,
		.api	= MTD_EP_API_PB,
	},
	[MTD_API_EP_PB_FPIEPS_AMEND] = {
		.epstr	= "MTD_API_EP_PB_FPIEPS_AMEND",
		.tmpl	= "/individuals/business/property/foreign/{nino}/{businessId}/period/{taxYear}/{submissionId}",
		.method	= M_PUT,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= MTD_EP_API_PB,
	},
	/* Foreign Property Cumulative Period Summary */
	[MTD_API_EP_PB_FPCPS_GET] = {
		.epstr	= "MTD_API_EP_PB_FPCPS_GET",
		.tmpl	= "/individuals/business/property/foreign/{nino}/{businessId}/cumulative/{taxYear}",
		.method	= M_GET,
		.api	= MTD_EP_API_PB,
	},
	[MTD_API_EP_PB_FPCPS_AMEND] = {
		.epstr	= "MTD_API_EP_PB_FPCPS_AMEND",
		.tmpl	= "/individuals/business/property/foreign/{nino}/{businessId}/cumulative/{taxYear}",
		.method	= M_PUT,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= MTD_EP_API_PB,
	},
	/* Foreign Property Annual Submission */
	[MTD_API_EP_PB_FPAS_GET] = {
		.epstr	= "MTD_API_EP_PB_FPAS_GET",
		.tmpl	= "/individuals/business/property/foreign/{nino}/{businessId}/annual/{taxYear}",
		.method	= M_GET,
		.api	= MTD_EP_API_PB,
	},
	[MTD_API_EP_PB_FPAS_AMEND] = {
		.epstr	= "MTD_API_EP_PB_FPAS_AMEND",
		.tmpl	= "/individuals/business/property/foreign/{nino}/{businessId}/annual/{taxYear}",
		.method	= M_PUT,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= MTD_EP_API_PB,
	},
	/* UK or Foreign Property Annual Submission Deletion */
	[MTD_API_EP_PB_AS_DELETE] = {
		.epstr	= "MTD_API_EP_PB_AS_DELETE",
		.tmpl	= "/individuals/business/property/{nino}/{businessId}/annual/{taxYear}",
		.method	= M_DELETE,
		.api	= MTD_EP_API_PB,
	},
	/* UK or Foreign Property Income and Expenses Period Summaries List */
	[MTD_API_EP_PB_PIEPS_LIST] = {
		.epstr = "MTD_API_EP_PB_PIEPS_LIST",
		.tmpl	= "/individuals/business/property/{nino}/{businessId}/period/{taxYear}",
		.method	= M_GET,
		.api	= MTD_EP_API_PB,
	},

	/* Self Assessment Accounts - Payments and Liabilities */
	[MTD_API_EP_SAA_PL_HIST] = {
		.epstr	= "MTD_API_EP_SAA_PL_HIST",
		.tmpl	= "/accounts/self-assessment/{nino}/charges/{transactionId}",
		.method	= M_GET,
		.api	= MTD_EP_API_SAA,
	},
	[MTD_API_EP_SAA_PL_GET_BY_TID] = {
		.epstr	= "MTD_API_EP_SAA_PL_GET_BY_TID",
		.tmpl	= "/accounts/self-assessment/{nino}/charges/transactionId/{transactionId}",
		.method	= M_GET,
		.api	= MTD_EP_API_SAA,
	},
	[MTD_API_EP_SAA_PL_GET_BY_CR] = {
		.epstr	= "MTD_API_EP_SAA_PL_GET_BY_CR",
		.tmpl	= "/accounts/self-assessment/{nino}/charges/chargeReference/{chargeReference}",
		.method	= M_GET,
		.api	= MTD_EP_API_SAA,
	},
	[MTD_API_EP_SAA_PL_GET_BAL_TRANS] = {
		.epstr	= "MTD_API_EP_SAA_PL_GET_BAL_TRANS",
		.tmpl	= "/accounts/self-assessment/{nino}/balance-and-transactions/{query_params}",
		.method	= M_GET,
		.api	= MTD_EP_API_SAA,
	},
	[MTD_API_EP_SAA_PL_PA_LIST] = {
		.epstr	= "MTD_API_EP_SAA_PL_PA_LIST",
		.tmpl	= "/accounts/self-assessment/{nino}/payments-and-allocations/{query_params}",
		.method	= M_GET,
		.api	= MTD_EP_API_SAA,
	},
	/* Coding Out Underpayments and Debts */
	[MTD_API_EP_SAA_COUD_GET] = {
		.epstr	= "MTD_API_EP_SAA_COUD_GET",
		.tmpl	= "/accounts/self-assessment/{nino}/{taxYear}/collection/tax-code/{query_params}",
		.method	= M_GET,
		.api	= MTD_EP_API_SAA,
	},
	[MTD_API_EP_SAA_COUD_AMEND] = {
		.epstr	= "MTD_API_EP_SAA_COUD_AMEND",
		.tmpl	= "/accounts/self-assessment/{nino}/{taxYear}/collection/tax-code",
		.method	= M_PUT,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= MTD_EP_API_SAA,
	},
	[MTD_API_EP_SAA_COUD_DELETE] = {
		.epstr	= "MTD_API_EP_SAA_COUD_DELETE",
		.tmpl	= "/accounts/self-assessment/{nino}/{taxYear}/collection/tax-code",
		.method	= M_DELETE,
		.api	= MTD_EP_API_SAA,
	},
	/* Coding Out Status */
	[MTD_API_EP_SAA_COS_OPT_OUT] = {
		.epstr	= "MTD_API_EP_SAA_COS_OPT_OUT",
		.tmpl	= "/accounts/self-assessment/{nino}/{taxYear}/collection/tax-code/coding-out/opt-out",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_NONE,
		.api	= MTD_EP_API_SAA,
	},
	[MTD_API_EP_SAA_COS_OPT_STATUS] = {
		.epstr	= "MTD_API_EP_SAA_COS_OPT_STATUS",
		.tmpl	= "/accounts/self-assessment/{nino}/{taxYear}/collection/tax-code/coding-out/status",
		.method = M_GET,
		.api	= MTD_EP_API_SAA,
	},
	[MTD_API_EP_SAA_COS_OPT_IN] = {
		.epstr	= "MTD_API_EP_SAA_COS_OPT_IN",
		.tmpl	= "/accounts/self-assessment/{nino}/{taxYear}/collection/tax-code/coding-out/opt-in",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_NONE,
		.api	= MTD_EP_API_SAA,
	},

	/* Self Assessment Assist */
	[MTD_API_EP_SAASS_REPORT_GEN] = {
		.epstr	= "MTD_API_EP_SAASS_REPORT_GEN",
		.tmpl	= "/individuals/self-assessment/assist/reports/{nino}/{taxYear}/{calculationId}",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_NONE,
		.api	= MTD_EP_API_SAASS,
	},
	[MTD_API_EP_SAASS_REPORT_ACK] = {
		.epstr	= "MTD_API_EP_SAASS_REPORT_ACK",
		.tmpl	= "/individuals/self-assessment/assist/reports/acknowledge/{nino}/{reportId}/{correlationId}",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_NONE,
		.api	= MTD_EP_API_SAASS,
	},

	/* Self Assessment Individual Details */
	[MTD_API_EP_SAID_STATUS] = {
		.epstr	= "MTD_API_EP_SAID_STATUS",
		.tmpl	= "/individuals/person/itsa-status/{nino}/{taxYear}/{query_params}",
		.method	= M_GET,
		.api	= MTD_EP_API_SAID,
	},

	/* Self Employment Business - Self-Employment Annual Submission */
	[MTD_API_EP_SEB_SEAS_AMEND] = {
		.epstr	= "MTD_API_EP_SEB_SEAS_AMEND",
		.tmpl	= "/individuals/business/self-employment/{nino}/{businessId}/annual/{taxYear}",
		.method	= M_PUT,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= MTD_EP_API_SEB,
	},
	[MTD_API_EP_SEB_SEAS_GET] = {
		.epstr	= "MTD_API_EP_SEB_SEAS_GET",
		.tmpl	= "/individuals/business/self-employment/{nino}/{businessId}/annual/{taxYear}",
		.method	= M_GET,
		.api	= MTD_EP_API_SEB,
	},
	[MTD_API_EP_SEB_SEAS_DELETE] = {
		.epstr	= "MTD_API_EP_SEB_SEAS_DELETE",
		.tmpl	= "/individuals/business/self-employment/{nino}/{businessId}/annual/{taxYear}",
		.method	= M_DELETE,
		.api	= MTD_EP_API_SEB,
	},
	/* Self-Employment Period Summaries */
	[MTD_API_EP_SEB_SEPS_CREATE] = {
		.epstr	= "MTD_API_EP_SEB_SEPS_CREATE",
		.tmpl	= "/individuals/business/self-employment/{nino}/{businessId}/period",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= MTD_EP_API_SEB,
	},
	[MTD_API_EP_SEB_SEPS_LIST] = {
		.epstr	= "MTD_API_EP_SEB_SEPS_LIST",
		.tmpl	= "/individuals/business/self-employment/{nino}/{businessId}/period/{taxYear}",
		.method	= M_GET,
		.api	= MTD_EP_API_SEB,
	},
	[MTD_API_EP_SEB_SEPS_AMEND] = {
		.epstr	= "MTD_API_EP_SEB_SEPS_AMEND",
		.tmpl	= "/individuals/business/self-employment/{nino}/{businessId}/period/{taxYear}/{periodId}",
		.method	= M_PUT,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= MTD_EP_API_SEB,
	},
	[MTD_API_EP_SEB_SEPS_GET] = {
		.epstr	= "MTD_API_EP_SEB_SEPS_GET",
		.tmpl	= "/individuals/business/self-employment/{nino}/{businessId}/period/{taxYear}/{periodId}",
		.method	= M_GET,
		.api	= MTD_EP_API_SEB,
	},
	/* Self-Employment Cumulative Period Summary */
	[MTD_API_EP_SEB_SECPS_AMEND] = {
		.epstr	= "MTD_API_EP_SEB_SECPS_AMEND",
		.tmpl	= "/individuals/business/self-employment/{nino}/{businessId}/cumulative/{taxYear}",
		.method	= M_PUT,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= MTD_EP_API_SEB,
	},
	[MTD_API_EP_SEB_SECPS_GET] = {
		.epstr	= "MTD_API_EP_SEB_SECPS_GET",
		.tmpl	= "/individuals/business/self-employment/{nino}/{businessId}/cumulative/{taxYear}",
		.method	= M_GET,
		.api	= MTD_EP_API_SEB,
	},

	/* VAT */
	[MTD_API_EP_VAT_LIST_OBLIGATIONS] = {
		.epstr	= "MTD_API_EP_VAT_LIST_OBLIGATIONS",
		.tmpl	= "/organisations/vat/{vrn}/obligations/{query_params}",
		.method	= M_GET,
		.api	= MTD_EP_API_VAT,
	},
	[MTD_API_EP_VAT_SUBMIT] = {
		.epstr	= "MTD_API_EP_VAT_SUBMIT",
		.tmpl	= "/organisations/vat/{vrn}/returns",
		.method	= M_POST,
		.api	= MTD_EP_API_VAT,
	},
	[MTD_API_EP_VAT_VIEW_RETURN] = {
		.epstr	= "MTD_API_EP_VAT_VIEW_RETURN",
		.tmpl	= "/organisations/vat/{vrn}/returns/{periodKey}",
		.method	= M_GET,
		.api	= MTD_EP_API_VAT,
	},
	[MTD_API_EP_VAT_GET_LIABILITIES] = {
		.epstr	= "MTD_API_EP_VAT_GET_LIABILITIES",
		.tmpl	= "/organisations/vat/{vrn}/liabilities/{query_params}",
		.method	= M_GET,
		.api	= MTD_EP_API_VAT,
	},
	[MTD_API_EP_VAT_GET_PAYMENTS] = {
		.epstr	= "MTD_API_EP_VAT_GET_PAYMENTS",
		.tmpl	= "/organisations/vat/{vrn}/payments/{query_params}",
		.method	= M_GET,
		.api	= MTD_EP_API_VAT,
	},
	[MTD_API_EP_VAT_GET_PENALTIES] = {
		.epstr	= "MTD_API_EP_VAT_GET_PENALTIES",
		.tmpl	= "/organisations/vat/{vrn}/penalties",
		.method	= M_GET,
		.api	= MTD_EP_API_VAT,
	},
	[MTD_API_EP_VAT_GET_FINANCIAL_DETAILS] = {
		.epstr	= "MTD_API_EP_VAT_GET_FINANCIAL_DETAILS",
		.tmpl	= "/organisations/vat/{vrn}/financial-details/{penaltyChargeReference}",
		.method	= M_GET,
		.api	= MTD_EP_API_VAT,
	},

	/* Create Test User */
	[MTD_API_EP_TEST_CU_CREATE_INDIVIDUAL] = {
		.epstr	= "MTD_API_EP_TEST_CU_CREATE_INDIVIDUAL",
		.tmpl	= "/create-test-user/individuals",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= MTD_EP_API_TEST_CU,
	},
	[MTD_API_EP_TEST_CU_CREATE_ORGANISATION] = {
		.epstr	= "MTD_API_EP_TEST_CU_CREATE_ORGANISATION",
		.tmpl	= "/create-test-user/organisations",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= MTD_EP_API_TEST_CU,
	},
	[MTD_API_EP_TEST_CU_CREATE_AGENT] = {
		.epstr	= "MTD_API_EP_TEST_CU_CREATE_AGENT",
		.tmpl	= "/create-test-user/agents",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= MTD_EP_API_TEST_CU,
	},
	[MTD_API_EP_TEST_CU_LIST_SERVICES] = {
		.epstr	= "MTD_API_EP_TEST_CU_LIST_SERVICES",
		.tmpl	= "/create-test-user/services",
		.method	= M_GET,
		.api	= MTD_EP_API_TEST_CU,
	},

	/* Test Fraud Prevention Headers */
	[MTD_API_EP_TEST_FPH_VALIDATE] = {
		.epstr	= "MTD_API_EP_TEST_FPH_VALIDATE",
		.tmpl	= "/test/fraud-prevention-headers/validate",
		.method	= M_GET,
		.api	= MTD_EP_API_TEST_FPH,
	},
	[MTD_API_EP_TEST_FPH_FEEDBACK] = {
		.epstr	= "MTD_API_EP_TEST_FPH_FEEDBACK",
		.tmpl	= "/test/fraud-prevention-headers/{api}/validation-feedback/{query_params}",
		.method	= M_GET,
		.api	= MTD_EP_API_TEST_FPH,
	},

	/* Self Assessment Test Support */
	[MTD_API_EP_TEST_SATS_DELETE] = {
		.epstr	= "MTD_API_EP_TEST_SATS_DELETE",
		.tmpl	= "/individuals/self-assessment-test-support/vendor-state/{query_params}",
		.method = M_DELETE,
		.api	= MTD_EP_API_TEST_SATS,
	},
	/* Self Assessment Test Support Checkpoint for Vendor Data */
	[MTD_API_EP_TEST_SATS_CHKPT_LIST] = {
		.epstr	= "MTD_API_EP_TEST_SATS_CHKPT_LIST",
		.tmpl	= "/individuals/self-assessment-test-support/vendor-state/checkpoints/{query_params}",
		.method = M_GET,
		.api	= MTD_EP_API_TEST_SATS,
	},
	[MTD_API_EP_TEST_SATS_CHKPT_CREATE] = {
		.epstr	= "MTD_API_EP_TEST_SATS_CHKPT_CREATE",
		.tmpl	= "/individuals/self-assessment-test-support/vendor-state/checkpoints/{query_params}",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_NONE,
		.api	= MTD_EP_API_TEST_SATS,
	},
	[MTD_API_EP_TEST_SATS_CHKPT_DELETE] = {
		.epstr	= "MTD_API_EP_TEST_SATS_CHKPT_DELETE",
		.tmpl	= "/individuals/self-assessment-test-support/vendor-state/checkpoints/{checkpointId}",
		.method = M_DELETE,
		.api	= MTD_EP_API_TEST_SATS,
	},
	[MTD_API_EP_TEST_SATS_CHKPT_RESTORE] = {
		.epstr	= "MTD_API_EP_TEST_SATS_CHKPT_RESTORE",
		.tmpl	= "/individuals/self-assessment-test-support/vendor-state/checkpoints/{checkpointId}/restore",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_NONE,
		.api	= MTD_EP_API_TEST_SATS,
	},
	/* Self Assessment Test Support Business Income Source */
	[MTD_API_EP_TEST_SATS_BIS_CREATE] = {
		.epstr	= "MTD_API_EP_TEST_SATS_BIS_CREATE",
		.tmpl	= "https://test-api.service.hmrc.gov.uk/individuals/self-assessment-test-support/business/{nino}",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= MTD_EP_API_TEST_SATS,
	},
	[MTD_API_EP_TEST_SATS_BIS_DELETE] = {
		.epstr	= "MTD_API_EP_TEST_SATS_BIS_DELETE",
		.tmpl	= "/individuals/self-assessment-test-support/business/{nino}/{businessId}",
		.method	= M_DELETE,
		.api	= MTD_EP_API_TEST_SATS,
	},
	/* Self Assessment Test Support ITSA Status */
	[MTD_API_EP_TEST_SATS_IS_AMEND] = {
		.epstr	= "MTD_API_EP_TEST_SATS_IS_AMEND",
		.tmpl	= "/individuals/self-assessment-test-support/itsa-status/{nino}/{taxYear}",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_JSON,
		.api	= MTD_EP_API_TEST_SATS,
	},

	/* OAuth */
	[MTD_API_EP_OA_REFRESH_TOKEN] = {
		.epstr	= "MTD_API_EP_OA_REFRESH_TOKEN",
		.tmpl	= "/oauth/token",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_URL_ENCODED,
		.api	= MTD_EP_API_NULL,
		.authz	= AUTHZ_NONE,
		.scope	= MTD_API_SCOPE_NULL
	},
	[MTD_API_EP_OA_EXCHANGE_AUTH_CODE] = {
		.epstr	= "MTD_API_EP_OA_EXCHANGE_AUTH_CODE",
		.tmpl	= "/oauth/token",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_URL_ENCODED,
		.api	= MTD_EP_API_NULL,
		.authz	= AUTHZ_NONE,
		.scope	= MTD_API_SCOPE_NULL
	},
	[MTD_API_EP_OA_APPLICATION_TOKEN] = {
		.epstr	= "MTD_API_EP_OA_APPLICATION_TOKEN",
		.tmpl	= "/oauth/token",
		.method	= M_POST,
		.ctype	= CONTENT_TYPE_URL_ENCODED,
		.api	= MTD_EP_API_NULL,
		.authz	= AUTHZ_NONE,
		.scope	= MTD_API_SCOPE_NULL
	},
};

#endif /* _API_ENDPOINTS_H_ */
