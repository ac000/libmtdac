/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * mtd.c - Make Tax Digital
 *
 * Copyright (C) 2020		Andrew Clayton <andrew@digital-domain.net>
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
#include <linux/limits.h>
#include <errno.h>

#include <jansson.h>

#include <curl/curl.h>

#include "mtd.h"
#include "mtd-priv.h"
#include "auth.h"
#include "fph.h"
#include "curler.h"
#include "logger.h"

#define TEST_API_URL		"https://test-api.service.hmrc.gov.uk"
#define PROD_API_URL		"https://api.service.hmrc.gov.uk"

#define MTD_CONFIG_DIR_FMT	".config/libmtdac/%s"

static const struct mtd_ctx dfl_mtd_ctx = {
	.log_level		= MTD_LOG_ERR,
	.app_conn_type		= MTD_ACT_OTHER_DIRECT,
	.api_url		= TEST_API_URL,
};
__thread struct mtd_ctx mtd_ctx;

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
	char path[PATH_MAX];
	char uuid[37];
	char *p;
	struct stat sb;
	json_t *new;
	int err;

	snprintf(path, sizeof(path), "%s/uuid.json", mtd_ctx.config_dir);

	err = stat(path, &sb);
	if (!err) {
		logger(MTD_LOG_INFO,
		       "%s already exists, not overwriting\n", path);
		return 0;
	}

	if (errno != ENOENT) {
		char errbuf[129];

		logger(MTD_LOG_ERR, "stat %s: %s\n", path,
		       strerror_r(errno, errbuf, sizeof(errbuf)));
		return -1;
	}

	p = gen_uuid(uuid);
	if (!p) {
		logger(MTD_LOG_ERR, "error generating UUID\n");
		return -1;
	}

	new = json_pack("{s:s}", "device_id", uuid);
	json_dump_file(new, path, JSON_INDENT(4));
	json_decref(new);

	return 0;
}

static int mkdir_p(int dirfd, const char *path, mode_t mode)
{
	char *dir;
	char *ptr;
	char mdir[4096] = "\0";
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
		char *token;

		token = strsep(&dir, "/");
		if (!token)
			break;

		strcat(mdir, token);
		ret = mkdirat(dirfd, mdir, mode);
		if (ret == -1 && errno != EEXIST) {
			ret = -1;
			break;
		}
		strcat(mdir, "/");
	}
	free(ptr);

	return ret;
}

static int check_config_dir(bool is_production)
{
	char path[PATH_MAX];
	char errbuf[129];
	char *home_dir;
	char *cfg_dir;
	struct stat sb;
	int dfd;
	int err;

	home_dir = getenv("HOME");
	if (!home_dir)
		return MTD_ERR_OS;

	dfd = open(home_dir, O_PATH|O_DIRECTORY|O_CLOEXEC);
	if (dfd == -1)
		return MTD_ERR_OS;

	snprintf(path, sizeof(path), MTD_CONFIG_DIR_FMT,
		 is_production ? "prod-api" : "test-api");
	err = asprintf(&cfg_dir, "%s/%s", home_dir, path);
	if (err == -1)
		return MTD_ERR_OS;
	mtd_ctx.config_dir = cfg_dir;

	err = fstatat(dfd, path, &sb, 0);
	if (!err)
		return 0;

	err = mkdir_p(dfd, path, 0777);
	if (!err)
		return 0;

	logger(MTD_LOG_ERR, "mkdirat %s/%s: %s\n", getenv("HOME"), path,
	       strerror_r(errno, errbuf, sizeof(errbuf)));

	return MTD_ERR_OS;
}

void mtd_global_init(void)
{
	curl_global_init(CURL_GLOBAL_ALL);
}

int mtd_init(unsigned int flags, const struct mtd_cfg *cfg)
{
	int err;
	enum app_conn_type *conn_type = &mtd_ctx.app_conn_type;

	/* initialise struct mtd_ctx to default values */
	memcpy(&mtd_ctx, &dfl_mtd_ctx, sizeof(struct mtd_ctx));

	/* Check for unknown flags */
	if (flags & ~(MTD_OPT_ALL))
		return MTD_ERR_UNKNOWN_FLAGS;
	mtd_ctx.opts = flags;

	err = check_config_dir(flags & MTD_OPT_PRODUCTION_API);
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

	fph_init_ops();

	if (!cfg)
		return MTD_ERR_NONE;

	fph_set_ops(cfg->fph_ops);
	mtd_ctx.extra_hdrs = cfg->extra_hdrs;

	return MTD_ERR_NONE;
}

void mtd_deinit(void)
{
	curl_global_cleanup();

	free((char *)mtd_ctx.config_dir);
}

static const char * const api_scopes[] = {
	"write:self-assessment",
	"read:self-assessment",
	"read:individual-employment",
	"read:individual-income",
	"read:national-insurance",
	"read:vat",
	"write:vat",
	(const char *)NULL
};

extern char **environ;
int mtd_init_auth(void)
{
	struct mtd_dsrc_ctx dsctx;
	char *client_id = load_token("client_id", FT_CONFIG);
	char *client_secret = load_token("client_secret", FT_CONFIG);
	const char *args[3];
	const char * const *scope;
	char auth_code[41];
	char *buf;
	char data[4096];
	char url[2048];
	char path[PATH_MAX];
	char *s;
	json_t *array;
	json_t *root;
	json_t *result;
	pid_t child_pid;
	int len;
	int err;

	snprintf(path, sizeof(path), "%s/oauth.json", mtd_ctx.config_dir);

	printf("You need to authorise libmtdac to have read/write access to "
	       "your Self\nAssessment information.\n");
	printf("\n");
	printf("The HMRC authorisation endpoint will open up in a new browser "
	       "window/tab.\n");
	printf("Follow the login process and accept the requested grants.\n");
	printf("\n");
	printf("Copy and paste the auth code shown on screen into the below "
	       "prompt.\n");

	printf("\n\n");
	printf("                       Press ENTER to continue\n");
	getchar();

	args[0] = "xdg-open";
	len = snprintf(url, sizeof(url),
		       "%s/oauth/authorize?response_type=code&client_id=%s&scope=", mtd_ctx.api_url, client_id);
	for (scope = api_scopes; *scope != NULL; scope++)
		len += snprintf(url + len, sizeof(url) - len, "%s+", *scope);
	url[--len] = '\0';
	snprintf(url + len, sizeof(url) - len,
		 "&redirect_uri=urn:ietf:wg:oauth:2.0:oob");
	args[1] = url;
	args[2] = (const char *)NULL;
	posix_spawnp(&child_pid, args[0], NULL, NULL, (char * const *)args,
		     environ);

	printf("\nEnter authorisation code > ");
	s = fgets(auth_code, sizeof(auth_code) - 1, stdin);
	if (!s) {
		err = MTD_ERR_OS;
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

	printf("\n");
	printf("Wrote oauth.json to %s\n\n", path);
	array = json_loads(buf, 0, NULL);
	root = json_array_get(array, 0);
	result = json_object_get(root, "result");
	json_dumpf(result, stdout, JSON_INDENT(4));
	json_dump_file(result, path, JSON_INDENT(4));
	printf("\n");
	json_decref(array);
	json_decref(result);

out_free:
	free(client_id);
	free(client_secret);
	free(buf);

	return err;
}

int mtd_init_nino(void)
{
	char path[PATH_MAX];
	char nino[41];
	char *s;
	json_t *new;

	snprintf(path, sizeof(path), "%s/nino.json", mtd_ctx.config_dir);

	printf("Enter your 'NINO'          > ");
	s = fgets(nino, sizeof(nino) - 1, stdin);
	if (!s)
		return MTD_ERR_OS;
	nino[strlen(nino) - 1] = '\0';

	new = json_pack("{s:s}", "nino", nino);
	json_dump_file(new, path, JSON_INDENT(4));

	printf("\n");
	printf("Wrote nino.json to %s\n\n", path);
	json_dumpf(new, stdout, JSON_INDENT(4));
	printf("\n");

	json_decref(new);

	return MTD_ERR_NONE;
}

int mtd_init_config(void)
{
	char path[PATH_MAX];
	char client_id[41];
	char client_secret[41];
	char *s;
	json_t *new;
	int err;

	snprintf(path, sizeof(path), "%s/config.json", mtd_ctx.config_dir),

	printf("Enter your 'client_id'     > ");
	s = fgets(client_id, sizeof(client_id) - 1, stdin);
	if (!s)
		return MTD_ERR_OS;
	client_id[strlen(client_id) - 1] = '\0';

	printf("Enter your 'client_secret' > ");
	s = fgets(client_secret, sizeof(client_secret) - 1, stdin);
	if (!s)
		return MTD_ERR_OS;
	client_secret[strlen(client_secret) - 1] = '\0';

	new = json_pack("{s:s, s:s}", "client_id", client_id,
			"client_secret", client_secret);
	json_dump_file(new, path, JSON_INDENT(4));

	printf("\n");
	printf("Wrote config.json to %s\n\n", path);
	json_dumpf(new, stdout, JSON_INDENT(4));
	printf("\n");

	json_decref(new);

	err = generate_device_id();
	if (err)
		return MTD_ERR_OS;

	return MTD_ERR_NONE;
}
