/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * mtd-priv.h - Make Tax Digital
 *
 * Copyright (C) 2020 - 2021	Andrew Clayton <andrew@digital-domain.net>
 */

#ifndef _MTD_PRIV_H_
#define _MTD_PRIV_H_

#include <jansson.h>

#include "mtd.h"

#ifndef __unused
#define __unused	__attribute__((unused))
#endif

enum app_conn_type {
	MTD_ACT_MOBILE_APP_DIRECT = 0,
	MTD_ACT_DESKTOP_APP_DIRECT,
	MTD_ACT_MOBILE_APP_VIA_SERVER,
	MTD_ACT_DESKTOP_APP_VIA_SERVER,
	MTD_ACT_WEB_APP_VIA_SERVER,
	MTD_ACT_BATCH_PROCESS_DIRECT,
	MTD_ACT_OTHER_DIRECT,
	MTD_ACT_OTHER_VIA_SERVER,
};

enum log_level {
	MTD_LOG_ERRNO = 0,
	MTD_LOG_ERR,
	MTD_LOG_INFO,
	MTD_LOG_DEBUG_DATA,
	MTD_LOG_DEBUG_ALL,
};

struct mtd_ctx {
	int opts;
	enum app_conn_type app_conn_type;
	enum log_level log_level;
	const char *config_dir;
	const char *api_url;

	const struct mtd_cfg *cfg;
};

extern int write_config(const char *dir, const char *name, const json_t *json);

#endif /* _MTD_PRIV_H_ */
