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
#include "mtd-priv.h"
#include "curler.h"
#include "logger.h"

char *load_token(const char *which, enum file_type type)
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

	snprintf(path, sizeof(path), MTD_CONFIG_FMT, getenv("HOME"), file);

	root = json_load_file(path, 0, &error);
	if (!root) {
		logger(MTD_LOG_ERR, "%s: json_load_file: %s\n", __func__,
		       error.text);
		return NULL;
	}
	tok_obj = json_object_get(root, which);
	if (!tok_obj)
		return NULL;
	token = strdup(json_string_value(tok_obj));
	json_decref(root);

	return token;
}

int oauther_refresh_access_token(void)
{
	struct curl_ctx ctx = { 0 };
	char data[4096];
	char path[PATH_MAX];
	char *buf;
	char *refresh_token = load_token("refresh_token", FT_AUTH);
	char *client_id = load_token("client_id", FT_CONFIG);
	char *client_secret = load_token("client_secret", FT_CONFIG);
	json_t *array;
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

	snprintf(path, sizeof(path), MTD_CONFIG_FMT, getenv("HOME"),
		 "oauth.json");

	array = json_loads(buf, 0, NULL);
	root = json_array_get(array, 0);
	result = json_object_get(root, "result");
	json_dump_file(result, path, JSON_INDENT(4));
	json_decref(array);
	json_decref(result);

out_free:
	free(buf);
	free(refresh_token);
	free(client_id);
	free(client_secret);

	return err;
}

int oauther_get_application_token(void)
{
	struct curl_ctx ctx = { 0 };
	char data[4096];
	char path[PATH_MAX];
	char *buf;
	char *client_id = load_token("client_id", FT_CONFIG);
	char *client_secret = load_token("client_secret", FT_CONFIG);
	json_t *array;
	json_t *root;
	json_t *result;
	int err;

	snprintf(data, sizeof(data),
		 "client_secret=%s&client_id=%s&grant_type=client_credentials",
		 client_secret, client_id);

	ctx.endpoint = OA_APPLICATION_TOKEN;
	err = do_post(&ctx, NULL, data, &buf);
	if (err) {
		logger(MTD_LOG_ERR, "%s: %s\n", __func__, buf);
		goto out_free;
	}

	snprintf(path, sizeof(path), MTD_CONFIG_FMT, getenv("HOME"),
		 "oauth-application.json");

	array = json_loads(buf, 0, NULL);
	root = json_array_get(array, 0);
	result = json_object_get(root, "result");
	json_dump_file(result, path, JSON_INDENT(4));
	json_decref(array);
	json_decref(result);

out_free:
	free(buf);
	free(client_id);
	free(client_secret);

	return err;
}

int oauther_dummy(void)
{
	return 0;
}
