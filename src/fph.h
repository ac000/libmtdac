/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * fph.h - Make Tax Digital - Fraud Prevention Headers
 *
 * Copyright (C) 2020		Andrew Clayton <andrew@digital-domain.net>
 */

#ifndef _FPH_H_
#define _FPH_H_

#include "curler.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void set_anti_fraud_hdrs(struct curl_ctx *ctx);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _FPH_H_ */
