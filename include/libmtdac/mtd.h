/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * mtd.h - Make Tax Digital
 *
 * Copyright (C) 2020	Andrew Clayton <andrew@digital-domain.net>
 */

#ifndef _MTD_H_
#define _MTD_H_

#ifdef __cplusplus
extern "C" {
#endif

#define LIBMTDAC_MAJOR_VERSION		0
#define LIBMTDAC_MINOR_VERSION		0
#define LIBMTDAC_MICRO_VERSION		0

#define MTD_CONFIG_DIR_FMT		"%s/.config/libmtdac/%s"

/* Option flags for mtd_init() */
#define MTD_OPT_UI_CLI		0x01
#define MTD_OPT_UI_GUI		0x02
#define MTD_OPT_LOG_ERR		0x04
#define MTD_OPT_LOG_INFO	0x08
#define MTD_OPT_LOG_DEBUG	0x10

enum mtd_error {
	MTD_ERR_NONE = 0,
	MTD_ERR_OS,
	MTD_ERR_REQUEST,
	MTD_ERR_CURL,
	MTD_ERR_MISSING_CONFIG,
	MTD_ERR_NEEDS_AUTHORISATION,
	MTD_ERR_UNKNOWN_FLAGS,
};

#pragma GCC visibility push(default)

extern int mtd_init(int flags);
extern int mtd_init_auth(void);
extern int mtd_init_config(void);

#pragma GCC visibility pop

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _MTD_H_ */
