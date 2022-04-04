/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * mtd-ep-il.c - Make Tax Digital - Individual Loses API
 *
 * Copyright (C) 2020 - 2022	Andrew Clayton <andrew@digital-domain.net>
 */

#include <stddef.h>

#include "mtd-il.h"		/* for default (public) visibility */
#include "endpoints.h"

#define VERSION		"3.0"
#define API_VER		"Accept: application/vnd.hmrc." VERSION "+json"

/*
 * [PUT ]
 * /individuals/losses/{nino}/loss-claims/order/{taxYearClaimedFor}
 */
int mtd_il_lc_update_loss_order(const struct mtd_dsrc_ctx *dsctx,
				const char *tax_year, char **buf)
{
	return do_ep(IL_LC_UPDATE_LOSS_ORDER, API_VER,
		     dsctx, buf, tax_year, (char *)NULL);
}

/*
 * [POST]
 * /individuals/losses/{nino}/loss-claims/{claimId}/change-type-of-claim
 */
int mtd_il_lc_update_loss_type(const struct mtd_dsrc_ctx *dsctx,
			       const char *cid, char **buf)
{
	return do_ep(IL_LC_UPDATE_LOSS_TYPE, API_VER,
		     dsctx, buf, cid, (char *)NULL);
}

/*
 * [DELETE]
 * /individuals/losses/{nino}/loss-claims/{claimId}
 */
int mtd_il_lc_delete_loss(const char *cid, char **buf)
{
	return do_ep(IL_LC_DELETE_LOSS, API_VER,
		     NULL, buf, cid, (char *)NULL);
}

/*
 * [GET ]
 * /individuals/losses/{nino}/loss-claims/{claimId}
 */
int mtd_il_lc_get_loss(const char *cid, char **buf)
{
	return do_ep(IL_LC_GET_LOSS, API_VER,
		     NULL, buf, cid, (char *)NULL);
}

/*
 * [POST]
 * /individuals/losses/{nino}/loss-claims
 */
int mtd_il_lc_create_loss(const struct mtd_dsrc_ctx *dsctx, char **buf)
{
	return do_ep(IL_LC_CREATE_LOSS, API_VER,
		     dsctx, buf, (char *)NULL);
}

/*
 * [GET ]
 * /individuals/losses/{nino}/loss-claims
 *
 * Optional query string:
 *
 *	?businessId=&taxYearClaimedFor=YYYY-YY&typeOfLoss=&typeOfClaim=carry-sideways
 *
 *	typeOfLoss={self-employment,uk-property-non-fhl,foreign-property}
 */
int mtd_il_lc_list_loses(const char *query_string, char **buf)
{
	return do_ep(IL_LC_LIST_LOSES, API_VER,
		     NULL, buf, query_string, (char *)NULL);
}

/*
 * [POST]
 * /individuals/losses/{nino}/brought-forward-losses/{lossId}/change-loss-amount
 */
int mtd_il_bf_update_loss_amnt(const struct mtd_dsrc_ctx *dsctx,
			       const char *lid, char **buf)
{
	return do_ep(IL_BF_UPDATE_LOSS_AMNT, API_VER,
		     dsctx, buf, lid, (char *)NULL);
}

/*
 * [DELETE]
 * /individuals/losses/{nino}/brought-forward-losses/{lossId}
 */
int mtd_il_bf_delete_loss(const char *lid, char **buf)
{
	return do_ep(IL_BF_DELETE_LOSS, API_VER,
		     NULL, buf, lid, (char *)NULL);
}

/*
 * [GET ]
 * /individuals/losses/{nino}/brought-forward-losses/{lossId}
 */
int mtd_il_bf_get_loss(const char *lid, char **buf)
{
	return do_ep(IL_BF_GET_LOSS, API_VER,
		     NULL, buf, lid, (char *)NULL);
}

/*
 * [POST]
 * /individuals/losses/{nino}/brought-forward-losses
 */
int mtd_il_bf_create_loss(const struct mtd_dsrc_ctx *dsctx, char **buf)
{
	return do_ep(IL_BF_CREATE_LOSS, API_VER,
		     dsctx, buf, (char *)NULL);
}

/*
 * [GET ]
 * /individuals/losses/{nino}/brought-forward-losses
 *
 * Optional query string:
 *
 *	?businessId=&taxYearBroughtForwardFrom=YYYY-YY&typeOfLoss=
 *
 *	typeOfLoss={self-employment,uk-property-fhl,uk-property-non-fhl,
 *		    foreign-property-fhl-eea,foreign-property}
 */
int mtd_il_bf_list_loses(const char *query_string, char **buf)
{
	return do_ep(IL_BF_LIST_LOSES, API_VER,
		     NULL, buf, query_string, (char *)NULL);
}
