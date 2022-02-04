/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * mtd-ep-id.c - Make Tax Digital - Individuals Disclosures API
 *
 * Copyright (C) 2021		Andrew Clayton <andrew@digital-domain.net>
 */

#include <stddef.h>

#include "mtd-id.h"		/* for default (public) visibility */
#include "endpoints.h"

#define VERSION		"1.0"
#define API_VER		"Accept: application/vnd.hmrc." VERSION "+json"

/*
 * [POST]
 * /individuals/disclosures/marriage-allowance/{nino}
 */
int mtd_id_ma_create(const struct mtd_dsrc_ctx *dsctx, char **buf)
{
	return do_ep(ID_MA_CREATE, API_VER,
		     dsctx, buf, (char *)NULL);
}

/*
 * [DELETE]
 * /individuals/disclosures/{nino}/{taxYear}
 */
int mtd_id_delete(const char *tax_year, char **buf)
{
	return do_ep(ID_DELETE, API_VER,
		     NULL, buf, tax_year, (char *)NULL);
}

/*
 * [PUT ]
 * /individuals/disclosures/{nino}/{taxYear}
 */
int mtd_id_set(const struct mtd_dsrc_ctx *dsctx, const char *tax_year,
	       char **buf)
{
	return do_ep(ID_SET, API_VER,
		     dsctx, buf, tax_year, (char *)NULL);
}

/*
 * [GET ]
 * /individuals/disclosures/{nino}/{taxYear}
 */
int mtd_id_get(const char *tax_year, char **buf)
{
	return do_ep(ID_GET, API_VER,
		     NULL, buf, tax_year, (char *)NULL);
}
