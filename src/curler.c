/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * curler.c
 *
 * Copyright (C) 2020		Andrew Clayton <andrew@digital-domain.net>
 */

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <curl/curl.h>

#include <jansson.h>

#include "mtd.h"
#include "mtd-priv.h"
#include "curler.h"
#include "endpoints.h"
#include "auth.h"
#include "fph.h"
#include "logger.h"

/* Just the HTTP status codes relevant to the MTD API */
enum http_status_code {
	OK = 200,
	CREATED,
	ACCEPTED,
	NO_CONTENT = 204,
	SEE_OTHER = 303,
	BAD_REQUEST = 400,
	UNAUTHORIZED,
	FORBIDDEN = 403,
	NOT_FOUND,
	METHOD_NOT_ALLOWED,
	NOT_ACCEPTABLE,
	GONE = 410,
	TOO_MANY_REQUESTS = 429,
	INTERNAL_SERVER_ERROR = 500,
	NOT_IMPLEMENTED,
	SERVICE_UNAVAILABLE = 503,
	GATEWAY_TIMEOUT,
};

static const struct http_status_code_entry {
	const enum http_status_code sc;
	const char *str_enum;
	const char *str;
} http_status_code_map[] = {
	{ OK, "OK", "OK" },
	{ CREATED, "CREATED", "Created" },
	{ ACCEPTED, "ACCEPTED", "Accepted" },
	{ NO_CONTENT, "NO_CONTENT", "No Content" },
	{ SEE_OTHER, "SEE_OTHER", "See Other" },
	{ BAD_REQUEST, "BAD_REQUEST", "Bad Request" },
	{ UNAUTHORIZED, "UNAUTHORIZED", "Unauthorized" },
	{ FORBIDDEN, "FORBIDDEN", "Forbidden" },
	{ NOT_FOUND, "NOT_FOUND", "Not Found" },
	{ METHOD_NOT_ALLOWED, "METHOD_NOT_ALLOWED", "Method Not Allowed" },
	{ NOT_ACCEPTABLE, "NOT_ACCEPTABLE", "Not Acceptable" },
	{ GONE, "GONE", "Gone" },
	{ TOO_MANY_REQUESTS, "TOO_MANY_REQUESTS", "Too Many Requests" },
	{ INTERNAL_SERVER_ERROR, "INTERNAL_SERVER_ERROR",
				 "Internal Server Error" },
	{ NOT_IMPLEMENTED, "NOT_IMPLEMENTED", "Not Implemented" },
	{ SERVICE_UNAVAILABLE, "SERVICE_UNAVAILABLE", "Service Unavailable" },
	{ GATEWAY_TIMEOUT, "GATEWAY_TIMEOUT", "Gateway Timeout" },
};

static const struct http_method_str {
	const enum http_method method;
	const char *str;
} methods_str[] = {
	{ M_OPTIONS, "OPTIONS" },
	{ M_GET, "GET" },
	{ M_HEAD, "HEAD" },
	{ M_POST, "POST" },
	{ M_PUT, "PUT" },
	{ M_DELETE, "DELETE" },
	{ M_TRACE, "TRACE" },
	{ M_CONNECT, "CONNECT" },
};

static inline const char *http_status_code2str(enum http_status_code sc)
{
	int i = 0;
	int nr = sizeof(http_status_code_map) /
		 sizeof(http_status_code_map[0]);

	for ( ; i < nr; i++) {
		if (http_status_code_map[i].sc == sc)
			return http_status_code_map[i].str;
	}

	return "(Unknown status code)";
}

static size_t curl_readfp_cb(void *ptr, size_t size, size_t nmemb, void *userp)
{
	return fread(ptr, size, nmemb, (FILE *)userp);
}

static size_t curl_writeb_cb(void *contents, size_t size, size_t nmemb,
			     void *userp)
{
	size_t realsize = size * nmemb;
	struct curl_buf *curl_buf = userp;

	curl_buf->buf = realloc(curl_buf->buf, curl_buf->len + realsize + 1);

	memcpy(curl_buf->buf + curl_buf->len, contents, realsize);
	curl_buf->len += realsize;
	curl_buf->buf[curl_buf->len] = '\0';

	return realsize;
}

static char *strchomp(char *string)
{
	size_t len = strlen(string);
	char *ptr = string;

	ptr += len;
	while (*(--ptr)) {
		if (*ptr == ' ' ||
		    *ptr == '\t' ||
		    *ptr == '\n' ||
		    *ptr == '\r')
			*ptr = '\0';
		else
			break;
	}

	return string;
}

static size_t header_cb(char *buffer, size_t size, size_t nitems,
			void *userdata)
{
	if (strstr(buffer, "Location:")) {
		struct curl_ctx *ctx = userdata;
		char *ptr;

		ptr = strchr(buffer, ' ');
		if (!ptr)
			goto out;
		ctx->accepted_location = strdup(strchomp(++ptr));
	}

out:
	return nitems * size;
}

static void curl_ctx_free(const struct curl_ctx *ctx)
{
	free(ctx->location);
	free(ctx->accepted_location);
	free(ctx->curl_buf->buf);
	free(ctx->curl_buf);
	free(ctx->res_buf);
	curl_slist_free_all(ctx->hdrs);

	if (ctx->src_file && ctx->dsctx->src_type == MTD_DATA_SRC_FILE)
		fclose(ctx->src_file);
}

int curl_add_hdr(struct curl_ctx *ctx, const char *fmt, ...)
{
	int len;
	char *hdr;
	va_list ap;

	va_start(ap, fmt);
	len = vasprintf(&hdr, fmt, ap);
	if (len == -1) {
		va_end(ap);
		return -1;
	}
	va_end(ap);

	ctx->hdrs = curl_slist_append(ctx->hdrs, hdr);

	free(hdr);

	return 0;
}

static void set_response(struct curl_ctx *ctx)
{
	json_t *rootbuf = NULL;
	json_t *resbuf;
	json_t *new;

	if (ctx->curl_buf->buf && strlen(ctx->curl_buf->buf) > 0)
		rootbuf = json_loads(ctx->curl_buf->buf, 0, NULL);

	if (ctx->accepted_location &&
	    (ctx->status_code == ACCEPTED || ctx->status_code == CREATED)) {
		json_t *loc;

		loc = json_pack("{s:s}", "location", ctx->accepted_location);
		if (!rootbuf)
			rootbuf = json_pack("{}");
		json_object_update(rootbuf, loc);
		json_decref(loc);
	}

	new = json_pack("{s:i, s:s, s:s, s:s, s:o}",
			"status_code", ctx->status_code,
			"status_str", http_status_code2str(ctx->status_code),
			"url", ctx->url,
			"method", methods_str[ctx->http_method].str,
			"result", rootbuf ? rootbuf : json_null());

	/* Cater for multiple responses in case of redirect... */
	if (ctx->res_buf)
		resbuf = json_loads(ctx->res_buf, 0, NULL);
	else
		resbuf = json_array();
	json_array_append_new(resbuf, new);

	free(ctx->res_buf);
	ctx->res_buf = json_dumps(resbuf, 0);

	json_decref(resbuf);
}

static const char *get_user_agent(char *ua)
{
	curl_version_info_data *verinfo = curl_version_info(CURLVERSION_NOW);

	snprintf(ua, 1024, "%s %d.%d.%d (%s) / (libcurl/%s)",
		 LIBNAME, LIBMTDAC_MAJOR_VERSION, LIBMTDAC_MINOR_VERSION,
		 LIBMTDAC_MICRO_VERSION, GIT_VERSION + 1, verinfo->version);

	return ua;
}

extern __thread struct mtd_ctx mtd_ctx;
static int curl_perform(struct curl_ctx *ctx)
{
	int ret = 0;
	char ua[1024];
	CURL *curl;
	CURLcode res;

	curl = curl_easy_init();

	if (ctx->hdrs)
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, ctx->hdrs);
	curl_easy_setopt(curl, CURLOPT_URL, ctx->url);

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, ctx->write_cb);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, ctx->curl_buf);

	if (ctx->src_file)	/* maybe we're retrying the request */
		rewind(ctx->src_file);

	if (ctx->post_data) {
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, ctx->post_data);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, ctx->post_size);
	} else if (ctx->http_method == M_POST) {
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, NULL);
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, ctx->read_cb);
		curl_easy_setopt(curl, CURLOPT_READDATA, ctx->src_file);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, ctx->src_size);
	} else if (ctx->http_method == M_PUT) {
		curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, ctx->read_cb);
		curl_easy_setopt(curl, CURLOPT_READDATA, ctx->src_file);
		curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE,
				 (curl_off_t)ctx->src_size);
	}

	/* Get the returned headers */
	curl_easy_setopt(curl, CURLOPT_HEADERDATA, ctx);
	curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, header_cb);

	curl_easy_setopt(curl, CURLOPT_USERAGENT, get_user_agent(ua));

	if (mtd_ctx.log_level == MTD_LOG_DEBUG)
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

	res = curl_easy_perform(curl);
	if (res != CURLE_OK) {
		logger(MTD_LOG_ERR, "curl_easy_perform(): %s\n",
		       curl_easy_strerror(res));
		ret = MTD_ERR_CURL;
	}
	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &ctx->status_code);
	if (ctx->status_code == SEE_OTHER) {
		char *location;

		curl_easy_getinfo(curl, CURLINFO_REDIRECT_URL, &location);
		ctx->location = strdup(location);
	}
	curl_easy_cleanup(curl);

	logger(MTD_LOG_DEBUG, "[%ld] (%s)\n", ctx->status_code, ctx->url);

	set_response(ctx);

	return ret;
}

static int set_headers(struct curl_ctx *ctx)
{
	int err = MTD_ERR_NONE;
	const char * const *hdrs;

	if (!strstr(ctx->url, "/oauth/token")) {
		err = curl_add_hdr(ctx, ctx->mtd_api_ver);
		if (err)
			return MTD_ERR_OS;
	}

	switch (ctx->content_type) {
	case CONTENT_TYPE_URL_ENCODED:
		err = curl_add_hdr(ctx, "Content-Type: application/x-www-form-urlencoded");
		break;
	case CONTENT_TYPE_JSON:
		err = curl_add_hdr(ctx, "Content-Type: application/json");
		break;
	default:
		break;
	}
	if (err)
		return MTD_ERR_OS;

	if (!strstr(ctx->url, "/oauth/token")) {
		char *access_token = ep_get_token(ctx->endpoint);

		err = curl_add_hdr(ctx, "Authorization: Bearer %s",
				   access_token);
		free(access_token);

		if (err)
			return MTD_ERR_OS;
	}

	/* Add any user supplied headers */
	if (!mtd_ctx.hdrs)
		return err;

	hdrs = mtd_ctx.hdrs;
	while (*hdrs) {
		err = curl_add_hdr(ctx, *(hdrs++));
		if (err) {
			err = MTD_ERR_OS;
			break;
		}
	}

	return err;
}

static int do_curl(struct curl_ctx *ctx)
{
	int err;
	char url[URL_LEN + 1];
	bool refreshed_token = false;

	ctx->oauther = ep_set_oauther(ctx->endpoint);
	ctx->url = ep_make_url(ctx->endpoint, ctx->params, url);

curl_again:
	set_anti_fraud_hdrs(ctx);
	err = set_headers(ctx);
	if (err)
		return err;

retry_curl:
	err = curl_perform(ctx);
	if (err) {
		return MTD_ERR_CURL;
	} else if (ctx->status_code == UNAUTHORIZED && !refreshed_token) {
		if (strstr(ctx->curl_buf->buf, "INVALID_CREDENTIALS")) {
			logger(MTD_LOG_INFO, "INVALID_CREDENTIALS: "
			       "Refreshing access_token\n");
			ctx->oauther();
			curl_slist_free_all(ctx->hdrs);
			ctx->hdrs = NULL;
			*ctx->curl_buf->buf = '\0';
			ctx->curl_buf->len = 0;
			refreshed_token = true;
			logger(MTD_LOG_INFO, "Trying the request again...\n");
			goto curl_again;
		}
	} else if (ctx->status_code == TOO_MANY_REQUESTS) {
		logger(MTD_LOG_INFO, "TOO_MANY_REQUESTS. "
		       "Sleeping 1s before retrying request...\n");
		*ctx->curl_buf->buf = '\0';
		ctx->curl_buf->len = 0;
		sleep(1);
		/* no need to re-add headers... */
		goto retry_curl;
	} else if (ctx->status_code == BAD_REQUEST && refreshed_token) {
		if (strstr(ctx->curl_buf->buf, "invalid_request"))
			return MTD_ERR_NEEDS_AUTHORISATION;
	} else if (ctx->status_code == SEE_OTHER) {
		logger(MTD_LOG_INFO, "Performing re-direct: GET %s\n",
		       ctx->location);
		if (ctx->src_file) {
			fclose(ctx->src_file);
			ctx->src_file = NULL;
			ctx->src_size = 0;
		} else if (ctx->post_data) {
			ctx->post_data = NULL;
			ctx->post_size = 0;
		}
		ctx->url = ctx->location;
		ctx->http_method = M_GET;
		ctx->content_type = CONTENT_TYPE_NONE;
		curl_slist_free_all(ctx->hdrs);
		ctx->hdrs = NULL;
		goto curl_again;
	} else if (ctx->status_code >= 300) {
		return MTD_ERR_REQUEST;
	}

	return MTD_ERR_NONE;
}

static int do_put_post(struct curl_ctx *ctx, char **buf)
{
	int err;

	/*
	 * Handle endpoints that are a POST/PUT request but do not
	 * send any data such as SA_CR_INTENT_TO_CRYSTALLISE
	 */
	if (!ctx->dsctx)
		goto out_do_curl;

	switch (ctx->dsctx->src_type) {
	case MTD_DATA_SRC_FILE:
		ctx->src_file = fopen(ctx->dsctx->data_src.file, "re");
		if (!ctx->src_file) {
			logger(MTD_LOG_ERR, "couldn't open file %s\n",
			       ctx->src_file);
			return MTD_ERR_OS;
		}
		break;
	case MTD_DATA_SRC_FD:
		ctx->src_file = fdopen(ctx->dsctx->data_src.fd, "re");
		break;
	case MTD_DATA_SRC_FP:
		ctx->src_file = ctx->dsctx->data_src.fp;
		break;
	case MTD_DATA_SRC_BUF:
		ctx->post_data = ctx->dsctx->data_src.buf;
		ctx->post_size = ctx->dsctx->data_len;
		break;
	}

	switch (ctx->dsctx->src_type) {
	case MTD_DATA_SRC_FILE:
	case MTD_DATA_SRC_FD:
	case MTD_DATA_SRC_FP: {
		struct stat sb;

		if (!ctx->src_file &&
		    ctx->dsctx->src_type != MTD_DATA_SRC_FILE) {
			logger(MTD_LOG_ERR, "couldn't open file\n");
			return MTD_ERR_OS;
		}

		err = fstat(fileno(ctx->src_file), &sb);
		if (err) {
			logger(MTD_LOG_ERR, "couldn't stat() file\n");
			return MTD_ERR_OS;
		}
		ctx->src_size = sb.st_size;
		ctx->read_cb = curl_readfp_cb;
	}
	default:
		break;
	}

out_do_curl:
	ctx->write_cb = curl_writeb_cb;
	ctx->curl_buf = calloc(1, sizeof(struct curl_buf));

	err = do_curl(ctx);
	if (ctx->res_buf)
		*buf = strdup(ctx->res_buf);

	curl_ctx_free(ctx);

	return err;
}

int do_put(struct curl_ctx *ctx, char **buf)
{
	return do_put_post(ctx, buf);
}

int do_post(struct curl_ctx *ctx, char **buf)
{
	return do_put_post(ctx, buf);
}

int do_get_delete(struct curl_ctx *ctx, char **buf)
{
	int err;

	ctx->write_cb = curl_writeb_cb;
	ctx->curl_buf = calloc(1, sizeof(struct curl_buf));

	err = do_curl(ctx);
	if (ctx->res_buf)
		*buf = strdup(ctx->res_buf);

	curl_ctx_free(ctx);

	return err;
}

int do_delete(struct curl_ctx *ctx, char **buf)
{
	return do_get_delete(ctx, buf);
}

int do_get(struct curl_ctx *ctx, char **buf)
{
	return do_get_delete(ctx, buf);
}
