/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * mtd-ob.h - Make Tax Digital - Obligations API
 *
 * Copyright (c) 2021		Andrew Clayton <andrew@digital-domain.net>
 */

#ifndef _MTD_OB_H_
#define _MTD_OB_H_

#include "mtd.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma GCC visibility push(default)

extern int mtd_ob_list_end_of_period_obligations(const char *query_string,
						 char **buf);
extern int mtd_ob_list_crystallisation_obligations(const char *query_string,
						   char **buf);
extern int mtd_ob_list_inc_and_expend_obligations(const char *query_string,
						  char **buf);

#pragma GCC visibility pop

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _MTD_OB_H_ */
