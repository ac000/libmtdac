/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * mtd-bd.h - Make Tax Digital - Business Details API
 *
 * Copyright (c) 2021		Andrew Clayton <andrew@digital-domain.net>
 */

#ifndef _MTD_BD_H_
#define _MTD_BD_H_

#include "mtd.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma GCC visibility push(default)

extern int mtd_bd_get(const char *bid, char **buf);
extern int mtd_bd_list(char **buf);

#pragma GCC visibility pop

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _MTD_BD_H_ */
