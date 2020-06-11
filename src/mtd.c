/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * mtd.c - Make Tax Digital
 *
 * Copyright (C) 2020		Andrew Clayton <andrew@digital-domain.net>
 */

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <spawn.h>
#include <linux/limits.h>
#include <errno.h>

#include <jansson.h>

#include <curl/curl.h>

#include "mtd.h"
#include "mtd-priv.h"
#include "auth.h"
#include "curler.h"
#include "logger.h"

__thread struct mtd_ctx mtd_ctx = {
	.app_conn_type	= MTD_ACT_OTHER_DIRECT,
	.log_level	= MTD_LOG_ERR,
};

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

	snprintf(path, sizeof(path), MTD_CONFIG_FMT, getenv("HOME"),
		 "uuid.json");

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

static int check_config_dir(void)
{
	char path[PATH_MAX];
	char errbuf[129];
	struct stat sb;
	int err;

	snprintf(path, sizeof(path), MTD_CONFIG_DIR_FMT, getenv("HOME"));
	err = stat(path, &sb);
	if (!err)
		return 0;

	err = mkdir(path, 0777);
	if (!err)
		return 0;

	logger(MTD_LOG_ERR, "mkdir %s: %s\n", path,
	       strerror_r(errno, errbuf, sizeof(errbuf)));

	return -1;
}

void mtd_hdrs_add(const char * const hdrs[])
{
	mtd_ctx.hdrs = hdrs;
}

void mtd_hdrs_reset(void)
{
	mtd_ctx.hdrs = NULL;
}

void mtd_global_init(void)
{
	curl_global_init(CURL_GLOBAL_ALL);
}

int mtd_init(int flags)
{
	/* Check for unknown flags */
	if (flags & ~(MTD_OPT_ALL))
		return MTD_ERR_UNKNOWN_FLAGS;

	mtd_ctx.opts = flags;

	if (flags & MTD_OPT_LOG_INFO)
		mtd_ctx.log_level = MTD_LOG_INFO;
	else if (flags & MTD_OPT_LOG_DEBUG)
		mtd_ctx.log_level = MTD_LOG_DEBUG;

	if (flags & MTD_OPT_GLOBAL_INIT)
		curl_global_init(CURL_GLOBAL_ALL);

	check_config_dir();

	return MTD_ERR_NONE;
}

void mtd_deinit(void)
{
	curl_global_cleanup();
}

extern char **environ;
int mtd_init_auth(void)
{
	struct curl_ctx ctx = { 0 };
	char *client_id = load_token("client_id", FT_CONFIG);
	char *client_secret = load_token("client_secret", FT_CONFIG);
	const char *args[3];
	char auth_code[41];
	char *buf;
	char data[4096];
	char path[PATH_MAX];
	char *s;
	json_t *array;
	json_t *root;
	json_t *result;
	pid_t child_pid;
	int err;

	snprintf(path, sizeof(path), MTD_CONFIG_FMT, getenv("HOME"),
		 "oauth.json");

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
	args[1] = BASE_URL "/oauth/authorize?response_type=code&client_id=TlQLDx2MSRpl6KAkgVeL4CaeXkAa&scope=write:self-assessment+read:self-assessment+read:national-insurance&redirect_uri=urn:ietf:wg:oauth:2.0:oob";
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

	ctx.endpoint = OA_EXCHANGE_AUTH_CODE;
	err = do_post(&ctx, NULL, data, &buf);
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

	snprintf(path, sizeof(path), MTD_CONFIG_FMT, getenv("HOME"),
		 "nino.json");

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

	snprintf(path, sizeof(path), MTD_CONFIG_FMT, getenv("HOME"),
		 "config.json");

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
