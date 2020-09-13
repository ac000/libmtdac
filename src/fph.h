/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * fph.h - Make Tax Digital - Fraud Prevention Headers
 *
 * Copyright (C) 2020		Andrew Clayton <andrew@digital-domain.net>
 */

#ifndef _FPH_H_
#define _FPH_H_

#include "mtd.h"
#include "curler.h"

extern void set_anti_fraud_hdrs(struct curl_ctx *ctx);
extern void fph_set_ops(const struct mtd_fph_ops *ops);
extern void fph_init_ops(void);

#endif /* _FPH_H_ */
