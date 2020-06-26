/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * mtd-ic.h - Make Tax Digital - Individual Calculations API
 *
 * Copyright (C) 2020		Andrew Clayton <andrew@digital-domain.net>
 */

#ifndef _MTD_IC_H_
#define _MTD_IC_H_

#include "mtd.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma GCC visibility push(default)

extern int mtd_ic_get_messages(const char *cid, char **buf);
extern int mtd_ic_get_end_of_year_est(const char *cid, char **buf);
extern int mtd_ic_get_allowances_deductions_reliefs(const char *cid,
						    char **buf);
extern int mtd_ic_get_taxable_income(const char *cid, char **buf);
extern int mtd_ic_get_income_tax_nics_calc(const char *cid, char **buf);
extern int mtd_ic_get_calculation_meta(const char *cid, char **buf);
extern int mtd_ic_trigger_calculation(const struct mtd_dsrc_ctx *dsctx,
				      char **buf);
extern int mtd_ic_list_calculations(const char *query_string, char **buf);

#pragma GCC visibility pop

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _MTD_IC_H_ */
