/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * mtd-test-fph.c - Make Tax Digital - Test Fraud Prevention Headers API
 *
 * Copyright (C) 2020 - 2021	Andrew Clayton <andrew@digital-domain.net>
 */

#include <stddef.h>

#include "mtd-test-fph.h"		/* for default (public) visibility */
#include "endpoints.h"

#define API_VER			"Accept: application/vnd.hmrc.1.0+json"

/*
 * [GET ]
 * /test/fraud-prevention-headers/{api}/validation-feedback[?connectionMethod=]
 */
int mtd_test_fph_feedback(const char *api, const char *query_string,
			  char **buf)
{
	return do_ep(TEST_FPH_FEEDBACK, API_VER,
		     NULL, buf, api, query_string, (char *)NULL);
}

/*
 * [GET ]
 * /test/fraud-prevention-headers/validate
 */
int mtd_test_fph_validate(char **buf)
{
	return do_ep(TEST_FPH_VALIDATE, API_VER,
		     NULL, buf, (char *)NULL);
}
