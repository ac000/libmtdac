/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * anti-fraud.h - Make Tax Digital
 *
 * Copyright (C) 2020		Andrew Clayton <andrew@digital-domain.net>
 */

#ifndef _ANTI_FRAUD_H_
#define _ANTI_FRAUD_H_

#include "mtd-curl.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void set_anti_fraud_hdrs(struct curl_ctx *ctx);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _ANTI_FRAUD_H_ */
