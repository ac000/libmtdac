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

static inline enum mtd_api_scope get_scope(enum mtd_api_endpoint ep)
{
	enum ep_api api = endpoints[ep].api;

	return endpoints[ep].scope == MTD_API_SCOPE_UNSET ?
		api_default_values[api].scope : endpoints[ep].scope;
}

static inline enum oauth_authz get_authz(enum mtd_api_endpoint ep)
{
	enum ep_api api = endpoints[ep].api;

	return endpoints[ep].authz == AUTHZ_UNSET ?
		api_default_values[api].authz : endpoints[ep].authz;
}

int mtd_ep(enum mtd_api_endpoint ep, const struct mtd_dsrc_ctx *dsctx,
	   char **buf, const char * const params[])
{
	size_t len;
	enum ep_api api = endpoints[ep].api;
	struct curl_ctx ctx = {};
	char api_ver[128];

	*buf = NULL;

	len = snprintf(api_ver, sizeof(api_ver), API_VER_FMT,
		       api_default_values[api].api_version);
	if (len >= sizeof(api_ver))
	    return MTD_ERR_REQUEST;

	ctx.mtd_api_ver = api_ver;
	ctx.endpoint = ep;
	ctx.scope = get_scope(ep);
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
		return MTD_ERR_REQUEST;
	}
}

int (*ep_set_oauther(enum mtd_api_endpoint ep))(enum mtd_api_scope)
{
	enum oauth_authz authz = get_authz(ep);

	switch (authz) {
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
	enum oauth_authz authz = get_authz(ep);

	switch(authz) {
	case AUTHZ_USER: {
		enum mtd_api_scope scope = get_scope(ep);
		return load_token("access_token", FT_AUTH, scope);
	}
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
