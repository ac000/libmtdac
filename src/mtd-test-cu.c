/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * mtd-test-cu.c - Make Tax Digital - Create Test User API
 *
 * Copyright (C) 2020		Andrew Clayton <andrew@digital-domain.net>
 */

#include "mtd-test-cu.h"		/* for default (public) visibility */
#include "endpoints.h"
#include "curler.h"

#define API_VER			"Accept: application/vnd.hmrc.1.0+json"

/*
 * [GET ]
 * /create-test-user/services
 */
int mtd_test_cu_list_services(char **buf)
{
	struct curl_ctx ctx = { 0 };

	ctx.mtd_api_ver = API_VER;
	ctx.endpoint = TEST_CU_LIST_SERVICES;

	return do_get(&ctx, buf);
}

/*
 * [POST]
 * /create-test-user/agents
 */
int mtd_test_cu_create_agent(const char *src_file, char **buf)
{
	struct curl_ctx ctx = { 0 };

	ctx.mtd_api_ver = API_VER;
	ctx.endpoint = TEST_CU_CREATE_AGENT;

	return do_post(&ctx, src_file, NULL, buf);
}

/*
 * [POST]
 * /create-test-user/organisations
 */
int mtd_test_cu_create_organisation(const char *src_file, char **buf)
{
	struct curl_ctx ctx = { 0 };

	ctx.mtd_api_ver = API_VER;
	ctx.endpoint = TEST_CU_CREATE_ORGANISATION;

	return do_post(&ctx, src_file, NULL, buf);
}

/*
 * [POST]
 * /create-test-user/individuals
 */
int mtd_test_cu_create_individual(const char *src_file, char **buf)
{
	struct curl_ctx ctx = { 0 };

	ctx.mtd_api_ver = API_VER;
	ctx.endpoint = TEST_CU_CREATE_INDIVIDUAL;

	return do_post(&ctx, src_file, NULL, buf);
}
