/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * curler.c
 *
 * Copyright (C) 2020 - 2025	Andrew Clayton <ac@sigsegv.uk>
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
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <poll.h>
#include <time.h>
#include <errno.h>

#include <curl/curl.h>

#include <jansson.h>

#include "mtd.h"
#include "mtd-priv.h"
#include "curler.h"
#include "endpoint.h"
#include "auth.h"
#include "fph.h"
#include "logger.h"

static const struct {
	enum mtd_http_status_code sc;
	const char *str_enum;
	const char *str;
} http_status_code_map[] = {
	{ MTD_HTTP_OK, "OK", "OK" },
	{ MTD_HTTP_CREATED, "CREATED", "Created" },
	{ MTD_HTTP_ACCEPTED, "ACCEPTED", "Accepted" },
	{ MTD_HTTP_NO_CONTENT, "NO_CONTENT", "No Content" },
	{ MTD_HTTP_SEE_OTHER, "SEE_OTHER", "See Other" },
	{ MTD_HTTP_BAD_REQUEST, "BAD_REQUEST", "Bad Request" },
	{ MTD_HTTP_UNAUTHORIZED, "UNAUTHORIZED", "Unauthorized" },
	{ MTD_HTTP_FORBIDDEN, "FORBIDDEN", "Forbidden" },
	{ MTD_HTTP_NOT_FOUND, "NOT_FOUND", "Not Found" },
	{ MTD_HTTP_METHOD_NOT_ALLOWED, "METHOD_NOT_ALLOWED",
				       "Method Not Allowed" },
	{ MTD_HTTP_NOT_ACCEPTABLE, "NOT_ACCEPTABLE", "Not Acceptable" },
	{ MTD_HTTP_GONE, "GONE", "Gone" },
	{ MTD_HTTP_REQUEST_ENTITY_TOO_LARGE, "REQUEST_ENTITY_TOO_LARGE",
					     "Request Entity Too Large" },
	{ MTD_HTTP_UNSUPPORTED_MEDIA_TYPE, "UNSUPPORTED_MEDIA_TYPE",
				  "Unsupported Media Type" },
	{ MTD_HTTP_UNPROCESSABLE_CONTENT, "UNPROCESSABLE_CONTENT",
					  "Unprocessable Content" },
	{ MTD_HTTP_TOO_MANY_REQUESTS, "TOO_MANY_REQUESTS",
				      "Too Many Requests" },
	{ MTD_HTTP_INTERNAL_SERVER_ERROR, "INTERNAL_SERVER_ERROR",
					  "Internal Server Error" },
	{ MTD_HTTP_NOT_IMPLEMENTED, "NOT_IMPLEMENTED", "Not Implemented" },
	{ MTD_HTTP_SERVICE_UNAVAILABLE, "SERVICE_UNAVAILABLE",
					"Service Unavailable" },
	{ MTD_HTTP_GATEWAY_TIMEOUT, "GATEWAY_TIMEOUT", "Gateway Timeout" },
};

static const struct {
	enum http_method method;
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

const char *http_status_code2str_u(enum mtd_http_status_code sc)
{
	int i = 0;
	int nr = sizeof(http_status_code_map) /
		 sizeof(http_status_code_map[0]);

	for ( ; i < nr; i++) {
		if (http_status_code_map[i].sc == sc)
			return http_status_code_map[i].str_enum;
	}

	return "(UNKNOWN)";
}

const char *http_status_code2str(enum mtd_http_status_code sc)
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
	struct curl_ctx *ctx = userdata;
	char *hdr_val;
	char **ptr;
	size_t ret = nitems * size;

	if (strcasestr(buffer, "Location:"))
		ptr = &ctx->accepted_location;
	else if (strcasestr(buffer, "X-Correlationid:"))
		ptr = &ctx->x_corr_id;
	else if (strcasestr(buffer, "Deprecation:"))
		ptr = &ctx->deprecation_hdr;
	else if (strcasestr(buffer, "Sunset:"))
		ptr = &ctx->sunset_hdr;
	else if (strcasestr(buffer, "Link:"))
		ptr = &ctx->link_hdr;
	else
		return ret;

	hdr_val = strchr(buffer, ' ');
	if (!hdr_val)
		return ret;
	*ptr = strdup(strchomp(++hdr_val));

	return ret;
}

static void curl_ctx_free(const struct curl_ctx *ctx)
{
	free(ctx->location);
	free(ctx->accepted_location);
	free(ctx->curl_buf->buf);
	free(ctx->curl_buf);
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

/*
 * Generate a datestamp in the format (ISO 8601)
 *
 *     YYYY-MM-DDThh:mm:ss.sssZ
 *
 * e.g
 *
 *     2021-12-24T03:48:16.894Z
 */
const char *gen_datestamp(char *buf, size_t buflen)
{
	struct timespec tp;
	struct tm tm;
	size_t len;

	*buf = '\0';

	clock_gettime(CLOCK_REALTIME, &tp);
	gmtime_r(&tp.tv_sec, &tm);
	len = strftime(buf, buflen, "%FT%T", &tm);
	if (len == 0)
		return buf;

	snprintf(buf + len, buflen - len, ".%03ldZ", tp.tv_nsec / 1000000L);

	return buf;
}

static void set_response(struct curl_ctx *ctx)
{
	json_t *rootbuf = NULL;
	json_t *resbuf;
	json_t *new;
	char date[32];

	if (ctx->curl_buf->buf && strlen(ctx->curl_buf->buf) > 0)
		rootbuf = json_loads(ctx->curl_buf->buf, 0, NULL);

	if (ctx->accepted_location &&
	    (ctx->status_code == MTD_HTTP_ACCEPTED ||
	     ctx->status_code == MTD_HTTP_CREATED)) {
		json_t *loc;

		loc = json_pack("{s:s}", "location", ctx->accepted_location);
		if (!rootbuf)
			rootbuf = json_pack("{}");
		json_object_update(rootbuf, loc);
		json_decref(loc);
	}

	new = json_pack("{s:i, s:s, s:s, s:s?, s:s?, s:s, s:s, s:s?, s:s?, s:s?, s:s?, s:s, s:o?}",
			"status_code", ctx->status_code,
			"status_str", http_status_code2str(ctx->status_code),
			"libmtdac_endpoint", ctx->epstr,
			"api_name", ctx->mtd_api_name,
			"api_ver", ctx->mtd_api_ver,
			"url", ctx->url,
			"method", methods_str[ctx->http_method].str,
			"xid", ctx->x_corr_id,
			"deprecation", ctx->deprecation_hdr,
			"sunset", ctx->sunset_hdr,
			"link", ctx->link_hdr,
			"date", gen_datestamp(date, sizeof(date)),
			"result", rootbuf);

	/* Cater for multiple responses in case of redirect... */
	if (ctx->res_buf)
		resbuf = json_loads(ctx->res_buf, 0, NULL);
	else
		resbuf = json_array();
	json_array_append_new(resbuf, new);

	free(ctx->res_buf);

	free(ctx->x_corr_id);
	free(ctx->deprecation_hdr);
	free(ctx->sunset_hdr);
	free(ctx->link_hdr);
	ctx->x_corr_id = NULL;
	ctx->deprecation_hdr = NULL;
	ctx->sunset_hdr = NULL;
	ctx->link_hdr = NULL;

	ctx->res_buf = json_dumps(resbuf, JSON_INDENT(4));

	logger(MTD_LOG_DEBUG_DATA, "[%s]\n", ctx->res_buf);

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

static curl_socket_t opensocket(void *clientp, curlsocktype purpose __unused,
				struct curl_sockaddr *address __unused)
{
	return *(curl_socket_t *)clientp;
}

static int sockopt_cb(void *clientp __unused, curl_socket_t curlfd __unused,
		      curlsocktype purpose __unused)
{
	return CURL_SOCKOPT_ALREADY_CONNECTED;
}

extern __thread struct mtd_ctx mtd_ctx;
static int curl_perform(struct curl_ctx *ctx)
{
	int ret = 0;
	char ua[1024];
	CURL *curl;
	CURLcode res;

	curl = curl_easy_init();

	/* Use the already connected socket */
	curl_easy_setopt(curl, CURLOPT_OPENSOCKETFUNCTION, opensocket);
	curl_easy_setopt(curl, CURLOPT_OPENSOCKETDATA, &ctx->sockfd);
	curl_easy_setopt(curl, CURLOPT_SOCKOPTFUNCTION, sockopt_cb);

	if (ctx->hdrs)
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, ctx->hdrs);
	curl_easy_setopt(curl, CURLOPT_URL, ctx->url);

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, ctx->write_cb);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, ctx->curl_buf);

	if (ctx->src_file)	/* maybe we're retrying the request */
		rewind(ctx->src_file);

	if (ctx->http_method == M_PUT)
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
	else if (ctx->http_method == M_DELETE)
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");

	if (ctx->post_data) {
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, ctx->post_data);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, ctx->post_size);
	} else if (ctx->http_method == M_POST || ctx->http_method == M_PUT) {
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, NULL);
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, ctx->read_cb);
		curl_easy_setopt(curl, CURLOPT_READDATA, ctx->src_file);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, ctx->src_size);
	}

	/* Get the returned headers */
	curl_easy_setopt(curl, CURLOPT_HEADERDATA, ctx);
	curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, header_cb);

	curl_easy_setopt(curl, CURLOPT_USERAGENT, get_user_agent(ua));

	if (mtd_ctx.log_level == MTD_LOG_DEBUG_ALL) {
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
		if (mtd_ctx.cfg->log_fp)
			curl_easy_setopt(curl, CURLOPT_STDERR,
					 (FILE *)mtd_ctx.cfg->log_fp);
	}

	res = curl_easy_perform(curl);
	if (res != CURLE_OK) {
		logger(MTD_LOG_ERR, "curl_easy_perform(): %s\n",
		       curl_easy_strerror(res));
		ret = MTD_ERR_CURL;
	}
	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &ctx->status_code);
	if (ctx->status_code == MTD_HTTP_SEE_OTHER) {
		char *location;

		curl_easy_getinfo(curl, CURLINFO_REDIRECT_URL, &location);
		ctx->location = strdup(location);
	}
	curl_easy_cleanup(curl);

	logger(MTD_LOG_DEBUG_ALL, "[%ld] (%s)\n", ctx->status_code, ctx->url);

	set_response(ctx);

	return ret;
}

static int set_headers(struct curl_ctx *ctx)
{
	int ret = MTD_ERR_NONE;
	int err = 0;
	const char * const *hdrs;

	if (!strstr(ctx->url, "/oauth/token")) {
		err = curl_add_hdr(ctx, ctx->mtd_api_ver_hdr);
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
	if (!mtd_ctx.cfg->extra_hdrs)
		return ret;

	hdrs = mtd_ctx.cfg->extra_hdrs;
	while (*hdrs) {
		err = curl_add_hdr(ctx, *(hdrs++));
		if (err) {
			ret = MTD_ERR_OS;
			break;
		}
	}

	return ret;
}

static int try_connect(const struct addrinfo *ai)
{
	int ret;
	int err;
	int sockfd;
	int optval;
	int flags = SOCK_NONBLOCK|SOCK_CLOEXEC;
	socklen_t optlen = sizeof(optval);
	struct pollfd pfd;

	sockfd = socket(ai->ai_family, ai->ai_socktype | flags,
			ai->ai_protocol);

	ret = connect(sockfd, ai->ai_addr, ai->ai_addrlen);
	if (ret == -1 && errno != EINPROGRESS) {
		logger(MTD_LOG_ERRNO, NULL);
		close(sockfd);
		return ret;
	}

	pfd.fd = sockfd;
	pfd.events = POLLOUT;
do_poll:
	err = poll(&pfd, 1, 10000 /* 10s */);
	if (err <= 0) {
		switch (err) {
		case 0:
			logger(MTD_LOG_ERR, "Connection timed out\n");
			break;
		case -1:
			if (errno == EINTR)
				goto do_poll;

			logger(MTD_LOG_ERRNO, NULL);
			break;
		}

		close(sockfd);
		return -1;
	}

	err = getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &optval, &optlen);
	if (err || optval != 0) {
		close(sockfd);
		if (!err) {
			/* We want the error from the connect() */
			errno = optval;
		}
		logger(MTD_LOG_ERRNO, NULL);
		return -1;
	}

	/* Set socket back to blocking mode */
	flags = fcntl(sockfd, F_GETFL, 0);
	flags &= ~SOCK_NONBLOCK;
	ret = fcntl(sockfd, F_SETFL, flags);
	if (ret == -1) {
		logger(MTD_LOG_ERRNO, NULL);
		close(sockfd);
		return -1;
	}

	return sockfd;
}

static int do_connect(const struct mtd_ctx *ctx)
{
	const char *host = ctx->api_url + 8; /* skip past 'https://' */
	struct addrinfo hints;
	struct addrinfo *res;
	struct addrinfo *rp;
	int sfd = -1;
	int err;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	err = getaddrinfo(host, "443", &hints, &res);
	if (err) {
		logger(MTD_LOG_ERR, "getaddrinfo: %s\n", gai_strerror(err));
		return err;
	}

	for (rp = res; rp != NULL; rp = rp->ai_next) {
		sfd = try_connect(rp);
		if (sfd > -1)
			break;
	}
	freeaddrinfo(res);

	return sfd;
}

static int do_curl(struct curl_ctx *ctx)
{
	int err;
	char url[URL_LEN + 1];
	bool refreshed_token = false;

	ctx->oauther = ep_set_oauther(ctx->endpoint);
	ctx->url = ep_make_url(ctx->endpoint, ctx->params, url);

curl_again:
	ctx->sockfd = do_connect(&mtd_ctx);
	if (ctx->sockfd < 0)
		return MTD_ERR_OS;

	set_anti_fraud_hdrs(ctx);
	err = set_headers(ctx);
	if (err)
		return err;

	err = curl_perform(ctx);
	if (err) {
		return MTD_ERR_CURL;
	} else if (ctx->status_code == MTD_HTTP_UNAUTHORIZED &&
		   !refreshed_token) {
		if (strstr(ctx->curl_buf->buf, "INVALID_CREDENTIALS")) {
			logger(MTD_LOG_INFO, "INVALID_CREDENTIALS: "
			       "Refreshing access_token\n");
			ctx->oauther(ctx->scope);
			curl_slist_free_all(ctx->hdrs);
			ctx->hdrs = NULL;
			*ctx->curl_buf->buf = '\0';
			ctx->curl_buf->len = 0;
			refreshed_token = true;
			logger(MTD_LOG_INFO, "Trying the request again...\n");
			goto curl_again;
		}
	} else if (ctx->status_code == MTD_HTTP_TOO_MANY_REQUESTS) {
		logger(MTD_LOG_INFO, "TOO_MANY_REQUESTS. "
		       "Sleeping 1s before retrying request...\n");
		curl_slist_free_all(ctx->hdrs);
		ctx->hdrs = NULL;
		*ctx->curl_buf->buf = '\0';
		ctx->curl_buf->len = 0;
		sleep(1);
		goto curl_again;
	} else if (ctx->status_code == MTD_HTTP_BAD_REQUEST &&
		   refreshed_token) {
		if (strstr(ctx->curl_buf->buf, "invalid_request"))
			return MTD_ERR_NEEDS_AUTHORISATION;
		return MTD_ERR_REQUEST;
	} else if (ctx->status_code == MTD_HTTP_SEE_OTHER) {
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
	 * send any data such as IC_TRIGGER_CALCULATION
	 */
	if (!ctx->dsctx)
		goto out_do_curl;

	switch (ctx->dsctx->src_type) {
	case MTD_DATA_SRC_FILE:
		ctx->src_file = fopen(ctx->dsctx->data_src.file, "re");
		if (!ctx->src_file) {
			logger(MTD_LOG_ERR, "couldn't open file %s\n",
			       ctx->dsctx->data_src.file);
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
		logger(MTD_LOG_DEBUG_DATA, "[%s]\n", ctx->post_data);
		break;
	}

	switch (ctx->dsctx->src_type) {
	case MTD_DATA_SRC_FILE:
	case MTD_DATA_SRC_FD:
	case MTD_DATA_SRC_FP: {
		struct stat sb;
		char *log_buf;
		size_t r;

		if (!ctx->src_file &&
		    ctx->dsctx->src_type != MTD_DATA_SRC_FILE) {
			logger(MTD_LOG_ERR, "couldn't open file\n");
			return MTD_ERR_OS;
		}

		err = fstat(fileno(ctx->src_file), &sb);
		if (err) {
			logger(MTD_LOG_ERR, "couldn't stat() file\n");
			err = MTD_ERR_OS;
			goto out_cleanup;
		}
		ctx->src_size = sb.st_size;
		ctx->read_cb = curl_readfp_cb;

		if (mtd_ctx.log_level < MTD_LOG_DEBUG_DATA)
			break;

		log_buf = malloc(sb.st_size + 1);
		if (!log_buf) {
			logger(MTD_LOG_ERRNO, "malloc:");
			err = MTD_ERR_OS;
			goto out_cleanup;
		}

		r = fread(log_buf, 1, sb.st_size, ctx->src_file);
		if (r < (size_t)sb.st_size) {
			logger(MTD_LOG_ERR, "fread() failed\n");
			err = MTD_ERR_OS;
			free(log_buf);
			goto out_cleanup;
		}
		log_buf[sb.st_size] = '\0';

		logger(MTD_LOG_DEBUG_DATA, "[%s]\n", log_buf);

		free(log_buf);
	}
	default:
		break;
	}

out_do_curl:
	ctx->write_cb = curl_writeb_cb;
	ctx->curl_buf = calloc(1, sizeof(struct curl_buf));

	err = do_curl(ctx);

	*buf = ctx->res_buf;

out_cleanup:
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

static int do_get_delete(struct curl_ctx *ctx, char **buf)
{
	int err;

	ctx->write_cb = curl_writeb_cb;
	ctx->curl_buf = calloc(1, sizeof(struct curl_buf));

	err = do_curl(ctx);

	*buf = ctx->res_buf;

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
