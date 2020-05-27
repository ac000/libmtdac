/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * mtd-il.c - Make Tax Digital - Individual Loses API
 *
 * Copyright (C) 2020		Andrew Clayton <andrew@digital-domain.net>
 */

#include "mtd-il.h"		/* for default (public) visibility */
#include "endpoints.h"
#include "curler.h"

#define API_VER			"Accept: application/vnd.hmrc.1.0+json"

/*
 * [PUT ]
 * /individuals/losses/{nino}/loss-claims/order
 */
int mtd_il_lc_update_loss_order(const char *src_file, char **buf)
{
	struct curl_ctx ctx = { 0 };

	ctx.mtd_api_ver = API_VER;
	ctx.endpoint = IL_LC_UPDATE_LOSS_ORDER;

	return do_put(&ctx, src_file, NULL, buf);
}

/*
 * [POST]
 * /individuals/losses/{nino}/loss-claims/{claimId}/change-type-of-claim
 */
int mtd_il_lc_update_loss_type(const char *src_file, const char *cid,
			       char **buf)
{
	struct curl_ctx ctx = { 0 };
	const char *params[] = { cid };

	ctx.mtd_api_ver = API_VER;
	ctx.endpoint = IL_LC_UPDATE_LOSS_TYPE;
	ctx.params = params;

	return do_post(&ctx, src_file, NULL, buf);
}

/*
 * [DELETE]
 * /individuals/losses/{nino}/loss-claims/{claimId}
 */
int mtd_il_lc_delete_loss(const char *cid, char **buf)
{
	struct curl_ctx ctx = { 0 };
	const char *params[] = { cid };

	ctx.mtd_api_ver = API_VER;
	ctx.endpoint = IL_LC_DELETE_LOSS;
	ctx.params = params;

	return do_delete(&ctx, buf);
}

/*
 * [GET ]
 * /individuals/losses/{nino}/loss-claims/{claimId}
 */
int mtd_il_lc_get_loss(const char *cid, char **buf)
{
	struct curl_ctx ctx = { 0 };
	const char *params[] = { cid };

	ctx.mtd_api_ver = API_VER;
	ctx.endpoint = IL_LC_GET_LOSS;
	ctx.params = params;

	return do_get(&ctx, buf);
}

/*
 * [POST]
 * /individuals/losses/{nino}/loss-claims
 */
int mtd_il_lc_create_loss(const char *src_file, char **buf)
{
	struct curl_ctx ctx = { 0 };

	ctx.mtd_api_ver = API_VER;
	ctx.endpoint = IL_LC_CREATE_LOSS;

	return do_post(&ctx, src_file, NULL, buf);
}

/*
 * [GET ]
 * /individuals/losses/{nino}/loss-claims
 *
 * Optional query string:
 *
 *	?selfEmploymentId=&taxYear=YYYY-YY&typeOfLoss=&claimType=carry-sideways
 *
 *	typeOfLoss={self-employment,uk-property-fhl,uk-property-non-fhl}
 */
int mtd_il_lc_list_loses(const char *query_string, char **buf)
{
	struct curl_ctx ctx = { 0 };
	const char *params[] = { query_string };

	ctx.mtd_api_ver = API_VER;
	ctx.endpoint = IL_LC_LIST_LOSES;
	ctx.params = params;

	return do_get(&ctx, buf);
}

/*
 * [POST]
 * /individuals/losses/{nino}/brought-forward-losses/{lossId}/change-loss-amount
 */
int mtd_il_bf_update_loss_amnt(const char *src_file, const char *lid,
			       char **buf)
{
	struct curl_ctx ctx = { 0 };
	const char *params[] = { lid };

	ctx.mtd_api_ver = API_VER;
	ctx.endpoint = IL_BF_UPDATE_LOSS_AMNT;
	ctx.params = params;

	return do_post(&ctx, src_file, NULL, buf);
}

/*
 * [DELETE]
 * /individuals/losses/{nino}/brought-forward-losses/{lossId}
 */
int mtd_il_bf_delete_loss(const char *lid, char **buf)
{
	struct curl_ctx ctx = { 0 };
	const char *params[] = { lid };

	ctx.mtd_api_ver = API_VER;
	ctx.endpoint = IL_BF_DELETE_LOSS;
	ctx.params = params;

	return do_delete(&ctx, buf);
}

/*
 * [GET ]
 * /individuals/losses/{nino}/brought-forward-losses/{lossId}
 */
int mtd_il_bf_get_loss(const char *lid, char **buf)
{
	struct curl_ctx ctx = { 0 };
	const char *params[] = { lid };

	ctx.mtd_api_ver = API_VER;
	ctx.endpoint = IL_BF_GET_LOSS;
	ctx.params = params;

	return do_get(&ctx, buf);
}

/*
 * [POST]
 * /individuals/losses/{nino}/brought-forward-losses
 */
int mtd_il_bf_create_loss(const char *src_file, char **buf)
{
	struct curl_ctx ctx = { 0 };

	ctx.mtd_api_ver = API_VER;
	ctx.endpoint = IL_BF_CREATE_LOSS;

	return do_post(&ctx, src_file, NULL, buf);
}

/*
 * [GET ]
 * /individuals/losses/{nino}/brought-forward-losses
 *
 * Optional query string:
 *
 *	?selfEmploymentId=&taxYear=YYYY-YY&typeOfLoss=
 *
 *	typeOfLoss={self-employment,uk-property-fhl,uk-property-non-fhl}
 */
int mtd_il_bf_list_loses(const char *query_string, char **buf)
{
	struct curl_ctx ctx = { 0 };
	const char *params[] = { query_string };

	ctx.mtd_api_ver = API_VER;
	ctx.endpoint = IL_BF_LIST_LOSES;
	ctx.params = params;

	return do_get(&ctx, buf);
}
