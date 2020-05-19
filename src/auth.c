/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * auth.c
 *
 * Copyright (C) 2020		Andrew Clayton <andrew@digital-domain.net>
 */

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/limits.h>

#include <jansson.h>

#include "auth.h"
#include "mtd.h"
#include "curler.h"
#include "logger.h"

char *load_token(const char *which, enum file_type type)
{
	char path[PATH_MAX];
	char *token;
	json_t *root;
	json_t *tok_obj;

	snprintf(path, sizeof(path), MTD_CONFIG_DIR_FMT, getenv("HOME"),
		 type == FT_AUTH ? "oauth.json" : "config.json");

	root = json_load_file(path, 0, NULL);
	if (!root)
		return NULL;
	tok_obj = json_object_get(root, which);
	if (!tok_obj)
		return NULL;
	token = strdup(json_string_value(tok_obj));
	json_decref(root);

	return token;
}

int refresh_access_token(void)
{
	struct curl_ctx ctx = { 0 };
	char data[4096];
	char path[PATH_MAX];
	char *buf;
	char *refresh_token = load_token("refresh_token", FT_AUTH);
	char *client_id = load_token("client_id", FT_CONFIG);
	char *client_secret = load_token("client_secret", FT_CONFIG);
	json_t *root;
	json_t *result;
	int err;

	snprintf(data, sizeof(data),
		 "client_secret=%s&client_id=%s&grant_type=refresh_token"
		 "&refresh_token=%s", client_secret, client_id, refresh_token);

	ctx.endpoint = OA_REFRESH_TOKEN;
	err = do_post(&ctx, NULL, data, &buf);
	if (err) {
		logger(MTD_LOG_ERR, "%s: %s\n", __func__, buf);
		goto out_free;
	}

	snprintf(path, sizeof(path), MTD_CONFIG_DIR_FMT, getenv("HOME"),
		 "oauth.json");

	root = json_loads(buf, 0, NULL);
	result = json_object_get(root, "result");
	json_dump_file(result, path, JSON_INDENT(4));
	json_decref(root);
	json_decref(result);

out_free:
	free(buf);
	free(refresh_token);
	free(client_id);
	free(client_secret);

	return err;
}
