/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * mtd-sa.c - Make Tax Digital - Self Assessment API
 *
 * Copyright (C) 2020		Andrew Clayton <andrew@digital-domain.net>
 */

#include <stddef.h>

#include "mtd-sa.h"		/* for default (public) visibility */
#include "endpoints.h"

#define API_VER			"Accept: application/vnd.hmrc.2.0+json"

/*
 * [GET ]
 * /self-assessment/ni/{nino}/crystallisation/obligations?from=YYYY-MM-DD&to=YYYY-MM-DD
 */
int mtd_sa_cr_list_obligations(const char *query_string, char **buf)
{
	return do_ep(SA_CR_LIST_OBLIGATIONS, API_VER,
		     NULL, NULL, buf, query_string, (char *)NULL);
}

/*
 * [POST]
 * /self-assessment/ni/{nino}/{taxYear}/crystallisation
 */
int mtd_sa_cr_crystallise(const char *tax_year, char **buf)
{
	return do_ep(SA_CR_CRYSTALLISE, API_VER,
		     NULL, NULL, buf, tax_year, (char *)NULL);
}

/*
 * [POST]
 * /self-assessment/ni/{nino}/{taxYear}/intent-to-crystallise
 */
int mtd_sa_cr_intent_to_crystallise(const char *tax_year, char **buf)
{
	return do_ep(SA_CR_INTENT_TO_CRYSTALLISE, API_VER,
		     NULL, NULL, buf, tax_year, (char *)NULL);
}

/*
 * [GET ]
 * /self-assessment/ni/{nino}/calculations/{calculationId}/validation-messages
 */
int mtd_sa_tc_get_validation_msgs(const char *cid, char **buf)
{
	return do_ep(SA_TC_GET_VALIDATION_MSGS, API_VER,
		     NULL, NULL, buf, cid, (char *)NULL);
}

/*
 * [GET ]
 * /self-assessment/ni/{nino}/calculations/{calculationId}
 */
int mtd_sa_tc_get_calculation(const char *cid, char **buf)
{
	return do_ep(SA_TC_GET_CALCULATION, API_VER,
		     NULL, NULL, buf, cid, (char *)NULL);
}

/*
 * [POST]
 * /self-assessment/ni/{nino}/calculations
 */
int mtd_sa_tc_calculate(const char *src_file, char **buf)
{
	return do_ep(SA_TC_CALCULATE, API_VER,
		     src_file, NULL, buf, (char *)NULL);
}

/*
 * [PUT ]
 * /self-assessment/ni/{nino}/charitable-giving/{taxYear}
 */
int mtd_sa_cg_update_charitable_giving(const char *src_file,
				       const char *tax_year, char **buf)
{
	return do_ep(SA_CG_UPDATE_CHARITABLE_GIVING, API_VER,
		     src_file, NULL, buf, tax_year, (char *)NULL);
}

/*
 * [GET ]
 * /self-assessment/ni/{nino}/charitable-giving/{taxYear}
 */
int mtd_sa_cg_get_charitable_giving(const char *tax_year, char **buf)
{
	return do_ep(SA_CG_GET_CHARITABLE_GIVING, API_VER,
		     NULL, NULL, buf, tax_year, (char *)NULL);
}

/*
 * [PUT ]
 * /self-assessment/ni/{nino}/savings-accounts/{savingsAccountId}/{taxYear}
 */
int mtd_sa_sa_update_annual_summary(const char *src_file, const char *said,
				    const char *tax_year, char **buf)
{
	return do_ep(SA_SA_UPDATE_ANNUAL_SUMMARY, API_VER,
		     src_file, NULL, buf, said, tax_year, (char *)NULL);
}

/*
 * [GET ]
 * /self-assessment/ni/{nino}/savings-accounts/{savingsAccountId}/{taxYear}
 */
int mtd_sa_sa_get_annual_summary(const char *said, const char *tax_year,
				 char **buf)
{
	return do_ep(SA_SA_GET_ANNUAL_SUMMARY, API_VER,
		     NULL, NULL, buf, said, tax_year, (char *)NULL);
}

/*
 * [GET ]
 * /self-assessment/ni/{nino}/savings-accounts/{savingsAccountId}
 */
int mtd_sa_sa_get_account(const char *said, char **buf)
{
	return do_ep(SA_SA_GET_ACCOUNT, API_VER,
		     NULL, NULL, buf, said, (char *)NULL);
}

/*
 * [POST]
 * /self-assessment/ni/{nino}/savings-accounts
 */
int mtd_sa_sa_create_account(const char *src_file, char **buf)
{
	return do_ep(SA_SA_CREATE_ACCOUNT, API_VER,
		     src_file, NULL, buf, (char *)NULL);
}

/*
 * [GET ]
 * /self-assessment/ni/{nino}/savings-accounts
 */
int mtd_sa_sa_list_accounts(char **buf)
{
	return do_ep(SA_SA_LIST_ACCOUNTS, API_VER,
		     NULL, NULL, buf, (char *)NULL);
}

/*
 * [PUT ]
 * /self-assessment/ni/{nino}/dividends/{taxYear}
 */
int mtd_sa_di_update_annual_summary(const char *src_file, const char *tax_year,
				    char **buf)
{
	return do_ep(SA_DI_UPDATE_ANNUAL_SUMMARY, API_VER,
		     src_file, NULL, buf, tax_year, (char *)NULL);
}

/*
 * [GET ]
 * /self-assessment/ni/{nino}/dividends/{taxYear}
 */
int mtd_sa_di_get_annual_summary(const char *tax_year, char **buf)
{
	return do_ep(SA_DI_GET_ANNUAL_SUMMARY, API_VER,
		     NULL, NULL, buf, tax_year, (char *)NULL);
}

/*
 * [GET ]
 * /self-assessment/ni/{nino}/uk-properties/end-of-period-statements/obligations?from=YYYY-MM-DD&to=YYYY-MM-DD
 */
int mtd_sa_pb_get_end_of_period_statement(const char *query_string, char **buf)
{
	return do_ep(SA_PB_GET_END_OF_PERIOD_STATEMENT, API_VER,
		     NULL, NULL, buf, query_string, (char *)NULL);
}

/*
 * [POST]
 * /self-assessment/ni/{nino}/uk-properties/end-of-period-statements/from/{start}/to/{end}
 */
int mtd_sa_pb_submit_end_of_period_statement(const char *src_file,
					     const char *start,
					     const char *end, char **buf)
{
	return do_ep(SA_PB_SUBMIT_END_OF_PERIOD_STATEMENT, API_VER,
		     src_file, NULL, buf, start, end, (char *)NULL);
}

/*
 * [PUT ]
 * /self-assessment/ni/{nino}/uk-properties/furnished-holiday-lettings/{taxYear}
 */
int mtd_sa_pb_update_fhl_annual_summary(const char *src_file,
					const char *tax_year, char **buf)
{
	return do_ep(SA_PB_UPDATE_FHL_ANNUAL_SUMMARY, API_VER,
		     src_file, NULL, buf, tax_year, (char *)NULL);
}

/*
 * [GET ]
 * /self-assessment/ni/{nino}/uk-properties/furnished-holiday-lettings/{taxYear}
 */
int mtd_sa_pb_get_fhl_annual_summary(const char *tax_year, char **buf)
{
	return do_ep(SA_PB_GET_FHL_ANNUAL_SUMMARY, API_VER,
		     NULL, NULL, buf, tax_year, (char *)NULL);
}

/*
 * [PUT ]
 * /self-assessment/ni/{nino}/uk-properties/furnished-holiday-lettings/periods/{periodId}
 */
int mtd_sa_pb_update_fhl_period(const char *src_file, const char *pid,
				char **buf)
{
	return do_ep(SA_PB_UPDATE_FHL_PERIOD, API_VER,
		     src_file, NULL, buf, pid, (char *)NULL);
}

/*
 * [GET ]
 * /self-assessment/ni/{nino}/uk-properties/furnished-holiday-lettings/periods/{periodId}
 */
int mtd_sa_pb_get_fhl_period(const char *pid, char **buf)
{
	return do_ep(SA_PB_GET_FHL_PERIOD, API_VER,
		     NULL, NULL, buf, pid, (char *)NULL);
}

/*
 * [POST]
 * /self-assessment/ni/{nino}/uk-properties/furnished-holiday-lettings/periods
 */
int mtd_sa_pb_create_fhl_period(const char *src_file, char **buf)
{
	return do_ep(SA_PB_CREATE_FHL_PERIOD, API_VER,
		     src_file, NULL, buf, (char *)NULL);
}

/*
 * [GET ]
 * /self-assessment/ni/{nino}/uk-properties/furnished-holiday-lettings/periods
 */
int mtd_sa_pb_list_fhl_periods(char **buf)
{
	return do_ep(SA_PB_LIST_FHL_PERIODS, API_VER,
		     NULL, NULL, buf, (char *)NULL);
}

/*
 * [PUT ]
 * /self-assessment/ni/{nino}/uk-properties/other/{taxYear}
 */
int mtd_sa_pb_update_non_fhl_annual_summary(const char *src_file,
					    const char *tax_year, char **buf)
{
	return do_ep(SA_PB_UPDATE_NON_FHL_ANNUAL_SUMMARY, API_VER,
		     src_file, NULL, buf, tax_year, (char *)NULL);
}

/*
 * [GET ]
 * /self-assessment/ni/{nino}/uk-properties/other/{taxYear}
 */
int mtd_sa_pb_get_non_fhl_annual_summary(const char *tax_year, char **buf)
{
	return do_ep(SA_PB_GET_NON_FHL_ANNUAL_SUMMARY, API_VER,
		     NULL, NULL, buf, tax_year, (char *)NULL);
}

/*
 * [PUT ]
 * /self-assessment/ni/{nino}/uk-properties/other/periods/{periodId}
 */
int mtd_sa_pb_update_non_fhl_period(const char *src_file,
				    const char *pid, char **buf)
{
	return do_ep(SA_PB_UPDATE_NON_FHL_PERIOD, API_VER,
		     src_file, NULL, buf, pid, (char *)NULL);
}

/*
 * [GET ]
 * /self-assessment/ni/{nino}/uk-properties/other/periods/{periodId}
 */
int mtd_sa_pb_get_non_fhl_period(const char *pid, char **buf)
{
	return do_ep(SA_PB_GET_NON_FHL_PERIOD, API_VER,
		     NULL, NULL, buf, pid, (char *)NULL);
}

/*
 * [POST]
 * /self-assessment/ni/{nino}/uk-properties/other/periods
 */
int mtd_sa_pb_create_non_fhl_period(const char *src_file, char **buf)
{
	return do_ep(SA_PB_CREATE_NON_FHL_PERIOD, API_VER,
		     src_file, NULL, buf, (char *)NULL);
}

/*
 * [GET ]
 * /self-assessment/ni/{nino}/uk-properties/other/periods
 */
int mtd_sa_pb_list_non_fhl_periods(char **buf)
{
	return do_ep(SA_PB_LIST_NON_FHL_PERIODS, API_VER,
		     NULL, NULL, buf, (char *)NULL);
}

/*
 * [GET ]
 * /self-assessment/ni/{nino}/uk-properties/obligations
 */
int mtd_sa_pb_list_obligations(char **buf)
{
	return do_ep(SA_PB_LIST_OBLIGATIONS, API_VER,
		     NULL, NULL, buf, (char *)NULL);
}

/*
 * [POST]
 * /self-assessment/ni/{nino}/uk-properties
 */
int mtd_sa_pb_create_property(const char *src_file, char **buf)
{
	return do_ep(SA_PB_CREATE_PROPERTY, API_VER,
		     src_file, NULL, buf, (char *)NULL);
}

/*
 * [GET ]
 * /self-assessment/ni/{nino}/uk-properties
 */
int mtd_sa_pb_get_property(char **buf)
{
	return do_ep(SA_PB_GET_PROPERTY, API_VER,
		     NULL, NULL, buf, (char *)NULL);
}

/*
 * [GET ]
 * /self-assessment/ni/{nino}/self-employments/{selfEmploymentId}/end-of-period-statements/obligations
 *
 * Optional query string:
 *
 * 	?from=YYYY-MM-DD&to=YYYY-MM-DD
 */
int mtd_sa_se_get_end_of_period_statement(const char *seid,
					  const char *query_string, char **buf)
{
	return do_ep(SA_SE_GET_END_OF_PERIOD_STATEMENT, API_VER,
		     NULL, NULL, buf, seid, query_string, (char *)NULL);
}

/*
 * [POST]
 * /self-assessment/ni/{nino}/self-employments/{selfEmploymentId}/end-of-period-statements/from/{start}/to/{end}
 */
int mtd_sa_se_submit_end_of_period_statement(const char *src_file,
					     const char *seid,
					     const char *start,
					     const char *end, char **buf)
{
	return do_ep(SA_SE_SUBMIT_END_OF_PERIOD_STATEMENT, API_VER,
		     src_file, NULL, buf, seid, start, end, (char *)NULL);
}

/*
 * [PUT ]
 * /self-assessment/ni/{nino}/self-employments/{selfEmploymentId}/{taxYear}
 */
int mtd_sa_se_update_annual_summary(const char *src_file, const char *seid,
				    const char *tax_year, char **buf)
{
	return do_ep(SA_SE_UPDATE_ANNUAL_SUMMARY, API_VER,
		     src_file, NULL, buf, seid, tax_year, (char *)NULL);
}

/*
 * [GET ]
 * /self-assessment/ni/{nino}/self-employments/{selfEmploymentId}/{taxYear}
 */
int mtd_sa_se_get_annual_summary(const char *seid, const char *tax_year,
				 char **buf)
{
	return do_ep(SA_SE_GET_ANNUAL_SUMMARY, API_VER,
		     NULL, NULL, buf, seid, tax_year, (char *)NULL);

}

/*
 * [PUT ]
 * /self-assessment/ni/{nino}/self-employments/{selfEmploymentId}/periods/{periodId}
 */
int mtd_sa_se_update_period(const char *src_file, const char *seid,
			    const char *period_id, char **buf)
{
	return do_ep(SA_SE_UPDATE_PERIOD, API_VER,
		     src_file, NULL, buf, seid, period_id, (char *)NULL);
}

/*
 * [GET ]
 * /self-assessment/ni/{nino}/self-employments/{selfEmploymentId}/periods/{periodId}
 */
int mtd_sa_se_get_period(const char *seid, const char *period_id, char **buf)
{
	return do_ep(SA_SE_GET_PERIOD, API_VER,
		     NULL, NULL, buf, seid, period_id, (char *)NULL);
}

/*
 * [POST]
 * /self-assessment/ni/{nino}/self-employments/{selfEmploymentId}/periods
 */
int mtd_sa_se_create_period(const char *src_file, const char *seid, char **buf)
{
	return do_ep(SA_SE_CREATE_PERIOD, API_VER,
		     src_file, NULL, buf, seid, (char *)NULL);
}

/*
 * [GET ]
 * /self-assessment/ni/{nino}/self-employments/{selfEmploymentId}/periods
 */
int mtd_sa_se_list_periods(const char *seid, char **buf)
{
	return do_ep(SA_SE_LIST_PERIODS, API_VER,
		     NULL, NULL, buf, seid, (char *)NULL);
}

/*
 * [GET ]
 * /self-assessment/ni/{nino}/self-employments/{selfEmploymentId}/obligations
 */
int mtd_sa_se_list_obligations(const char *seid, char **buf)
{
	return do_ep(SA_SE_LIST_OBLIGATIONS, API_VER,
		     NULL, NULL, buf, seid, (char *)NULL);
}

/*
 * [GET ]
 * /self-assessment/ni/{nino}/self-employments/{selfEmploymentId}
 */
int mtd_sa_se_get_employment(const char *seid, char **buf)
{
	return do_ep(SA_SE_GET_SELF_EMPLOYMENT, API_VER,
		     NULL, NULL, buf, seid, (char *)NULL);
}

/*
 * [POST]
 * /self-assessment/ni/{nino}/self-employments
 */
int mtd_sa_se_create_employment(const char *src_file, char **buf)
{
	return do_ep(SA_SE_CREATE_SELF_EMPLOYMENT, API_VER,
		     src_file, NULL, buf, (char *)NULL);
}

/*
 * [GET ]
 * /self-assessment/ni/{nino}/self-employments
 */
int mtd_sa_se_list_employments(char **buf)
{
	return do_ep(SA_SE_LIST_SELF_EMPLOYMENTS, API_VER,
		     NULL, NULL, buf, (char *)NULL);
}
