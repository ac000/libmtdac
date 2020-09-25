/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * mtd-vat.c - Make Tax Digital - VAT API
 *
 * Copyright (C) 2020		Andrew Clayton <andrew@digital-domain.net>
 */

#include <stddef.h>

#include "mtd-vat.h"		/* for default (public) visibility */
#include "endpoints.h"

#include <curl/curl.h>

#define API_VER			"Accept: application/vnd.hmrc.1.0+json"

/*
 * [GET ]
 * /organisations/vat/{vrn}/payments?from=YYYY-MM-DD&to=YYYY-MM-DD
 */
int mtd_vat_list_payments(const char *vrn, const char *query_string,
			  char **buf)
{
	return do_ep(VAT_LIST_PAYMENTS, API_VER,
		     NULL, buf, vrn, query_string, (char *)NULL);
}

/*
 * [GET ]
 * /organisations/vat/{vrn}/liabilities?from=YYYY-MM-DD&to=YYYY-MM-DD
 */
int mtd_vat_list_liabilities(const char *vrn, const char *query_string,
			     char **buf)
{
	return do_ep(VAT_LIST_LIABILITIES, API_VER,
		     NULL, buf, vrn, query_string, (char *)NULL);
}

/*
 * [GET ]
 * /organisations/vat/{vrn}/returns/{periodKey}
 */
int mtd_vat_get_period(const char *vrn, const char *period_key, char **buf)
{
	CURL *curl;
	char *pkey;
	int ret;

	curl = curl_easy_init();
	/*
	 * The period_key may contain a '#' which needs to be
	 * percent encoded.
	 */
	pkey = curl_easy_escape(curl, period_key, 0);

	ret = do_ep(VAT_GET_PERIOD, API_VER,
		    NULL, buf, vrn, pkey, (char *)NULL);

	curl_free(pkey);
	curl_easy_cleanup(curl);

	return ret;
}

/*
 * [POST]
 * /organisations/vat/{vrn}/returns
 */
int mtd_vat_submit_period(const struct mtd_dsrc_ctx *dsctx, const char *vrn,
			  char **buf)
{
	return do_ep(VAT_SUBMIT_PERIOD, API_VER,
		     dsctx, buf, vrn, (char *)NULL);
}

/*
 * [GET ]
 * /organisations/vat/{vrn}/obligations
 *
 * Optional query string
 *
 *    ?[from=YYY-MM-DD][&[to=YYYY-MM-DD]][&[status=O|F]]
 */
int mtd_vat_list_obligations(const char *vrn, const char *query_string,
			     char **buf)
{
	return do_ep(VAT_LIST_OBLIGATIONS, API_VER,
		     NULL, buf, vrn, query_string, (char *)NULL);
}
