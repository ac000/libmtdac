/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * mtd-biss.h - Make Tax Digital - Business Income Source Summary API
 *
 * Copyright (C) 2020 - 2021	Andrew Clayton <andrew@digital-domain.net>
 */

#ifndef _MTD_BISS_H_
#define _MTD_BISS_H_

#include "mtd.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma GCC visibility push(default)

extern int mtd_biss_get_foreign_property(const char *query_string, char **buf);
extern int mtd_biss_get_uk_property(const char *query_string, char **buf);
extern int mtd_biss_get_self_employment(const char *query_string, char **buf);

#pragma GCC visibility pop

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _MTD_BISS_H_ */
