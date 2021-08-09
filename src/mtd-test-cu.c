/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * mtd-test-cu.c - Make Tax Digital - Create Test User API
 *
 * Copyright (C) 2020		Andrew Clayton <andrew@digital-domain.net>
 */

#include <stddef.h>

#include "mtd-test-cu.h"		/* for default (public) visibility */
#include "endpoints.h"

#define VERSION		"1.0"
#define API_VER		"Accept: application/vnd.hmrc."VERSION "+json"

/*
 * [GET ]
 * /create-test-user/services
 */
int mtd_test_cu_list_services(char **buf)
{
	return do_ep(TEST_CU_LIST_SERVICES, API_VER,
		     NULL, buf, (char *)NULL);
}

/*
 * [POST]
 * /create-test-user/agents
 */
int mtd_test_cu_create_agent(const struct mtd_dsrc_ctx *dsctx, char **buf)
{
	return do_ep(TEST_CU_CREATE_AGENT, API_VER,
		     dsctx, buf, (char *)NULL);
}

/*
 * [POST]
 * /create-test-user/organisations
 */
int mtd_test_cu_create_organisation(const struct mtd_dsrc_ctx *dsctx,
				    char **buf)
{
	return do_ep(TEST_CU_CREATE_ORGANISATION, API_VER,
		     dsctx, buf, (char *)NULL);
}

/*
 * [POST]
 * /create-test-user/individuals
 */
int mtd_test_cu_create_individual(const struct mtd_dsrc_ctx *dsctx, char **buf)
{
	return do_ep(TEST_CU_CREATE_INDIVIDUAL, API_VER,
		     dsctx, buf, (char *)NULL);
}
