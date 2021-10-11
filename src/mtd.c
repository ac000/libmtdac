/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * mtd.c - Make Tax Digital
 *
 * Copyright (C) 2020 - 2021	Andrew Clayton <andrew@digital-domain.net>
 */

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <spawn.h>
#include <limits.h>
#include <errno.h>

#include <jansson.h>

#include <curl/curl.h>

#include "mtd.h"
#include "mtd-priv.h"
#include "endpoints.h"
#include "auth.h"
#include "fph.h"
#include "curler.h"
#include "logger.h"

#define TEST_API_URL		"https://test-api.service.hmrc.gov.uk"
#define PROD_API_URL		"https://api.service.hmrc.gov.uk"

static const struct _mtd_err_map {
	const char *estr;
	const char *str;
} mtd_err_map[] = {
	[MTD_ERR_NONE] = {
		.estr	= "MTD_ERR_NONE",
		.str	= "No Error"
	},
	[MTD_ERR_OS] = {
		.estr	= "MTD_ERR_OS",
		.str	= "Operating System level error (check errno)"
	},
	[MTD_ERR_REQUEST] = {
		.estr	= "MTD_ERR_REQUEST",
		.str	= "HTTP request error"
	},
	[MTD_ERR_CURL] = {
		.estr	= "MTD_ERR_CURL",
		.str	= "Curl error"
	},
	[MTD_ERR_NEEDS_AUTHORISATION] = {
		.estr	= "MTD_ERR_NEEDS_AUTHORISATION",
		.str	= "Need to refresh the bearer token"
	},
	[MTD_ERR_UNKNOWN_FLAGS]	= {
		.estr	= "MTD_ERR_UNKNOWN_FLAGS",
		.str	= "One or more unknown flags provided"
	},
	[MTD_ERR_UNKNOWN_SCOPES] = {
		.estr	= "MTD_ERR_UNKNOWN_SCOPES",
		.str	= "One or more unknown/invalid scopes provided"
	},
	[MTD_ERR_LIB_TOO_OLD] = {
		.estr	= "MTD_ERR_LIB_TOO_OLD",
		.str	= "Library version too old"
	},
	[MTD_ERR_CONFIG_DIR_UNSPEC] = {
		.estr	= "MTD_ERR_CONFIG_DIR_UNSPEC",
		.str	= "Config directory not specified"
	},
	[MTD_ERR_CONFIG_DIR_INVALID] = {
		.estr	= "MTD_ERR_CONFIG_DIR_INVALID",
		.str	= "Config directory path is invalid"
	},
	[MTD_ERR_NO_CONFIG] = {
		.estr	= "MTD_ERR_NO_CONFIG",
		.str	= "No config specified"
	},
	[MTD_ERR_INVALID_EP_API] = {
		.estr	= "MTD_ERR_INVALID_EP_API",
		.str	= "Invalid Endpoint API"
	},

	/* keep this last */
	[MTD_ERR_INVALID_ERROR] = {
		.estr	= "MTD_ERR_INVALID_ERROR",
		.str	= "Invalid error code"
	}
};

static const struct _mtd_hmrc_err_map {
	const char *str;
} mtd_hmrc_err_map[] = {
	[MTD_HMRC_ERR_MULTIPLE] = {
		.str = "MTD_HMRC_ERR_MULTIPLE",
	},
	[MTD_HMRC_ERR_NO_MESSAGES_PRESENT] = {
		.str = "NO_MESSAGES_PRESENT",
	},
	[MTD_HMRC_ERR_MATCHING_RESOURCE_NOT_FOUND] = {
		.str = "MATCHING_RESOURCE_NOT_FOUND",
	},

	/* Generic top-level errors */
	[MTD_HMRC_ERR_BUSINESS_ERROR] = {
		.str = "BUSINESS_ERROR",
	},
	[MTD_HMRC_ERR_INVALID_REQUEST] = {
		.str = "INVALID_REQUEST",
	},

	[MTD_HMRC_ERR_UNKNOWN] = {
		.str = "MTD_HMRC_ERR_UNKNOWN",
	}
};

static const struct mtd_ctx dfl_mtd_ctx = {
	.log_level		= MTD_LOG_ERR,
	.app_conn_type		= MTD_ACT_OTHER_DIRECT,
	.api_url		= TEST_API_URL,
};
__thread struct mtd_ctx mtd_ctx;

const char *mtd_err2enum_str(int err)
{
	if (err > MTD_ERR_NONE || err <= -MTD_ERR_INVALID_ERROR)
		err = -MTD_ERR_INVALID_ERROR;

	return mtd_err_map[-err].estr;
}

const char *mtd_err2str(int err)
{
	if (err > MTD_ERR_NONE || err <= -MTD_ERR_INVALID_ERROR)
		err = -MTD_ERR_INVALID_ERROR;

	return mtd_err_map[-err].str;
}

enum mtd_http_status_code mtd_http_status_code(const char *json)
{
	json_t *jarray;
	json_t *root;
	json_t *status;
	enum mtd_http_status_code sc;

	/* return the last status code... */
	jarray = json_loads(json, 0, NULL);
	root = json_array_get(jarray, json_array_size(jarray) - 1);
	status = json_object_get(root, "status_code");
	sc = json_integer_value(status);
	json_decref(jarray);

	return sc;
}

const char *mtd_http_status_str_u(const char *json)
{
	json_t *jarray;
	json_t *root;
	json_t *status;
	enum mtd_http_status_code sc;
	const char *str;

	/* return the last status code... */
	jarray = json_loads(json, 0, NULL);
	root = json_array_get(jarray, json_array_size(jarray) - 1);
	status = json_object_get(root, "status_code");
	sc = json_integer_value(status);
	str = http_status_code2str_u(sc);
	json_decref(jarray);

	return str;
}

const char *mtd_http_status_str(const char *json)
{
	json_t *jarray;
	json_t *root;
	json_t *status;
	enum mtd_http_status_code sc;
	const char *str;

	/* return the last status code... */
	jarray = json_loads(json, 0, NULL);
	root = json_array_get(jarray, json_array_size(jarray) - 1);
	status = json_object_get(root, "status_code");
	sc = json_integer_value(status);
	str = http_status_code2str(sc);
	json_decref(jarray);

	return str;
}

enum mtd_hmrc_error mtd_hmrc_error(const char *json)
{
	json_t *jarray;
	json_t *root;
	json_t *result;
	json_t *code_obj;
	json_t *ecode_obj = NULL;
	json_t *errors;
	const char *code;
	int i;
	enum mtd_hmrc_error err = MTD_HMRC_ERR_UNKNOWN;

	jarray = json_loads(json, 0, NULL);
	root = json_array_get(jarray, json_array_size(jarray) - 1);
	result = json_object_get(root, "result");
	errors = json_object_get(root, "errors");
	if (errors && json_array_size(errors) > 1) {
		err = MTD_HMRC_ERR_MULTIPLE;
		goto out_free;
	} else if (errors) {
		ecode_obj = json_object_get(errors, "code");
		code_obj = ecode_obj;
	}

	/*
	 * You can get 1 or more HMRC errors returned from endpoints.
	 *
	 * Normally a single error would just be in the top level
	 * 'result' and multiple errors would be contained in an
	 * 'errors' array.
	 *
	 * However you can also get an errors array that only contains
	 * a single error and still have a top level error.
	 *
	 * Whether there's just a single error or multiple errors in
	 * the errors array, that will be the more specific error. So
	 * we want to check for that error first then fallback to the
	 * top level error.
	 *
	 * The reason for two separate loops for the two codes is so
	 * that we can put in generic codes like 'INVALID_REQUEST' &
	 * 'BUSINESS_ERROR' and we will only use them if we didn't
	 * find a more specific error code first.
	 */
	if (!ecode_obj)
		code_obj = json_object_get(result, "code");
	if (!code_obj)
		goto out_free;

	code = json_string_value(code_obj);
	for (i = 0; i < MTD_HMRC_ERR_UNKNOWN; i++) {
		if (strcmp(mtd_hmrc_err_map[i].str, code) != 0)
			continue;

		err = i;
		goto out_free;
	}

	if (!ecode_obj)
		goto out_free;

	code_obj = json_object_get(result, "code");
	code = json_string_value(code_obj);
	for (i = 0; i < MTD_HMRC_ERR_UNKNOWN; i++) {
		if (strcmp(mtd_hmrc_err_map[i].str, code) != 0)
			continue;

		err = i;
		break;
	}

out_free:
	json_decref(jarray);

	return err;
}

char *mtd_percent_encode(const char *str, ssize_t len)
{
	char *p = (char *)str;
	char *buf;
	char *p2;
	size_t buflen;

	buflen = ((len > -1 ? (size_t)len : strlen(str)) * 3) + 1;
	buf = malloc(buflen);
	if (!buf) {
		logger(MTD_LOG_ERRNO, "malloc:");
		return NULL;
	}
	p2 = buf;

	while (*p) {
		switch (*p) {
		case ' ':	case '\n':	case '\r':	case '!':
		case '*':	case '\'':	case '(':	case ')':
		case ';':	case ':':	case '@':	case '&':
		case '=':	case '+':	case '$':	case ',':
		case '/':	case '?':	case '#':	case '[':
		case ']':	case '%':
			p2 += sprintf(p2, "%%%02X", *p);
			break;
		default:
			*(p2++) = *p;
		}
		p++;
	}
	*p2 = '\0';

	return buf;
}

static char *gen_uuid(char *buf)
{
	FILE *fp;
	size_t bytes_read;

	fp = fopen("/proc/sys/kernel/random/uuid", "r");
	if (!fp)
		return NULL;

	bytes_read = fscanf(fp, "%36s", buf);
	fclose(fp);

	if (bytes_read == 0)
		return NULL;

	return buf;
}

static int generate_device_id(void)
{
	char uuid[37];
	char *p;
	struct stat sb;
	json_t *new;
	int dfd;
	int fd;
	int err;
	int ret = -MTD_ERR_OS;

	dfd = open(mtd_ctx.config_dir, O_PATH|O_DIRECTORY|O_CLOEXEC);
	if (dfd == -1)
		return -MTD_ERR_CONFIG_DIR_INVALID;

	err = fstatat(dfd, "uuid.json", &sb, 0);
	if (!err) {
		logger(MTD_LOG_INFO,
		       "%s/uuid.json already exists, not overwriting\n",
		       mtd_ctx.config_dir);
		close(dfd);
		return MTD_ERR_NONE;
	}
	if (errno != ENOENT) {
		char errbuf[129];

		logger(MTD_LOG_ERR, "stat %s/uuid.json: %s\n",
		       mtd_ctx.config_dir,
		       strerror_r(errno, errbuf, sizeof(errbuf)));
		close(dfd);
		return -MTD_ERR_OS;
	}

	fd = openat(dfd, "uuid.json", O_CREAT|O_WRONLY|O_TRUNC|O_CLOEXEC,
		    0600);
	if (fd == -1) {
		close(dfd);
		return -MTD_ERR_OS;
	}

	p = gen_uuid(uuid);
	if (!p) {
		logger(MTD_LOG_ERR, "error generating UUID\n");
		goto out_close;
	}

	new = json_pack("{s:s}", "device_id", uuid);
	json_dumpfd(new, fd, JSON_INDENT(4));
	json_decref(new);

	ret = MTD_ERR_NONE;

out_close:
	close(fd);
	close(dfd);

	return ret;
}

static int mkdir_p(int dirfd, const char *path, mode_t mode)
{
	char *dir;
	char *ptr;
	char mdir[PATH_MAX] = "\0";
	int ret = 0;

	if (strlen(path) >= sizeof(mdir)) {
		errno = ENAMETOOLONG;
		return -1;
	}

	if (*path == '/')
		strcat(mdir, "/");

	dir = strdup(path);
	ptr = dir;
	for (;;) {
		struct stat sb;
		char *token;
		int err;

		token = strsep(&dir, "/");
		if (!token)
			break;

		strcat(mdir, token);
		strcat(mdir, "/");

		err = fstatat(dirfd, mdir, &sb, 0);
		if (!err)
			continue;

		ret = mkdirat(dirfd, mdir, mode);
		if (ret == -1 && errno != EEXIST) {
			ret = -1;
			break;
		}
	}
	free(ptr);

	return ret;
}

static int check_config_dir(const char *config_dir, bool is_production)
{
	char errbuf[129];
	char *cfg_dir;
	struct stat sb;
	int dfd;
	int err;
	int ret = MTD_ERR_NONE;

	dfd = open(config_dir, O_PATH|O_DIRECTORY|O_CLOEXEC);
	if (dfd == -1)
		return -MTD_ERR_CONFIG_DIR_INVALID;

	err = asprintf(&cfg_dir, "%s/libmtdac/%s", config_dir,
		       is_production ? "prod-api" : "test-api");
	if (err == -1) {
		ret = -MTD_ERR_OS;
		goto out_close;
	}

	mtd_ctx.config_dir = cfg_dir;

	/*
	 * The chances are the config directory exists and we
	 * can short cut out.
	 */
	err = fstatat(dfd, cfg_dir, &sb, 0);
	if (!err)
		goto out_close;

	err = mkdir_p(dfd, cfg_dir, 0700);
	if (!err)
		goto out_close;

	ret = -MTD_ERR_OS;
	logger(MTD_LOG_ERR, "mkdirat %s: %s\n", cfg_dir,
	       strerror_r(errno, errbuf, sizeof(errbuf)));

out_close:
	close(dfd);

	return ret;
}

void mtd_global_init(void)
{
	curl_global_init(CURL_GLOBAL_ALL);
}

int mtd_init(unsigned int flags, const struct mtd_cfg *cfg)
{
	int err;
	enum app_conn_type *conn_type = &mtd_ctx.app_conn_type;

	if (!cfg)
		return -MTD_ERR_NO_CONFIG;

	/* initialise struct mtd_ctx to default values */
	memcpy(&mtd_ctx, &dfl_mtd_ctx, sizeof(struct mtd_ctx));

	/* Check for unknown flags */
	if (flags & ~(MTD_OPT_ALL))
		return -MTD_ERR_UNKNOWN_FLAGS;
	mtd_ctx.opts = flags;

	if (!cfg->config_dir)
		return -MTD_ERR_CONFIG_DIR_UNSPEC;
	err = check_config_dir(cfg->config_dir,
			       flags & MTD_OPT_PRODUCTION_API);
	if (err)
		return err;

	if (flags & MTD_OPT_PRODUCTION_API)
		mtd_ctx.api_url = PROD_API_URL;

	if (flags & MTD_OPT_LOG_INFO)
		mtd_ctx.log_level = MTD_LOG_INFO;
	else if (flags & MTD_OPT_LOG_DEBUG)
		mtd_ctx.log_level = MTD_LOG_DEBUG;

	if (flags & MTD_OPT_GLOBAL_INIT)
		curl_global_init(CURL_GLOBAL_ALL);

	if (flags & MTD_OPT_ACT_MOBILE_APP_DIRECT)
		*conn_type = MTD_ACT_MOBILE_APP_DIRECT;
	else if (flags & MTD_OPT_ACT_DESKTOP_APP_DIRECT)
		*conn_type = MTD_ACT_DESKTOP_APP_DIRECT;
	else if (flags & MTD_OPT_ACT_MOBILE_APP_VIA_SERVER)
		*conn_type = MTD_ACT_MOBILE_APP_VIA_SERVER;
	else if (flags & MTD_OPT_ACT_DESKTOP_APP_VIA_SERVER)
		*conn_type = MTD_ACT_DESKTOP_APP_VIA_SERVER;
	else if (flags & MTD_OPT_ACT_WEB_APP_VIA_SERVER)
		*conn_type = MTD_ACT_WEB_APP_VIA_SERVER;
	else if (flags & MTD_OPT_ACT_BATCH_PROCESS_DIRECT)
		*conn_type = MTD_ACT_BATCH_PROCESS_DIRECT;
	else if (flags & MTD_OPT_ACT_OTHER_VIA_SERVER)
		*conn_type = MTD_ACT_OTHER_VIA_SERVER;

	fph_set_ops(mtd_ctx.app_conn_type, cfg->fph_ops);

	mtd_ctx.cfg = cfg;

	err = generate_device_id();
	if (err)
		return -MTD_ERR_OS;

	return MTD_ERR_NONE;
}

void mtd_deinit(void)
{
	curl_global_cleanup();

	free((char *)mtd_ctx.config_dir);
}

int write_config(const char *dir, const char *name, const json_t *json)
{
	int dfd;
	int fd;
	int err;
	int ret = MTD_ERR_NONE;

	dfd = open(dir, O_PATH|O_DIRECTORY|O_CLOEXEC);
	if (dfd == -1)
		return -MTD_ERR_CONFIG_DIR_INVALID;

	fd = openat(dfd, name, O_CREAT|O_WRONLY|O_TRUNC|O_CLOEXEC, 0600);
	if (fd == -1) {
		close(dfd);
		logger(MTD_LOG_ERRNO, "openat");
		return -MTD_ERR_OS;
	}

	err = json_dumpfd(json, fd, JSON_INDENT(4));
	if (err) {
		logger(MTD_LOG_ERR, "json_dump() returned -1\n");
		ret = -MTD_ERR_OS;
	}

	close(fd);
	close(dfd);

	return ret;;
}

static const struct {
	const enum mtd_scope scope;
	const char *str;
} scope_map[] = {
	{
		.scope	= MTD_SCOPE_RD_SA,
		.str	= "read:self-assessment"
	}, {
		.scope	= MTD_SCOPE_WR_SA,
		.str	= "write:self-assessment"
	}, {
		.scope	= MTD_SCOPE_RD_VAT,
		.str	= "read:vat"
	}, {
		.scope	= MTD_SCOPE_WR_VAT,
		.str	= "write:vat"
	},
};
#define ALL_SCOPES	(MTD_SCOPE_RD_SA|MTD_SCOPE_WR_SA|MTD_SCOPE_RD_VAT| \
			 MTD_SCOPE_WR_VAT)
#define RD_SCOPES	(MTD_SCOPE_RD_SA|MTD_SCOPE_RD_VAT)
#define WR_SCOPES	(MTD_SCOPE_WR_SA|MTD_SCOPE_WR_VAT)

extern char **environ;
int mtd_init_auth(enum mtd_ep_api api, unsigned long scopes)
{
	struct mtd_dsrc_ctx dsctx;
	char *client_id = NULL;
	char *client_secret = NULL;
	const char *args[3];
	char auth_code[41];
	char *buf = NULL;
	char data[4096];
	char url[2048];
	char *s;
	json_t *array;
	json_t *root;
	json_t *froot = NULL;
	json_t *result;
	pid_t child_pid;
	int n = sizeof(scope_map) / sizeof(scope_map[0]);
	int len;
	int err;
	bool rd_scope = scopes & RD_SCOPES;
	bool wr_scope = scopes & WR_SCOPES;
	bool reset_oauth;

	/* Check for unknown/invalid scopes */
	if (scopes == 0 || scopes & ~(ALL_SCOPES)) {
		err = -MTD_ERR_UNKNOWN_SCOPES;
		goto out_free;
	}

	reset_oauth = !(api & MTD_EP_API_ADD);
	api &= ~MTD_EP_API_ADD;

	client_id = load_token("client_id", FT_CREDS, api);
	client_secret = load_token("client_secret", FT_CREDS, api);

	printf("You need to authorise libmtdac to have '%s%s%s' access to "
	       "your\n'%s' information.\n",
	       rd_scope ? "read" : "", rd_scope && wr_scope ? "/" : "",
	       wr_scope ? "write" : "", ep_api_map[api].fname);
	printf("\n");
	printf("The HMRC authorisation endpoint will open up in a new browser "
	       "window/tab.\n");
	printf("Follow the login process and accept the requested grants.\n");
	printf("\n");
	printf("Copy and paste the auth code shown on screen into the below "
	       "prompt.\n");

	args[0] = "xdg-open";
	len = snprintf(url, sizeof(url),
		       "%s/oauth/authorize?response_type=code&client_id=%s&scope=", mtd_ctx.api_url, client_id);
	for (int i = 0; i < n; i++) {
		if (scopes & scope_map[i].scope) {
			snprintf(url + len, sizeof(url) - len, "%s+",
				 scope_map[i].str);
			len = strlen(url);
		}
	}
	url[--len] = '\0';
	snprintf(url + len, sizeof(url) - len,
		 "&redirect_uri=urn:ietf:wg:oauth:2.0:oob");
	args[1] = url;
	args[2] = (const char *)NULL;

	printf("\n");
	printf("If a browser window doesn't pop up, you can use the below "
	       "url.\n\n");
	printf("  %s\n\n\n", url);
	printf("                       Press ENTER to continue\n");
	getchar();

	posix_spawnp(&child_pid, args[0], NULL, NULL, (char * const *)args,
		     environ);

	printf("\nEnter authorisation code > ");
	s = fgets(auth_code, sizeof(auth_code) - 1, stdin);
	if (!s) {
		err = -MTD_ERR_OS;
		goto out_free;
	}
	auth_code[strlen(auth_code) - 1] = '\0';

	printf("\n");
	printf("Attempting to exchange authorisation code for access token\n");

	snprintf(data, sizeof(data),
		 "client_secret=%s&client_id=%s&grant_type=authorization_code"
		 "&code=%s&redirect_uri=urn:ietf:wg:oauth:2.0:oob",
		 client_secret, client_id, auth_code);

	dsctx.data_src.buf = data;
	dsctx.data_len = strlen(data);
	dsctx.src_type = MTD_DATA_SRC_BUF;
	err = do_ep(OA_EXCHANGE_AUTH_CODE, NULL, &dsctx, &buf, (char *)NULL);
	if (err)
		goto out_free;

	array = json_loads(buf, 0, NULL);
	root = json_array_get(array, 0);
	result = json_object_get(root, "result");

	if (!reset_oauth) {
		char path[PATH_MAX];

		snprintf(path, sizeof(path), "%s/oauth.json",
			 mtd_ctx.config_dir);
		froot = json_load_file(path, 0, NULL);
		if (froot)
			json_object_set(froot, ep_api_map[api].name, result);
	}
	if (!froot)
		froot = json_pack("{s:o}", ep_api_map[api].name, result);

	err = write_config(mtd_ctx.config_dir, "oauth.json", froot);
	if (err)
		goto out_free_json;

	printf("\n");
	printf("Wrote oauth.json to %s/oauth.json\n\n", mtd_ctx.config_dir);
	json_dumpf(froot, stdout, JSON_INDENT(4));
	printf("\n");

out_free_json:
	json_decref(array);
	json_decref(froot);

out_free:
	free(client_id);
	free(client_secret);
	free(buf);

	return err;
}

int mtd_init_nino(void)
{
	char nino[41];
	char *s;
	json_t *new;
	int err;

	printf("Enter your 'NINO'          > ");
	s = fgets(nino, sizeof(nino) - 1, stdin);
	if (!s)
		return -MTD_ERR_OS;
	nino[strlen(nino) - 1] = '\0';

	new = json_pack("{s:s}", "nino", nino);
	err = write_config(mtd_ctx.config_dir, "nino.json", new);
	if (err) {
		json_decref(new);
		return err;
	}

	printf("\n");
	printf("Wrote nino.json to %s/nino.json\n\n", mtd_ctx.config_dir);
	json_dumpf(new, stdout, JSON_INDENT(4));
	printf("\n");

	json_decref(new);

	return MTD_ERR_NONE;
}

int mtd_init_creds(enum mtd_ep_api api)
{
	char client_id[41];
	char client_secret[41];
	const char *api_s;
	char *s;
	json_t *froot = NULL;
	json_t *new;
	bool reset_creds;
	int err;

	reset_creds = !(api & MTD_EP_API_ADD);
	api &= ~MTD_EP_API_ADD;

	switch (api) {
	case MTD_EP_API_ITSA:
		api_s = "ITSA";
		break;
	case MTD_EP_API_VAT:
		api_s = "VAT";
		break;
	default:
		return -MTD_ERR_INVALID_EP_API;
	}
	printf("\t\tEnter your credentials for the [%s] API\n\n", api_s);

	printf("Enter your 'client_id'     > ");
	s = fgets(client_id, sizeof(client_id) - 1, stdin);
	if (!s)
		return -MTD_ERR_OS;
	client_id[strlen(client_id) - 1] = '\0';

	printf("Enter your 'client_secret' > ");
	s = fgets(client_secret, sizeof(client_secret) - 1, stdin);
	if (!s)
		return -MTD_ERR_OS;
	client_secret[strlen(client_secret) - 1] = '\0';

	new = json_pack("{s:s, s:s}", "client_id", client_id,
			"client_secret", client_secret);
	if (!reset_creds) {
		char path[PATH_MAX];

		snprintf(path, sizeof(path), "%s/creds.json",
			 mtd_ctx.config_dir);
		froot = json_load_file(path, 0, NULL);
		if (froot)
			json_object_set_new(froot, ep_api_map[api].name, new);
	}
	if (!froot)
		froot = json_pack("{s:o}", ep_api_map[api].name, new);

	err = write_config(mtd_ctx.config_dir, "creds.json", froot);
	if (err) {
		json_decref(froot);
		return err;
	}

	printf("\n");
	printf("Wrote creds.json to %s/creds.json\n\n", mtd_ctx.config_dir);
	json_dumpf(froot, stdout, JSON_INDENT(4));
	printf("\n");

	json_decref(froot);

	return MTD_ERR_NONE;
}
