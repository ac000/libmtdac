/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * mtd-ibeops.h - Make Tax Digital - Individuals Business End of Period Statement API
 *
 * Copyright (C) 2021		Andrew Clayton <andrew@digital-domain.net>
 */

#ifndef _MTD_IBEOPS_H_
#define _MTD_IBEOPS_H_

#include "mtd.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma GCC visibility push(default)

extern int mtd_ibeops_submit_eops(const struct mtd_dsrc_ctx *dsctx, char **buf);

#pragma GCC visibility pop

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _MTD_IBEOPS_H_ */
