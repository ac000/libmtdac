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

extern int mtd_sa_get_end_of_period_statement(const char *start,
					      const char *end,
					      char **buf);
extern int mtd_sa_submit_end_of_period_statement(const char *src_file,
						 const char *start,
						 const char *end, char **buf);
extern int mtd_sa_update_annual_summary(const char *src_file,
					const char *tax_year, char **buf);
extern int mtd_sa_get_annual_summary(const char *tax_year, char **buf);
extern int mtd_sa_update_period(const char *src_file, const char *period_id,
				char **buf);
extern int mtd_sa_get_period(const char *period_id, char **buf);
extern int mtd_sa_create_period(const char *src_file, char **buf);
extern int mtd_sa_list_periods(char **buf);
extern int mtd_sa_list_obligations(char **buf);
extern int mtd_sa_get_employment(const char *employment_id, char **buf);
extern int mtd_sa_list_employments(char **buf);

#pragma GCC visibility pop

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _MTD_SA_H_ */
