/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * auth.c
 *
 * Copyright (c) 2020 - 2021	Andrew Clayton <andrew@digital-domain.net>
 */

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/limits.h>

#include <jansson.h>

#include "mtd.h"
#include "auth.h"
#include "mtd-priv.h"
#include "endpoints.h"
#include "curler.h"
#include "logger.h"

extern __thread struct mtd_ctx mtd_ctx;

char *load_token(const char *which, enum file_type type, enum mtd_ep_api api)
{
	char path[PATH_MAX];
	char *token;
	const char *file;
	json_t *root;
	json_t *tok_obj;
	json_error_t error;

	switch (type) {
	case FT_AUTH:
		file = "oauth.json";
		break;
	case FT_AUTH_APPLICATION:
		file = "oauth-application.json";
		break;
	case FT_CONFIG:
		file = "config.json";
		break;
	case FT_NINO:
		file = "nino.json";
		break;
	}

	snprintf(path, sizeof(path), "%s/%s", mtd_ctx.config_dir, file);

	root = json_load_file(path, 0, &error);
	if (!root && type != FT_AUTH_APPLICATION) {
		/*
		 * Don't bother displaying this error message for the
		 * auth-application.json file as this can just be
		 * re-created.
		 *
		 * This will happen for instance on a fresh setup with no
		 * config files and creating a test user.
		 */
		logger(MTD_LOG_ERR, "json_load_file: %s\n", error.text);
		return NULL;
	}
	tok_obj = root;
	if (api != MTD_EP_API_NULL)
		tok_obj = json_object_get(root, ep_api_map[api].name);
	tok_obj = json_object_get(tok_obj, which);
	if (!tok_obj)
		return NULL;
	token = strdup(json_string_value(tok_obj));
	json_decref(root);

	return token;
}

int oauther_refresh_access_token(enum mtd_ep_api api)
{
	struct mtd_dsrc_ctx dsctx;
	char data[4096];
	char path[PATH_MAX];
	char *buf;
	char *refresh_token = load_token("refresh_token", FT_AUTH, api);
	char *client_id = load_token("client_id", FT_CONFIG, MTD_EP_API_NULL);
	char *client_secret = load_token("client_secret", FT_CONFIG,
					 MTD_EP_API_NULL);
	json_t *array;
	json_t *froot;
	json_t *result;
	int err;

	snprintf(data, sizeof(data),
		 "client_secret=%s&client_id=%s&grant_type=refresh_token"
		 "&refresh_token=%s", client_secret, client_id, refresh_token);

	dsctx.data_src.buf = data;
	dsctx.data_len = strlen(data);
	dsctx.src_type = MTD_DATA_SRC_BUF;
	err = do_ep(OA_REFRESH_TOKEN, NULL, &dsctx, &buf, (char *)NULL);
	if (err) {
		logger(MTD_LOG_ERR, "%s\n", buf);
		goto out_free;
	}

	snprintf(path, sizeof(path), "%s/oauth.json", mtd_ctx.config_dir);

	array = json_loads(buf, 0, NULL);
	result = json_array_get(array, 0);
	result = json_object_get(result, "result");

	froot = json_load_file(path, 0, NULL);
	if (froot)
		json_object_set(froot, ep_api_map[api].name, result);
	else
		froot = json_pack("{s:o}", ep_api_map[api].name, result);

	json_dump_file(froot, path, JSON_INDENT(4));
	json_decref(array);
	json_decref(froot);

out_free:
	free(buf);
	free(refresh_token);
	free(client_id);
	free(client_secret);

	return err;
}

int oauther_get_application_token(enum mtd_ep_api api __unused)
{
	struct mtd_dsrc_ctx dsctx;
	char data[4096];
	char path[PATH_MAX];
	char *buf;
	char *client_id = load_token("client_id", FT_CONFIG, MTD_EP_API_NULL);
	char *client_secret = load_token("client_secret", FT_CONFIG,
					 MTD_EP_API_NULL);
	json_t *array;
	json_t *root;
	json_t *result;
	int err;

	snprintf(data, sizeof(data),
		 "client_secret=%s&client_id=%s&grant_type=client_credentials",
		 client_secret, client_id);

	dsctx.data_src.buf = data;
	dsctx.data_len = strlen(data);
	dsctx.src_type = MTD_DATA_SRC_BUF;
	err = do_ep(OA_APPLICATION_TOKEN, NULL, &dsctx, &buf, (char *)NULL);
	if (err) {
		logger(MTD_LOG_ERR, "%s\n", buf);
		goto out_free;
	}

	snprintf(path, sizeof(path), "%s/oauth-application.json",
		 mtd_ctx.config_dir);

	array = json_loads(buf, 0, NULL);
	root = json_array_get(array, 0);
	result = json_object_get(root, "result");
	json_dump_file(result, path, JSON_INDENT(4));
	json_decref(array);

out_free:
	free(buf);
	free(client_id);
	free(client_secret);

	return err;
}

int oauther_dummy(enum mtd_ep_api api __unused)
{
	return 0;
}
