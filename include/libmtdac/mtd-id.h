/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * mtd-id.h - Make Tax Digital - Individuals Disclosures API
 *
 * Copyright (C) 2021		Andrew Clayton <andrew@digital-domain.net>
 */

#ifndef _MTD_ID_H_
#define _MTD_ID_H_

#include "mtd.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma GCC visibility push(default)

extern int mtd_id_ma_create(const struct mtd_dsrc_ctx *dsctx, char **buf);

extern int mtd_id_delete(const char *cid, char **buf);
extern int mtd_id_set(const struct mtd_dsrc_ctx *dsctx, const char *tax_year,
		      char **buf);
extern int mtd_id_get(const char *cid, char **buf);

#pragma GCC visibility pop

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _MTD_ID_H_ */
