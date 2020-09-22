/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * mtd-ie.h - Make Tax Digital - Individual Employment API
 *
 * Copyright (C) 2020		Andrew Clayton <andrew@digital-domain.net>
 */

#ifndef _MTD_IE_H_
#define _MTD_IE_H_

#include "mtd.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma GCC visibility push(default)

extern int mtd_ie_get_annual_summary(const char *utr, const char *tax_year,
				     char **buf);

#pragma GCC visibility pop

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _MTD_IE_H_ */
