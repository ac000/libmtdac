/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * mtd-isi.h - Make Tax Digital - Individuals Savings Income API
 *
 * Copyright (C) 2025		Andrew Clayton <ac@sigsegv.uk>
 */

#ifndef _MTD_ISI_H_
#define _MTD_ISI_H_

#include "mtd.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma GCC visibility push(default)

extern int mtd_isi_si_ua_list(const char *query_string, char **buf);
extern int mtd_isi_si_ua_add(const struct mtd_dsrc_ctx *dsctx, char **buf);
extern int mtd_isi_si_ua_get_annual_summary(const char *tax_year,
					    const char *said, char **buf);
extern int mtd_isi_si_ua_update_annual_summary(const struct mtd_dsrc_ctx *dsctx,
					       const char *tax_year,
					       const char *said, char **buf);
extern int mtd_isi_si_o_get(const char *tax_year, char **buf);
extern int mtd_isi_si_o_update(const struct mtd_dsrc_ctx *dsctx,
			       const char *tax_year, char **buf);
extern int mtd_isi_si_o_delete(const char *tax_year, char **buf);

#pragma GCC visibility pop

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _MTD_ISI_H_ */
