/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * mtd-il.h - Make Tax Digital - Individual Loses API
 *
 * Copyright (C) 2020		Andrew Clayton <andrew@digital-domain.net>
 */

#ifndef _MTD_IL_H_
#define _MTD_IL_H_

#include "mtd.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma GCC visibility push(default)

extern int mtd_il_lc_update_loss(const char *src_file, char **buf);
extern int mtd_il_lc_update_loss_type(const char *src_file, const char *cid,
				      char **buf);
extern int mtd_il_lc_delete_loss(const char *cid, char **buf);
extern int mtd_il_lc_get_loss(const char *cid, char **buf);
extern int mtd_il_lc_create_loss(const char *src_file, char **buf);
extern int mtd_il_lc_list_loses(const char *query_stirng, char **buf);
extern int mtd_il_bf_update_loss_amnt(const char *src_file, const char *lid,
				      char **buf);
extern int mtd_il_bf_delete_loss(const char *lid, char **buf);
extern int mtd_il_bf_get_loss(const char *lid, char **buf);
extern int mtd_il_bf_create_loss(const char *src_file, char **buf);
extern int mtd_il_bf_list_loses(const char *query_string, char **buf);

#pragma GCC visibility pop

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _MTD_IL_H_ */
