/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * mtd-ep-ibeops.c - Make Tax Digital - Individuals Business End of Period Statement API
 *
 * Copyright (C) 2021 - 2022	Andrew Clayton <andrew@digital-domain.net>
 */

#include <stddef.h>

#include "mtd-ibeops.h"		/* for default (public) visibility */
#include "endpoints.h"

#define VERSION		"2.0"
#define API_VER		"Accept: application/vnd.hmrc." VERSION "+json"

/*
 * [POST]
 * /individuals/business/end-of-period-statement/{nino}
 */
int mtd_ibeops_submit_eops(const struct mtd_dsrc_ctx *dsctx, char **buf)
{
	return do_ep(IBEOPS_SUBMIT_END_OF_PERIOD_STATEMENT, API_VER,
		     dsctx, buf, (char *)NULL);
}
