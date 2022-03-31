/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * mtd-ep-bsas.c - Make Tax Digital - Business Source Adjustable Summary API
 *
 * Copyright (C) 2020 - 2022	Andrew Clayton <andrew@digital-domain.net>
 */

#include <stddef.h>

#include "mtd-bsas.h"		/* for default (public) visibility */
#include "endpoints.h"

#define VERSION		"3.0"
#define API_VER		"Accept: application/vnd.hmrc." VERSION "+json"

/*
 * [POST]
 * /individuals/self-assessment/adjustable-summary/{nino}/foreign-property/{calculationId}/adjust
 */
int mtd_bsas_fp_update_summary_adjustments(const struct mtd_dsrc_ctx *dsctx,
					   const char *cid, char **buf)
{
	return do_ep(BSAS_FP_UPDATE_SUMMARY_ADJUSTMENTS, API_VER,
		     dsctx, buf, cid, (char *)NULL);
}

/*
 * [GET ]
 * /individuals/self-assessment/adjustable-summary/{nino}/foreign-property/{calculationId}
 */
int mtd_bsas_fp_get_summary(const char *cid, char **buf)
{
	return do_ep(BSAS_FP_GET_SUMMARY, API_VER,
		     NULL, buf, cid, (char *)NULL);
}

/*
 * [POST]
 * /individuals/self-assessment/adjustable-summary/{nino}/property/{calculationId}/adjust
 */
int mtd_bsas_pb_update_summary_adjustments(const struct mtd_dsrc_ctx *dsctx,
					   const char *cid, char **buf)
{
	return do_ep(BSAS_PB_UPDATE_SUMMARY_ADJUSTMENTS, API_VER,
		     dsctx, buf, cid, (char *)NULL);
}

/*
 * [GET ]
 * /individuals/self-assessment/adjustable-summary/{nino}/uk-property/{calculationId}
 */
int mtd_bsas_pb_get_summary(const char *cid, char **buf)
{
	return do_ep(BSAS_PB_GET_SUMMARY, API_VER,
		     NULL, buf, cid, (char *)NULL);
}

/*
 * [POST]
 * /individuals/self-assessment/adjustable-summary/{nino}/self-employment/{calculationId}/adjust
 */
int mtd_bsas_se_update_summary_adjustments(const struct mtd_dsrc_ctx *dsctx,
					   const char *cid, char **buf)
{
	return do_ep(BSAS_SE_UPDATE_SUMMARY_ADJUSTMENTS, API_VER,
		     dsctx, buf, cid, (char *)NULL);
}

/*
 * [GET ]
 * /individuals/self-assessment/adjustable-summary/{nino}/self-employment/{calculationId}
 */
int mtd_bsas_se_get_summary(const char *cid, char **buf)
{
	return do_ep(BSAS_SE_GET_SUMMARY, API_VER,
		     NULL, buf, cid, (char *)NULL);
}

/*
 * [POST]
 * /individuals/self-assessment/adjustable-summary/{nino}/trigger
 */
int mtd_bsas_trigger_summary(const struct mtd_dsrc_ctx *dsctx, char **buf)
{
	return do_ep(BSAS_TRIGGER_SUMMARY, API_VER,
		     dsctx, buf, (char *)NULL);
}

/*
 * [GET ]
 * /individuals/self-assessment/adjustable-summary/{nino}[?[businessId=][&[typeOfBusiness={self-employment,uk-property-non-fhl,uk-property-fhl,foreign-property-fhl-eea,foreign-property}][&[taxYear=YYYY-YY]]]]
 */
int mtd_bsas_list_summaries(const char *query_string, char **buf)
{
	return do_ep(BSAS_LIST_SUMMARIES, API_VER,
		     NULL, buf, query_string, (char *)NULL);
}
