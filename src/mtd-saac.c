/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * mtd-saac.c - Make Tax Digital - Self Assessment Accounts API
 *
 * Copyright (C) 2020		Andrew Clayton <andrew@digital-domain.net>
 */

#include <stddef.h>

#include "mtd-saac.h"		/* for default (public) visibility */
#include "endpoints.h"

#define API_VER			"Accept: application/vnd.hmrc.1.0+json"

/*
 * [GET ]
 * /accounts/self-assessment/{nino}/payments/{paymentId}
 */
int mtd_saac_get_payment(const char *pyid, char **buf)
{
	return do_ep(SAAC_GET_PAYMENT, API_VER,
		     NULL, NULL, buf, pyid, (char *)NULL);
}

/*
 * [GET ]
 * /accounts/self-assessment/{nino}/payments?from=YYYY-MM-DD&to=YYYY-MM-DD
 */
int mtd_saac_list_payments(const char *query_string, char **buf)
{
	return do_ep(SAAC_LIST_PAYMENTS, API_VER,
		     NULL, NULL, buf, query_string, (char *)NULL);
}

/*
 * [GET ]
 * /accounts/self-assessment/{nino}/charges/{transactionId}
 */
int mtd_saac_get_charge(const char *trid, char **buf)
{
	return do_ep(SAAC_GET_CHARGE, API_VER,
		     NULL, NULL, buf, trid, (char *)NULL);
}

/*
 * [GET ]
 * /accounts/self-assessment/{nino}/charges?from=YYYY-MM-DD&to=YYYY-MM-DD
 */
int mtd_saac_list_charges(const char *query_string, char **buf)
{
	return do_ep(SAAC_LIST_CHARGES, API_VER,
		     NULL, NULL, buf, query_string, (char *)NULL);
}

/*
 * [GET ]
 * /accounts/self-assessment/{nino}/transactions/{transactionId}
 */
int mtd_saac_get_transaction(const char *trid, char **buf)
{
	return do_ep(SAAC_GET_TRANSACTION, API_VER,
		     NULL, NULL, buf, trid, (char *)NULL);
}

/*
 * [GET ]
 * /accounts/self-assessment/{nino}/transactions?from=YYYY-MM-DD&to=YYYY-MM-DD
 */
int mtd_saac_list_transactions(const char *query_string, char **buf)
{
	return do_ep(SAAC_LIST_TRANSACTIONS, API_VER,
		     NULL, NULL, buf, query_string, (char *)NULL);
}

/*
 * [GET ]
 * /accounts/self-assessment/{nino}/balance
 */
int mtd_saac_get_balance(char **buf)
{
	return do_ep(SAAC_GET_BALANCE, API_VER,
		     NULL, NULL, buf, (char *)NULL);
}
