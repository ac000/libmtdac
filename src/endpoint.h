/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * endpoint.h - Make Tax Digital - Self Assessment API
 *
 * Copyright (C) 2020 - 2025	Andrew Clayton <ac@sigsegv.uk>
 */

#ifndef _ENDPOINT_H_
#define _ENDPOINT_H_

#include "mtd.h"

#define URL_LEN			1024

static const struct {
	const char *name;
	const char *fname;
} api_scope_map[] = {
	[MTD_API_SCOPE_UNSET]	= {},
	[MTD_API_SCOPE_SA]	= { "itsa", "Income Tax Self-Assessment" },
	[MTD_API_SCOPE_VAT]	= { "vat", "VAT"			 },
};

extern int (*ep_set_oauther(enum mtd_api_endpoint ep))(enum mtd_api_scope);
extern char *ep_get_token(enum mtd_api_endpoint ep);
extern char *ep_make_url(enum mtd_api_endpoint ep, const char * const params[],
			 char *url);

#endif /* _ENDPOINT_H_ */
