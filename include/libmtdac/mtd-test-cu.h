/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * mtd-test-cu.h - Make Tax Digital - Create Test User API
 *
 * Copyright (C) 2020		Andrew Clayton <andrew@digital-domain.net>
 */

#ifndef _MTD_TEST_CU_H_
#define _MTD_TEST_CU_H_

#include "mtd.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma GCC visibility push(default)

extern int mtd_test_cu_list_services(char **buf);
extern int mtd_test_cu_create_agent(const struct mtd_dsrc_ctx *dsctx,
				    char **buf);
extern int mtd_test_cu_create_organisation(const struct mtd_dsrc_ctx *dsctx,
					   char **buf);
extern int mtd_test_cu_create_individual(const struct mtd_dsrc_ctx *dsctx,
					 char **buf);

#pragma GCC visibility pop

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _MTD_TEST_CU_H_ */
