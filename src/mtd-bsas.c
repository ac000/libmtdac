/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * mtd-bsas.c - Make Tax Digital - Business Source Adjustable Summary API
 *
 * Copyright (C) 2020		Andrew Clayton <andrew@digital-domain.net>
 */

#include <stddef.h>

#include "mtd-bsas.h"		/* for default (public) visibility */
#include "endpoints.h"

#define API_VER			"Accept: application/vnd.hmrc.1.0+json"

/*
 * [POST]
 * /individuals/self-assessment/adjustable-summary/{nino}/property/{bsasId}/adjust
 */
int mtd_bsas_pb_update_summary_adjustments(const char *src_file,
					   const char *bid, char **buf)
{
	return do_ep(BSAS_PB_UPDATE_SUMMARY_ADJUSTMENTS, API_VER,
		     src_file, NULL, buf, bid, (char *)NULL);
}
/*
 * [GET ]
 * /individuals/self-assessment/adjustable-summary/{nino}/property/{bsasId}/adjust
 */
int mtd_bsas_pb_list_summary_adjustments(const char *bid, char **buf)
{
	return do_ep(BSAS_PB_LIST_SUMMARY_ADJUSTMENTS, API_VER,
		     NULL, NULL, buf, bid, (char *)NULL);
}

/*
 * [GET ]
 * /individuals/self-assessment/adjustable-summary/{nino}/property/{bsasId}[?adjustedStatus={true,false}]
 */
int mtd_bsas_pb_get_summary(const char *bid, const char *query_string,
			    char **buf)
{
	return do_ep(BSAS_PB_GET_SUMMARY, API_VER,
		     NULL, NULL, buf, bid, query_string, (char *)NULL);
}

/*
 * [POST]
 * /individuals/self-assessment/adjustable-summary/{nino}/self-employment/{bsasId}/adjust
 */
int mtd_bsas_se_update_summary_adjustments(const char *src_file,
					   const char *bid, char **buf)
{
	return do_ep(BSAS_SE_UPDATE_SUMMARY_ADJUSTMENTS, API_VER,
		     src_file, NULL, buf, bid, (char *)NULL);
}
/*
 * [GET ]
 * /individuals/self-assessment/adjustable-summary/{nino}/self-employment/{bsasId}/adjust
 */
int mtd_bsas_se_list_summary_adjustments(const char *bid, char **buf)
{
	return do_ep(BSAS_SE_LIST_SUMMARY_ADJUSTMENTS, API_VER,
		     NULL, NULL, buf, bid, (char *)NULL);
}

/*
 * [GET ]
 * /individuals/self-assessment/adjustable-summary/{nino}/self-employment/{bsasId}[?adjustedStatus={true,false}]
 */
int mtd_bsas_se_get_summary(const char *bid, const char *query_string,
			    char **buf)
{
	return do_ep(BSAS_SE_GET_SUMMARY, API_VER,
		     NULL, NULL, buf, bid, query_string, (char *)NULL);
}

/*
 * [POST]
 * /individuals/self-assessment/adjustable-summary/{nino}/trigger
 */
int mtd_bsas_trigger_summary(const char *src_file, char **buf)
{
	return do_ep(BSAS_TRIGGER_SUMMARY, API_VER,
		     src_file, NULL, buf, (char *)NULL);
}

/*
 * [GET ]
 * /individuals/self-assessment/adjustable-summary/{nino}[?[selfEmploymentId=][&[typeOfBusiness={self-employment,uk-property-non-fhl,uk-property-fhl}][&[taxYear=YYYY-YY]]]]
 */
int mtd_bsas_list_summaries(const char *query_string, char **buf)
{
	return do_ep(BSAS_LIST_SUMMARIES, API_VER,
		     NULL, NULL, buf, query_string, (char *)NULL);
}
