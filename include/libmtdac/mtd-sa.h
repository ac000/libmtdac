/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * mtd-sa.h - Make Tax Digital - Self Assessment API
 *
 * Copyright (c) 2020 - 2021	Andrew Clayton <andrew@digital-domain.net>
 */

#ifndef _MTD_SA_H_
#define _MTD_SA_H_

#include "mtd.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma GCC visibility push(default)

extern int mtd_sa_cr_list_obligations(const char *query_string, char **buf);
extern int mtd_sa_cr_crystallise(const struct mtd_dsrc_ctx *dsctx,
				 const char *tax_year, char **buf);
extern int mtd_sa_cr_intent_to_crystallise(const char *tax_year, char **buf);

extern int mtd_sa_cg_update_charitable_giving(const struct mtd_dsrc_ctx *dsctx,
					      const char *tax_year,
					      char **buf);
extern int mtd_sa_cg_get_charitable_giving(const char *tax_year, char **buf);

extern int mtd_sa_sa_update_annual_summary(const struct mtd_dsrc_ctx *dsctx,
					   const char *said,
					   const char *tax_year, char **buf);
extern int mtd_sa_sa_get_annual_summary(const char *said, const char *tax_year,
					char **buf);
extern int mtd_sa_sa_get_account(const char *said, char **buf);
extern int mtd_sa_sa_create_account(const struct mtd_dsrc_ctx *dsctx,
				    char **buf);
extern int mtd_sa_sa_list_accounts(char **buf);

extern int mtd_sa_di_update_annual_summary(const struct mtd_dsrc_ctx *dsctx,
					   const char *tax_year, char **buf);
extern int mtd_sa_di_get_annual_summary(const char *tax_year, char **buf);

extern int mtd_sa_pb_get_end_of_period_statement(const char *query_string,
						 char **buf);
extern int mtd_sa_pb_submit_end_of_period_statement(const struct mtd_dsrc_ctx *dsctx,
						    const char *start,
						    const char *end,
						    char **buf);
extern int mtd_sa_pb_update_fhl_annual_summary(const struct mtd_dsrc_ctx *dsctx,
					       const char *tax_year,
					       char **buf);
extern int mtd_sa_pb_get_fhl_annual_summary(const char *tax_year, char **buf);
extern int mtd_sa_pb_update_fhl_period(const struct mtd_dsrc_ctx *dsctx,
				       const char *pid, char **buf);
extern int mtd_sa_pb_get_fhl_period(const char *pid, char **buf);
extern int mtd_sa_pb_create_fhl_period(const struct mtd_dsrc_ctx *dsctx,
				       char **buf);
extern int mtd_sa_pb_list_fhl_periods(char **buf);
extern int mtd_sa_pb_update_non_fhl_annual_summary(const struct mtd_dsrc_ctx *dsctx,
						   const char *tax_year,
						   char **buf);
extern int mtd_sa_pb_get_non_fhl_annual_summary(const char *tax_year,
						char **buf);
extern int mtd_sa_pb_update_non_fhl_period(const struct mtd_dsrc_ctx *dsctx,
					   const char *pid, char **buf);
extern int mtd_sa_pb_get_non_fhl_period(const char *pid, char **buf);
extern int mtd_sa_pb_create_non_fhl_period(const struct mtd_dsrc_ctx *dsctx, char **buf);
extern int mtd_sa_pb_list_non_fhl_periods(char **buf);
extern int mtd_sa_pb_list_obligations(char **buf);
extern int mtd_sa_pb_create_property(const struct mtd_dsrc_ctx *dsctx,
				     char **buf);
extern int mtd_sa_pb_get_property(char **buf);

extern int mtd_sa_se_get_end_of_period_obligations(const char *seid,
						   const char *query_string,
						   char **buf);
extern int mtd_sa_se_submit_end_of_period_statement(const struct mtd_dsrc_ctx *dsctx,
						    const char *seid,
						    const char *start,
						    const char *end,
						    char **buf);
extern int mtd_sa_se_update_annual_summary(const struct mtd_dsrc_ctx *dsctx,
					   const char *seid,
					   const char *tax_year, char **buf);
extern int mtd_sa_se_get_annual_summary(const char *seid, const char *tax_year,
					char **buf);
extern int mtd_sa_se_update_period(const struct mtd_dsrc_ctx *dsctx,
				   const char *seid, const char *period_id,
				   char **buf);
extern int mtd_sa_se_get_period(const char *seid, const char *period_id,
				char **buf);
extern int mtd_sa_se_create_period(const struct mtd_dsrc_ctx *dsctx,
				   const char *seid, char **buf);
extern int mtd_sa_se_list_periods(const char *seid, char **buf);
extern int mtd_sa_se_list_obligations(const char *seid,
				      const char *query_string, char **buf);
extern int mtd_sa_se_get_employment(const char *seid, char **buf);
extern int mtd_sa_se_create_employment(const struct mtd_dsrc_ctx *dsctx,
				       char **buf);
extern int mtd_sa_se_list_employments(char **buf);

#pragma GCC visibility pop

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _MTD_SA_H_ */
