/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * vldt.c - Make Tax Digital - Self Assessment API
 *
 * Copyright (C) 2025    Andrew Clayton <ac@sigsegv.uk>
 */

#define _GNU_SOURCE

#include <stdbool.h>
#include <regex.h>

#include "mtd.h"

static const int regcomp_flags = REG_EXTENDED|REG_NOSUB;

static bool check_nino(const char *str);

static const struct {
	const char *regex;
	bool (*func)(const char *str);
} regexes[] = {
	[MTD_VLDT_FMT_ACCOUNT_NAME] = {
		.regex	= "^[A-Za-z0-9 &'()*,-./@£]{1,32}$"
	},
	[MTD_VLDT_FMT_BUSINESS_ID] = {
		.regex	= "^X[A-Z0-9]{1}IS[0-9]{11}$",
	},
	[MTD_VLDT_FMT_CALCULATION_ID] = {
		.regex	= "^[0-9]{8}|[0-9a-f]{8}-[0-9a-f]{4}-[1-5][0-9a-f]{3}-[89ab][0-9a-f]{3}-[0-9a-f]{12}$"
	},
	[MTD_VLDT_FMT_CHARITY_NAME] = {
		.regex	= "^[-A-Za-z0-9 &'()*,./@£]{1,75}$"
	},
	[MTD_VLDT_FMT_COMPANY_NAME] = {
		.regex	= "^.{0,160}$"
	},
	[MTD_VLDT_FMT_COMPANY_NUMBER] = {
		.regex	= "^[0-9]{8}|[A-Za-z]{2}[0-9]{6}$"
	},
	[MTD_VLDT_FMT_EMPLOYER_NAME] = {
		.regex	= "^\\S.{0,73}$"
	},
	[MTD_VLDT_FMT_EMPLOYER_REF] = {
		.regex	= "^[0-9]{3}/[^ ].{0,9}$"
	},
	[MTD_VLDT_FMT_ID_15] = {
		.regex	= "^[A-Za-z0-9]{15}$"
	},
	[MTD_VLDT_FMT_I_F_NAME] = {
		.regex	= "^[-0-9a-zA-Z{À-˿'} _&`():.'^]{1,105}$"
	},
	[MTD_VLDT_FMT_ISO_8601_DATE] = {
		.regex	= "^[0-9]{4}-[0-9]{2}-[0-9]{2}$"
	},
	[MTD_VLDT_FMT_NINO] = {
		.func	= check_nino
	},
	[MTD_VLDT_FMT_PAYROLL_ID] = {
		.regex	= "^[-A-Za-z0-9.,()/=!\\\"%&*; <>'+:?]{0,38}$"
	},
	[MTD_VLDT_FMT_PERIOD_ID] = {
		.regex = "[0-9]{4}-[0-9]{2}-[0-9]{2}_[0-9]{4}-[0-9]{2}-[0-9]{2}$"
	},
	[MTD_VLDT_FMT_REF] = {
		.regex	= "^[-0-9a-zA-Z{À-˿’} _&`():.'^]{1,90}$"
	},
	[MTD_VLDT_FMT_SCHEME_REF] = {
		.regex	= "^[0-9]{4}-[0-9]{2}$"
	},
	[MTD_VLDT_FMT_SCHEME_REF_TAS] = {
		.regex	= "^[0-9]{8}$"
	},
	[MTD_VLDT_FMT_TAX_YEAR] = {
		.regex	= "^2[0-9]{3}-[0-9]{2}$"
	},
	[MTD_VLDT_FMT_UUID] = {
		.regex	= "^[0-9a-f]{8}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{12}$"
	},
	[MTD_VLDT_FMT_UUID_89AB] = {
		.regex	= "^[0-9a-f]{8}-[0-9a-f]{4}-[1-5][0-9a-f]{3}-[89ab][0-9a-f]{3}-[0-9a-f]{12}$"
	},
};

static bool check_nino(const char *str)
{
	regex_t re;
	int err;
	int ret;

	/*
	 * Ideally we'd use something like
	 *
	 * ^(?!(BG|GB|KN|NK|NT|TN|ZZ)|(D|F|I|Q|U|V)[A-Z]|[A-Z](D|F|I|O|Q|U|V))[A-Z]{2}[0-9]{6}[A-D]$
	 *
	 * However, POSIX EREs don't support lookahead, so we need to
	 * split the check up into two parts
	 *
	 *   1) Check if the NINO's first two characters are forbidden.
	 *      This handles the initial negative lookahead part
	 *
	 *      ^(?!(BG|GB|KN|NK|NT|TN|ZZ)|(D|F|I|Q|U|V)[A-Z]|[A-Z](D|F|I|O|Q|U|V))
	 *
	 *   2) The general format check
	 *
	 *      [A-Z]{2}[0-9]{6}[A-D]$
	 */

	err = regcomp(&re,
		      "^(BG|GB|KN|NK|NT|TN|ZZ)|(D|F|I|Q|U|V)[A-Z]|[A-Z](D|F|I|O|Q|U|V)",
		      regcomp_flags);
	if (err)
		return false;

	ret = regexec(&re, str, 0, NULL, 0);
	if (ret != REG_NOMATCH)
		return false;

	regfree(&re);

	err = regcomp(&re, "^[A-Z]{2}[0-9]{6}[A-D]$", regcomp_flags);
	if (err)
		return false;

	ret = regexec(&re, str, 0, NULL, 0);

	regfree(&re);

	return true;
}

bool mtd_is_valid_fmt(enum mtd_vldt_fmt what, const char *str)
{
	int err;
	int ret;
	bool match;
	regex_t re;

	if (regexes[what].func)
		return regexes[what].func(str);

	err = regcomp(&re, regexes[what].regex, regcomp_flags);
	if (err)
		return false;

	ret = regexec(&re, str, 0, NULL, 0);
	if (ret == REG_NOMATCH)
		match = false;
	else
		match = true;

	regfree(&re);

	return match;
}
