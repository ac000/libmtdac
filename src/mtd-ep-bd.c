/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * mtd-ep-bd.c - Make Tax Digital - Business Details API
 *
 * Copyright (C) 2021		Andrew Clayton <andrew@digital-domain.net>
 */

#include <stddef.h>

#include "mtd-bd.h"		/* for default (public) visibility */
#include "endpoints.h"

#define VERSION		"1.0"
#define API_VER		"Accept: application/vnd.hmrc." VERSION "+json"

/*
 * [GET ]
 * /individuals/business/details/{nino}/{businessId}
 */
int mtd_bd_get(const char *bid, char **buf)
{
	return do_ep(BD_GET, API_VER, NULL, buf, bid, (char *)NULL);
}

/*
 * [GET ]
 * /individuals/business/details/{nino}/list
 */
int mtd_bd_list(char **buf)
{
	return do_ep(BD_LIST, API_VER, NULL, buf, (char *)NULL);
}
