/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * mtd-test-fph.h - Make Tax Digital - Test Fraud Prevention Headers API
 *
 * Copyright (C) 2020 - 2021	Andrew Clayton <andrew@digital-domain.net>
 */

#ifndef _MTD_TEST_FPH_H_
#define _MTD_TEST_FPH_H_

#include "mtd.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma GCC visibility push(default)

extern int mtd_test_fph_feedback(const char *api, const char *query_string,
				 char **buf);
extern int mtd_test_fph_validate(char **buf);

#pragma GCC visibility pop

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _MTD_TEST_FPH_H_ */
