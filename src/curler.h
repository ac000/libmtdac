/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * curler.h
 *
 * Copyright (C) 2020 - 2021	Andrew Clayton <andrew@digital-domain.net>
 */

#ifndef _CURLER_H_
#define _CURLER_H_

#include <stdio.h>
#include <stddef.h>

#include <curl/curl.h>

#include "mtd.h"
#include "endpoints.h"

enum http_method {
	M_OPTIONS = 0,
	M_GET,
	M_HEAD,
	M_POST,
	M_PUT,
	M_DELETE,
	M_TRACE,
	M_CONNECT,
};

enum content_type {
	CONTENT_TYPE_NONE = 0,
	CONTENT_TYPE_URL_ENCODED,
	CONTENT_TYPE_JSON,
};

struct curl_buf {
	char *buf;
	size_t len;
};

struct curl_ctx {
	int sockfd;

	const char *url;
	char *location;
	char *x_corr_id;

	const char *mtd_api_ver;

	enum http_method http_method;
	enum endpoint endpoint;

	const char * const *params;
	char *res_buf;
	char *accepted_location;

	enum mtd_ep_api api;
	int (*oauther)(enum mtd_ep_api);

	struct curl_slist *hdrs;

	size_t (*write_cb)(void *contents, size_t size, size_t nmemb,
			   void *userp);
	struct curl_buf *curl_buf;

	size_t (*read_cb)(void *ptr, size_t size, size_t nmemb, void *userp);
	FILE *src_file;
	off_t src_size;

	const char *post_data;
	size_t post_size;

	enum content_type content_type;

	const struct mtd_dsrc_ctx *dsctx;

	long status_code;
};

extern const char *http_status_code2str_u(enum mtd_http_status_code sc);
extern const char *http_status_code2str(enum mtd_http_status_code sc);
extern int curl_add_hdr(struct curl_ctx *ctx, const char *fmt, ...);
extern const char *gen_datestamp(char *buf, size_t buflen);
extern int do_put(struct curl_ctx *ctx, char **buf);
extern int do_post(struct curl_ctx *ctx, char **buf);
extern int do_delete(struct curl_ctx *ctx, char **buf);
extern int do_get(struct curl_ctx *ctx, char **buf);

#endif /* _CURLER_H_ */
