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

extern void set_anti_fraud_hdrs(const struct mtd_ctx *mtd_ctx,
				struct curl_ctx *ctx);
extern void fph_set_ops(enum app_conn_type conn_type,
			const struct mtd_fph_ops *ops);

#endif /* _FPH_H_ */
