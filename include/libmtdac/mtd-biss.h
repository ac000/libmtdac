/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * mtd-biss.h - Make Tax Digital - Business Income Source Summary API
 *
 * Copyright (C) 2020 - 2022	Andrew Clayton <andrew@digital-domain.net>
 */

#ifndef _MTD_BISS_H_
#define _MTD_BISS_H_

#include "mtd.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma GCC visibility push(default)

extern int mtd_biss_get_summary(const char *business_type,
				const char *tax_year, const char *bid,
				char **buf);

#pragma GCC visibility pop

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _MTD_BISS_H_ */
