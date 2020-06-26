/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * mtd.h - Make Tax Digital
 *
 * Copyright (C) 2020	Andrew Clayton <andrew@digital-domain.net>
 */

#ifndef _MTD_H_
#define _MTD_H_

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#define LIBMTDAC_MAJOR_VERSION		 0
#define LIBMTDAC_MINOR_VERSION		11
#define LIBMTDAC_MICRO_VERSION		 0

/* MTD_OPT_* - option flags for mtd_init() */
#define MTD_OPT_LOG_ERR				0x00000001
#define MTD_OPT_LOG_INFO			0x00000002
#define MTD_OPT_LOG_DEBUG			0x00000004

/* also update enum app_conn_type in mtd.c */
#define MTD_OPT_ACT_MOBILE_APP_DIRECT		0x00000100
#define MTD_OPT_ACT_DESKTOP_APP_DIRECT		0x00000200
#define MTD_OPT_ACT_MOBILE_APP_VIA_SERVER	0x00000400
#define MTD_OPT_ACT_DESKTOP_APP_VIA_SERVER	0x00000800
#define MTD_OPT_ACT_WEB_APP_VIA_SERVER		0x00001000
#define MTD_OPT_ACT_BATCH_PROCESS_DIRECT	0x00002000
#define MTD_OPT_ACT_OTHER_DIRECT		0x00004000
#define MTD_OPT_ACT_OTHER_VIA_SERVER		0x00008000

#define MTD_OPT_SND_ANTI_FRAUD_HDRS		0x10000000

#define MTD_OPT_GLOBAL_INIT			0x40000000

#define MTD_OPT_PRODUCTION_API			0x80000000

#define MTD_OPT_ALL \
	MTD_OPT_LOG_ERR | \
	MTD_OPT_LOG_INFO | \
	MTD_OPT_LOG_DEBUG | \
	MTD_OPT_ACT_MOBILE_APP_DIRECT | \
	MTD_OPT_ACT_DESKTOP_APP_DIRECT | \
	MTD_OPT_ACT_MOBILE_APP_VIA_SERVER | \
	MTD_OPT_ACT_DESKTOP_APP_VIA_SERVER | \
	MTD_OPT_ACT_WEB_APP_VIA_SERVER | \
	MTD_OPT_ACT_BATCH_PROCESS_DIRECT | \
	MTD_OPT_ACT_OTHER_DIRECT | \
	MTD_OPT_ACT_OTHER_VIA_SERVER | \
	MTD_OPT_SND_ANTI_FRAUD_HDRS | \
	MTD_OPT_GLOBAL_INIT | \
	MTD_OPT_PRODUCTION_API

enum mtd_error {
	MTD_ERR_NONE = 0,
	MTD_ERR_OS,
	MTD_ERR_REQUEST,
	MTD_ERR_CURL,
	MTD_ERR_NEEDS_AUTHORISATION,
	MTD_ERR_UNKNOWN_FLAGS,
};

enum mtd_data_src_type {
	MTD_DATA_SRC_FILE = 0,
	MTD_DATA_SRC_BUF,
	MTD_DATA_SRC_FP,
	MTD_DATA_SRC_FD,
};

typedef union mtd_data_src {
	const void *buf;
	const char *file;
	FILE *fp;
	int fd;
} mtd_data_src_t;

struct mtd_dsrc_ctx {
	mtd_data_src_t data_src;
	size_t data_len;

	enum mtd_data_src_type src_type;
};

#pragma GCC visibility push(default)

extern void mtd_hdrs_add(const char * const hdrs[]);
extern void mtd_hdrs_reset(void);
extern void mtd_global_init(void);
extern int mtd_init(int flags);
extern void mtd_deinit(void);
extern int mtd_init_auth(void);
extern int mtd_init_config(void);
extern int mtd_init_nino(void);

#pragma GCC visibility pop

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _MTD_H_ */
