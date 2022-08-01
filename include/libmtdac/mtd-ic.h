/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * mtd-ic.h - Make Tax Digital - Individual Calculations API
 *
 * Copyright (C) 2020 - 2022	Andrew Clayton <andrew@digital-domain.net>
 */

#ifndef _MTD_IC_H_
#define _MTD_IC_H_

#include "mtd.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma GCC visibility push(default)

extern int mtd_ic_final_decl(const char *tax_year, const char *cid,
			     char **buf);
extern int mtd_ic_get_calculation(const char *tax_year, const char *cid,
				  char **buf);
extern int mtd_ic_trigger_calculation(const char *tax_year,
				      const char *query_string, char **buf);
extern int mtd_ic_list_calculations(const char *query_string, char **buf);

#pragma GCC visibility pop

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _MTD_IC_H_ */
