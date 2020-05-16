/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * mtd-saac.h - Make Tax Digital - Self Assessment Accounts API
 *
 * Copyright (C) 2020		Andrew Clayton <andrew@digital-domain.net>
 */

#ifndef _MTD_SAAC_H_
#define _MTD_SAAC_H_

#include "mtd.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma GCC visibility push(default)

extern int mtd_saac_get_payment(const char *pyid, char **buf);
extern int mtd_saac_list_payments(const char *from, const char *to,
				  char **buf);
extern int mtd_saac_get_charge(const char *trid, char **buf);
extern int mtd_saac_list_charges(const char *from, const char *to, char **buf);
extern int mtd_saac_get_transaction(const char *trid, char **buf);
extern int mtd_saac_list_transactions(const char *from, const char *to,
				      char **buf);
extern int mtd_saac_get_balance(char **buf);

#pragma GCC visibility pop

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _MTD_SAAC_H_ */
