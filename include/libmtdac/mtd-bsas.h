/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * mtd-bsas.h - Make Tax Digital - Business Source Adjustable Summary API
 *
 * Copyright (C) 2020		Andrew Clayton <andrew@digital-domain.net>
 */

#ifndef _MTD_BSAS_H_
#define _MTD_BSAS_H_

#include "mtd.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma GCC visibility push(default)

extern int mtd_bsas_pb_update_summary_adjustments(const struct mtd_dsrc_ctx *dsctx,
						  const char *bid, char **buf);
extern int mtd_bsas_pb_list_summary_adjustments(const char *bid, char **buf);
extern int mtd_bsas_pb_get_summary(const char *bid, const char *query_string,
				   char **buf);
extern int mtd_bsas_se_update_summary_adjustments(const struct mtd_dsrc_ctx *dsctx,
						  const char *bid, char **buf);
extern int mtd_bsas_se_list_summary_adjustments(const char *bid, char **buf);
extern int mtd_bsas_se_get_summary(const char *bid, const char *query_string,
				   char **buf);
extern int mtd_bsas_trigger_summary(const struct mtd_dsrc_ctx *dsctx,
				    char **buf);
extern int mtd_bsas_list_summaries(const char *query_string, char **buf);

#pragma GCC visibility pop

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _MTD_BSAS_H_ */
