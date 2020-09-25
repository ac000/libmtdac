/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * mtd-vat.h - Make Tax Digital - VAT API
 *
 * Copyright (C) 2020		Andrew Clayton <andrew@digital-domain.net>
 */

#ifndef _MTD_VAT_H_
#define _MTD_VAT_H_

#include "mtd.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma GCC visibility push(default)

extern int mtd_vat_list_payments(const char *vrn, const char *query_string,
				 char **buf);
extern int mtd_vat_list_liabilities(const char *vrn, const char *query_string,
				    char **buf);
extern int mtd_vat_get_period(const char *vrn, const char *period_key,
			      char **buf);
extern int mtd_vat_submit_period(const struct mtd_dsrc_ctx *dsctx,
				 const char *vrn, char **buf);
extern int mtd_vat_list_obligations(const char *vrn, const char *query_string,
				    char **buf);

#pragma GCC visibility pop

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _MTD_VAT_H_ */
