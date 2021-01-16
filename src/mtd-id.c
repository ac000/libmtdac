/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * mtd-id.c - Make Tax Digital - Individuals Disclosures API
 *
 * Copyright (C) 2021		Andrew Clayton <andrew@digital-domain.net>
 */

#include <stddef.h>

#include "mtd-id.h"		/* for default (public) visibility */
#include "endpoints.h"

#define API_VER			"Accept: application/vnd.hmrc.1.0+json"

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
