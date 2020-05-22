/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * mtd-sa.h - Make Tax Digital - Self Assessment API
 *
 * Copyright (C) 2020		Andrew Clayton <andrew@digital-domain.net>
 */

#ifndef _MTD_SA_H_
#define _MTD_SA_H_

#include "mtd.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma GCC visibility push(default)

extern int mtd_sa_sa_update_annual_summary(const char *src_file,
					   const char *said,
					   const char *tax_year, char **buf);
extern int mtd_sa_sa_get_annual_summary(const char *said, const char *tax_year,
					char **buf);
extern int mtd_sa_sa_get_account(const char *said, char **buf);
extern int mtd_sa_sa_create_account(const char *src_file, char **buf);
extern int mtd_sa_sa_list_accounts(char **buf);

extern int mtd_sa_se_get_end_of_period_statement(const char *seid,
						 const char *query_string,
						 char **buf);
extern int mtd_sa_se_submit_end_of_period_statement(const char *src_file,
						    const char *seid,
						    const char *start,
						    const char *end,
						    char **buf);
extern int mtd_sa_se_update_annual_summary(const char *src_file,
					   const char *seid,
					   const char *tax_year, char **buf);
extern int mtd_sa_se_get_annual_summary(const char *seid, const char *tax_year,
					char **buf);
extern int mtd_sa_se_update_period(const char *src_file, const char *seid,
				   const char *period_id, char **buf);
extern int mtd_sa_se_get_period(const char *seid, const char *period_id,
				char **buf);
extern int mtd_sa_se_create_period(const char *src_file, const char *seid,
				   char **buf);
extern int mtd_sa_se_list_periods(const char *seid, char **buf);
extern int mtd_sa_se_list_obligations(const char *seid, char **buf);
extern int mtd_sa_se_get_employment(const char *seid, char **buf);
extern int mtd_sa_se_list_employments(char **buf);

#pragma GCC visibility pop

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _MTD_SA_H_ */
