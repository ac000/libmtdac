/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * curler.h
 *
 * Copyright (C) 2020		Andrew Clayton <andrew@digital-domain.net>
 */

#ifndef _CURLER_H_
#define _CURLER_H_

#include <stdio.h>
#include <stddef.h>

#include <curl/curl.h>

#include "endpoints.h"

#ifdef __cplusplus
extern "C" {
#endif

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

struct curl_buf {
	char *buf;
	size_t len;
};

struct curl_ctx {
	const char *url;
	char *location;

	const char *mtd_api_ver;

	enum http_method http_method;
	enum endpoint endpoint;

	const char **params;
	char *res_buf;
	char *accepted_location;

	int (*oauther)(void);

	struct curl_slist *hdrs;

	size_t (*write_cb)(void *contents, size_t size, size_t nmemb,
			   void *userp);
	struct curl_buf *curl_buf;

	size_t (*read_cb)(void *ptr, size_t size, size_t nmemb, void *userp);
	FILE *src_file;
	off_t src_size;

	const char *post_data;
	size_t post_size;

	long status_code;
};

extern int curl_add_hdr(struct curl_ctx *ctx, const char *fmt, ...);
extern int do_put(struct curl_ctx *ctx, const char *src_file, const char *data,
		  char **buf);
extern int do_post(struct curl_ctx *ctx, const char *src_file,
		   const char *data, char **buf);
extern int do_delete(struct curl_ctx *ctx, char **buf);
extern int do_get(struct curl_ctx *ctx, char **buf);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _CURLER_H_ */
