/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * mtd-hmrc-err.c - Make Tax Digital API Errors
 *
 * Copyright (C) 2020 - 2022	Andrew Clayton <andrew@digital-domain.net>
 */

#define _GNU_SOURCE

#include <string.h>

#include <jansson.h>

#include "mtd.h"

static const struct _mtd_hmrc_err_map {
	const char *str;
} mtd_hmrc_err_map[] = {
	[MTD_HMRC_ERR_MULTIPLE] = {
		.str = "MTD_HMRC_ERR_MULTIPLE"
	},
	[MTD_HMRC_ERR_NO_MESSAGES_PRESENT] = {
		.str = "NO_MESSAGES_PRESENT"
	},
	[MTD_HMRC_ERR_MATCHING_RESOURCE_NOT_FOUND] = {
		.str = "MATCHING_RESOURCE_NOT_FOUND"
	},

	/* Generic top-level errors */
	[MTD_HMRC_ERR_BUSINESS_ERROR] = {
		.str = "BUSINESS_ERROR"
	},
	[MTD_HMRC_ERR_INVALID_REQUEST] = {
		.str = "INVALID_REQUEST"
	},

	[MTD_HMRC_ERR_UNKNOWN] = {
		.str = "MTD_HMRC_ERR_UNKNOWN"
	}
};

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
