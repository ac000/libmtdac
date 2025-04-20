/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * endpoint.c - Make Tax Digital - Self Assessment API
 *
 * Copyright (C) 2020 - 2025	Andrew Clayton <ac@sigsegv.uk>
 */

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mtd.h"
#include "endpoint.h"
#include "api_endpoints.h"
#include "curler.h"
#include "auth.h"
#include "logger.h"
#include "mtd-priv.h"

#define API_VER_FMT         "Accept: application/vnd.hmrc.%s+json"

static const char * const api_version_map[] = {
	[EP_API_BD]		= "1.0",
	[EP_API_BSAS]		= "6.0",
	[EP_API_ICAL]		= "7.0",
	[EP_API_ILOS]		= "5.0",
	[EP_API_ISI]		= "1.0",
	[EP_API_OB]		= "3.0",
	[EP_API_PB]		= "5.0",
	[EP_API_SEB]		= "4.0",

	[EP_API_TEST_CU]	= "1.0",
	[EP_API_TEST_FPH]	= "1.0",
};

int mtd_ep(enum mtd_api_endpoint ep, const struct mtd_dsrc_ctx *dsctx,
	   char **buf, const char * const params[])
{
	size_t len;
	struct curl_ctx ctx = {};
	char api_ver[128];

	*buf = NULL;

	len = snprintf(api_ver, sizeof(api_ver), API_VER_FMT,
		       api_version_map[endpoints[ep].api]);
	if (len >= sizeof(api_ver))
	    return -MTD_ERR_REQUEST;

	ctx.mtd_api_ver = api_ver;
	ctx.endpoint = ep;
	ctx.scope = endpoints[ep].scope;
	ctx.params = params;
	ctx.content_type = endpoints[ep].ctype;
	ctx.http_method = endpoints[ep].method;
	ctx.dsctx = dsctx;

	switch (ctx.http_method) {
	case M_GET:
		return do_get(&ctx, buf);
	case M_POST:
		return do_post(&ctx, buf);
	case M_PUT:
		return do_put(&ctx, buf);
	case M_DELETE:
		return do_delete(&ctx, buf);
	default:
		return -MTD_ERR_REQUEST;
	}
}

int (*ep_set_oauther(enum mtd_api_endpoint ep))(enum mtd_api_scope)
{
	switch (endpoints[ep].authz) {
	case AUTHZ_USER:
		return oauther_refresh_access_token;
	case AUTHZ_APPLICATION:
		return oauther_get_application_token;
	default:
		return oauther_dummy;
	}
}

char *ep_get_token(enum mtd_api_endpoint ep)
{
	switch(endpoints[ep].authz) {
	case AUTHZ_USER:
		return load_token("access_token", FT_AUTH, endpoints[ep].scope);
	case AUTHZ_APPLICATION:
		return load_token("access_token", FT_AUTH_APPLICATION,
				  MTD_API_SCOPE_NULL);
	default:
		return NULL;
	}
}

extern __thread struct mtd_ctx mtd_ctx;
char *ep_make_url(enum mtd_api_endpoint ep, const char * const params[],
		  char *url)
{
	char *nino = NULL;
	char *string;
	char *ptr;
	int len;

	string = strdup(endpoints[ep].tmpl + 1); /* skip past first '/' */
	ptr = string;

	len = snprintf(url, URL_LEN + 1, "%s", mtd_ctx.api_url);

	if (strstr(string, "{nino}"))
		nino = load_token("nino", FT_NINO, MTD_API_SCOPE_NULL);

	for (int p = 0; ;) {
		char *token;

		token = strsep(&ptr, "/");
		if (!token)
			break;

		if (*token != '{')
			snprintf(url + len, URL_LEN+1 - len, "/%s", token);
		else if (strcmp(token, "{nino}") == 0)
			snprintf(url + len, URL_LEN+1 - len, "/%s", nino);
		else if (strstr(token, "query_params"))
			snprintf(url + len, URL_LEN+1 - len, "%s",
				 params[p] ? params[p++] : "");
		else
			snprintf(url + len, URL_LEN+1 - len, "/%s",
				 params[p++]);

		len = strlen(url);
	}

	free(string);
	free(nino);

	return url;
}
