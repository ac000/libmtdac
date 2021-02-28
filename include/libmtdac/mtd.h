/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * mtd.h - Make Tax Digital
 *
 * Copyright (C) 2020 - 2021	Andrew Clayton <andrew@digital-domain.net>
 */

#ifndef _MTD_H_
#define _MTD_H_

#include <stdio.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define LIBMTDAC_MAJOR_VERSION		 0
#define LIBMTDAC_MINOR_VERSION		18
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

#define MTD_OPT_NO_ANTI_FRAUD_HDRS		0x10000000

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
	MTD_OPT_NO_ANTI_FRAUD_HDRS | \
	MTD_OPT_GLOBAL_INIT | \
	MTD_OPT_PRODUCTION_API

enum mtd_error {
	MTD_ERR_NONE = 0,
	MTD_ERR_OS,
	MTD_ERR_REQUEST,
	MTD_ERR_CURL,
	MTD_ERR_NEEDS_AUTHORISATION,
	MTD_ERR_UNKNOWN_FLAGS,
	MTD_ERR_LIB_TOO_OLD,
	MTD_ERR_CONFIG_DIR_UNSPEC,
	MTD_ERR_CONFIG_DIR_INVALID,
	MTD_ERR_NO_CONFIG,

	/* keep this last */
	MTD_ERR_INVALID_ERROR
};

/* Just the HTTP status codes relevant to the MTD API */
enum mtd_http_status_code {
	MTD_HTTP_OK = 200,
	MTD_HTTP_CREATED,
	MTD_HTTP_ACCEPTED,
	MTD_HTTP_NO_CONTENT = 204,
	MTD_HTTP_SEE_OTHER = 303,
	MTD_HTTP_BAD_REQUEST = 400,
	MTD_HTTP_UNAUTHORIZED,
	MTD_HTTP_FORBIDDEN = 403,
	MTD_HTTP_NOT_FOUND,
	MTD_HTTP_METHOD_NOT_ALLOWED,
	MTD_HTTP_NOT_ACCEPTABLE,
	MTD_HTTP_GONE = 410,
	MTD_HTTP_REQUEST_ENTITY_TOO_LARGE = 413,
	MTD_HTTP_UNSUPPORTED_MEDIA_TYPE = 415,
	MTD_HTTP_TOO_MANY_REQUESTS = 429,
	MTD_HTTP_INTERNAL_SERVER_ERROR = 500,
	MTD_HTTP_NOT_IMPLEMENTED,
	MTD_HTTP_SERVICE_UNAVAILABLE = 503,
	MTD_HTTP_GATEWAY_TIMEOUT,
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

#define MTD_FPH_SET_FUNC(s, m, f)	s.m = f

#define MTD_FPH_CLI_PUBLIC_IP			fph_srcip
#define MTD_FPH_CLI_PUBLIC_PORT			fph_srcport
#define MTD_FPH_CLI_DEV_ID			fph_device_id
#define MTD_FPH_CLI_USER_ID			fph_user
#define MTD_FPH_CLI_TZ				fph_tz
#define MTD_FPH_CLI_LOCAL_IPS			fph_ipaddrs
#define MTD_FPH_CLI_MAC_ADDRS			fph_macaddrs
#define MTD_FPH_CLI_UA				fph_ua
#define MTD_FPH_CLI_MULTI_FACTOR		fph_multi_factor
#define MTD_FPH_CLI_SCREENS			fph_screens
#define MTD_FPH_CLI_WINDOW_SZ			fph_window_sz
#define MTD_FPH_CLI_BROWSER_PLUGINS		fph_browser_plugins
#define MTD_FPH_CLI_BROWSER_JS_UA		fph_browser_js_ua
#define MTD_FPH_CLI_BROWSER_DNT			fph_browser_dnt
#define MTD_FPH_CLI_LOCAL_IPS_TS		fph_ipaddrs_ts
#define MTD_FPH_CLI_PUBLIC_IP_TS		fph_srcip_ts
#define MTD_FPH_VEN_VERSION			fph_version
#define MTD_FPH_VEN_VERSION_CLI			fph_version_cli
#define MTD_FPH_VEN_LICENSE_ID			fph_license_id
#define MTD_FPH_VEN_PUBLIC_IP			fph_vendor_ip
#define MTD_FPH_VEN_FWD				fph_vendor_fwd
#define MTD_FPH_VEN_PROD_NAME			fph_prod_name

struct mtd_fph_ops {
	char *(*fph_device_id)(void);
	char *(*fph_user)(void);
	char *(*fph_tz)(void);
	char *(*fph_ipaddrs)(void);
	char *(*fph_ipaddrs_ts)(void);
	char *(*fph_macaddrs)(void);
	char *(*fph_srcip)(void);
	char *(*fph_srcip_ts)(void);
	char *(*fph_srcport)(void);
	char *(*fph_screens)(void);
	char *(*fph_window_sz)(void);
	char *(*fph_browser_plugins)(void);
	char *(*fph_browser_js_ua)(void);
	char *(*fph_browser_dnt)(void);
	char *(*fph_vendor_ip)(void);
	char *(*fph_vendor_fwd)(void);
	char *(*fph_ua)(void);
	char *(*fph_multi_factor)(void);
	char *(*fph_license_id)(void);
	char *(*fph_version)(void);
	char *(*fph_version_cli)(void);
	char *(*fph_prod_name)(void);
};

struct mtd_cfg {
	const struct mtd_fph_ops *fph_ops;
	const char * const *extra_hdrs;

	const char *config_dir;
};

#pragma GCC visibility push(default)

extern void mtd_global_init(void);
extern int mtd_init(unsigned int flags, const struct mtd_cfg *cfg);
extern void mtd_deinit(void);
extern int mtd_init_auth(void);
extern int mtd_init_config(void);
extern int mtd_init_nino(void);
extern char *mtd_percent_encode(const char *str, ssize_t len);
extern const char *mtd_err2str(int err);
extern const char *mtd_err2enum_str(int err);
extern enum mtd_http_status_code mtd_http_status_code(const char *json);

#pragma GCC visibility pop

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _MTD_H_ */
