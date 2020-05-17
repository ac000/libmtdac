/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * mtd-ni.h - Make Tax Digital - National Insurnace API
 *
 * Copyright (C) 2020		Andrew Clayton <andrew@digital-domain.net>
 */

#ifndef _MTD_NI_H_
#define _MTD_NI_H_

#include "mtd.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma GCC visibility push(default)

extern int mtd_ni_get_annual_summary(const char *utr, const char *tax_year,
				     char **buf);

#pragma GCC visibility pop

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _MTD_NI_H_ */
