/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * mtd-ep-isi.c - Make Tax Digital - Individuals Savings Income API
 *
 * Copyright (C) 2025		Andrew Clayton <ac@sigsegv.uk>
 */

#include <stddef.h>

#include "mtd-isi.h"		/* for default (public) visibility */
#include "endpoints.h"

#define VERSION			"1.0"
#define API_VER			"Accept: application/vnd.hmrc." VERSION "+json"

/*
 * UK Savings Account
 */

/*
 * [GET ]
 * /individuals/savings-income/uk-accounts/{nino}[?savingsAccountId=^[A-Za-z0-9]{15}$]
 */
int mtd_isi_si_ua_list(const char *query_string, char **buf)
{
	EP(ISI_SI_UA_LIST, NULL, query_string)
}

/*
 * [POST]
 * /individuals/savings-income/uk-accounts/{nino}
 */
int mtd_isi_si_ua_add(const struct mtd_dsrc_ctx *dsctx, char **buf)
{
	EP(ISI_SI_UA_ADD, dsctx)
}

/*
 * [GET ]
 * /individuals/savings-income/uk-accounts/{nino}/{taxYear}/{savingsAccountId}
 */
int mtd_isi_si_ua_get_annual_summary(const char *tax_year, const char *said,
				     char **buf)
{
	EP(ISI_SI_UA_GET_ANNUAL_SUMMARY, NULL, tax_year, said)
}

/*
 * [PUT ]
 * /individuals/savings-income/uk-accounts/{nino}/{taxYear}/{savingsAccountId}
 */
int mtd_isi_si_ua_update_annual_summary(const struct mtd_dsrc_ctx *dsctx,
					const char *tax_year, const char *said,
					char **buf)
{
	EP(ISI_SI_UA_UPDATE_ANNUAL_SUMMARY, dsctx, tax_year, said)
}

/*
 * Savings Income
 */

/*
 * [GET ]
 * /individuals/savings-income/other/{nino}/{taxYear}
 */
int mtd_isi_si_o_get(const char *tax_year, char **buf)
{
	EP(ISI_SI_O_GET, NULL, tax_year)
}

/*
 * [PUT ]
 * /individuals/savings-income/other/{nino}/{taxYear}
 */
int mtd_isi_si_o_update(const struct mtd_dsrc_ctx *dsctx, const char *tax_year,
			char **buf)
{
	EP(ISI_SI_O_UPDATE, dsctx, tax_year)
}

/*
 * [DELETE]
 * /individuals/savings-income/other/{nino}/{taxYear}
 */
int mtd_isi_si_o_delete(const char *tax_year, char **buf)
{
	EP(ISI_SI_O_DELETE, NULL, tax_year)
}
