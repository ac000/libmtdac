/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * fph.c - Make Tax Digital - Fraud Prevention Headers
 *
 * Copyright (C) 2020 - 2025	Andrew Clayton <ac@sigsegv.uk>
 */

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <sys/utsname.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <limits.h>

#include <jansson.h>

#include "platform.h"
#include "mtd.h"
#include "mtd-priv.h"
#include "curler.h"
#include "logger.h"
#include "fph.h"

#define BUF_SZ	1024

enum fph_hdr {
	FPH_C_PUBLIC_IP		= 0x1,
	FPH_C_PUBLIC_PORT	= 0x2,
	FPH_C_DEV_ID		= 0x4,
	FPH_C_USER_ID		= 0x8,
	FPH_C_TZ		= 0x10,
	FPH_C_LOCAL_IPS		= 0x20,
	FPH_C_MAC_ADDRS		= 0x40,
	FPH_C_UA		= 0x80,
	FPH_C_MULTI_FACTOR	= 0x100,
	FPH_C_SCREENS		= 0x200,
	FPH_C_WINDOW_SZ		= 0x400,
	FPH_C_BROWSER_PLUGINS	= 0x800,
	FPH_C_BROWSER_JS_UA	= 0x1000,
	FPH_C_BROWSER_DNT	= 0x2000,
	FPH_C_LOCAL_IPS_TS	= 0x4000,
	FPH_C_PUBLIC_IP_TS	= 0x8000,
	FPH_V_VERSION		= 0x100000,
	FPH_V_LICENSE_ID	= 0x200000,
	FPH_V_PUBLIC_IP		= 0x400000,
	FPH_V_FWD		= 0x800000,
	FPH_V_PROD_NAME		= 0x1000000,
};

#define _FPH_COMMON \
	FPH_C_DEV_ID	   | \
	FPH_C_USER_ID	   | \
	FPH_C_TZ	   | \
	FPH_C_LOCAL_IPS	   | \
	FPH_C_LOCAL_IPS_TS | \
	FPH_C_MAC_ADDRS	   | \
	FPH_C_UA	   | \
	FPH_C_MULTI_FACTOR | \
	FPH_V_VERSION	   | \
	FPH_V_LICENSE_ID   | \
	FPH_V_PROD_NAME

#define _FPH_COMMON_DESKTOP \
	_FPH_COMMON	| \
	FPH_C_SCREENS	| \
	FPH_C_WINDOW_SZ

#define FPH_DESKTOP_APP_DIRECT		_FPH_COMMON_DESKTOP

#define FPH_DESKTOP_APP_VIA_SERVER \
	FPH_C_PUBLIC_IP	    | \
	FPH_C_PUBLIC_IP_TS  | \
	FPH_C_PUBLIC_PORT   | \
	_FPH_COMMON_DESKTOP | \
	FPH_V_PUBLIC_IP	    | \
	FPH_V_FWD

#define FPH_WEB_APP_VIA_SERVER \
	((FPH_DESKTOP_APP_VIA_SERVER) & ~FPH_C_MAC_ADDRS & ~FPH_C_UA)	| \
	FPH_C_BROWSER_PLUGINS						| \
	FPH_C_BROWSER_JS_UA						| \
	FPH_C_BROWSER_DNT

#define FPH_MOBILE_APP_DIRECT		FPH_DESKTOP_APP_DIRECT

#define FPH_MOBILE_APP_VIA_SERVER	FPH_DESKTOP_APP_VIA_SERVER

#define FPH_BATCH_PROCESS_DIRECT \
	(_FPH_COMMON) & ~FPH_C_MULTI_FACTOR

#define FPH_OTHER_DIRECT		_FPH_COMMON

#define FPH_OTHER_VIA_SERVER \
	FPH_C_PUBLIC_IP	   | \
	FPH_C_PUBLIC_IP_TS | \
	FPH_C_PUBLIC_PORT  | \
	_FPH_COMMON	   | \
	FPH_V_PUBLIC_IP	   | \
	FPH_V_FWD

static const struct {
	unsigned int fph;
	const char *str;
} fph_type_map[] = {
	[MTD_ACT_MOBILE_APP_DIRECT] = {
		.fph	= FPH_MOBILE_APP_DIRECT,
		.str	= "MOBILE_APP_DIRECT"
	},
	[MTD_ACT_DESKTOP_APP_DIRECT] = {
		.fph	= FPH_DESKTOP_APP_DIRECT,
		.str	= "DESKTOP_APP_DIRECT"
	},
	[MTD_ACT_MOBILE_APP_VIA_SERVER] = {
		.fph	= FPH_MOBILE_APP_VIA_SERVER,
		.str	= "MOBILE_APP_VIA_SERVER"
	},
	[MTD_ACT_DESKTOP_APP_VIA_SERVER] = {
		.fph	= FPH_DESKTOP_APP_VIA_SERVER,
		.str	= "DESKTOP_APP_VIA_SERVER"
	},
	[MTD_ACT_WEB_APP_VIA_SERVER] = {
		.fph	= FPH_WEB_APP_VIA_SERVER,
		.str	= "WEB_APP_VIA_SERVER"
	},
	[MTD_ACT_BATCH_PROCESS_DIRECT] = {
		.fph	= FPH_BATCH_PROCESS_DIRECT,
		.str	= "BATCH_PROCESS_DIRECT"
	},
	[MTD_ACT_OTHER_DIRECT] = {
		.fph	= FPH_OTHER_DIRECT,
		.str	= "OTHER_DIRECT"
	},
	[MTD_ACT_OTHER_VIA_SERVER] = {
		.fph	= FPH_OTHER_VIA_SERVER,
		.str	= "OTHER_VIA_SERVER"
	},
};

#define FPH_HINF(h, s)	{ .hdr = h, .str = s }
static const struct {
	enum fph_hdr hdr;
	const char *str;
} fph_hdr_map[] = {
	FPH_HINF(FPH_C_PUBLIC_IP, "Gov-Client-Public-IP"),
	FPH_HINF(FPH_C_PUBLIC_PORT, "Gov-Client-Public-Port"),
	FPH_HINF(FPH_C_DEV_ID, "Gov-Client-Device-ID"),
	FPH_HINF(FPH_C_USER_ID, "Gov-Client-User-IDs"),
	FPH_HINF(FPH_C_TZ, "Gov-Client-Timezone"),
	FPH_HINF(FPH_C_LOCAL_IPS, "Gov-Client-Local-IPs"),
	FPH_HINF(FPH_C_MAC_ADDRS, "Gov-Client-MAC-Addresses"),
	FPH_HINF(FPH_C_UA, "Gov-Client-User-Agent"),
	FPH_HINF(FPH_C_MULTI_FACTOR, "Gov-Client-Multi-Factor"),
	FPH_HINF(FPH_C_SCREENS, "Gov-Client-Screens"),
	FPH_HINF(FPH_C_WINDOW_SZ, "Gov-Client-Window-Size"),
	FPH_HINF(FPH_C_BROWSER_PLUGINS, "Gov-Client-Browser-Plugins"),
	FPH_HINF(FPH_C_BROWSER_JS_UA, "Gov-Client-Browser-JS-User-Agent"),
	FPH_HINF(FPH_C_BROWSER_DNT, "Gov-Client-Browser-Do-Not-Track"),
	FPH_HINF(FPH_C_LOCAL_IPS_TS, "Gov-Client-Local-IPs-Timestamp"),
	FPH_HINF(FPH_C_PUBLIC_IP_TS, "Gov-Client-Public-IP-Timestamp"),
	FPH_HINF(FPH_V_VERSION, "Gov-Vendor-Version"),
	FPH_HINF(FPH_V_LICENSE_ID, "Gov-Vendor-License-IDs"),
	FPH_HINF(FPH_V_PUBLIC_IP, "Gov-Vendor-Public-IP"),
	FPH_HINF(FPH_V_FWD, "Gov-Vendor-Forwarded"),
	FPH_HINF(FPH_V_PROD_NAME, "Gov-Vendor-Product-Name"),
};

static __thread struct mtd_fph_ops fph_ops;

extern __thread struct mtd_ctx mtd_ctx;

static char *get_license_id(void *user_data __unused)
{
	return NULL;
}

static char *get_multi_factor(void *user_data __unused)
{
	return NULL;
}

static char *get_screens(void *user_data __unused)
{
	return NULL;
}

static char *get_window_sz(void *user_data __unused)
{
	return NULL;
}

static char *get_browser_plugins(void *user_data __unused)
{
	return NULL;
}

static char *get_browser_js_ua(void *user_data __unused)
{
	return NULL;
}

static char *get_browser_dnt(void *user_data __unused)
{
	return NULL;
}

static char *get_prod_name(void *user_data __unused)
{
	return NULL;
}

static char *get_version(void *user_data)
{
	char ver[128];
	char *encname;
	char *encver;
	char *ver_cli = NULL;
	char *buf;
	int err;

	encname = mtd_percent_encode(LIBNAME, -1);
	snprintf(ver, sizeof(ver), "%d.%d.%d (%s)",
		 LIBMTDAC_MAJOR_VERSION, LIBMTDAC_MINOR_VERSION,
		 LIBMTDAC_MICRO_VERSION, GIT_VERSION + 1);
	encver = mtd_percent_encode(ver, -1);

	if (fph_ops.fph_version_cli)
		ver_cli = fph_ops.fph_version_cli(user_data);

	err = asprintf(&buf, "%s%s%s=%s", ver_cli ? ver_cli : "",
		       ver_cli ? "&" : "", encname, encver);
	if (err == -1) {
		logger(MTD_LOG_ERRNO, "asprintf:");
		buf = NULL;
	}

	free(encname);
	free(encver);
	free(ver_cli);

	return buf;
}

static char *get_ua(void *user_data __unused)
{
	struct utsname un;
	char *buf;
	char ven[64];
	char model[128];
	char *encsys;
	char *encrel;
	char *encvendor;
	char *encmodel;
	int err;

	uname(&un);
	encsys = mtd_percent_encode(un.sysname, -1);
	encrel = mtd_percent_encode(un.release, -1);

	get_mach_info(ven, sizeof(ven), model, sizeof(model));
	encvendor = mtd_percent_encode(ven, -1);
	encmodel = mtd_percent_encode(model, -1);

	err = asprintf(&buf,
		       "os-family=%s&os-version=%s&device-manufacturer=%s&device-model=%s",
		       encsys, encrel, encvendor, encmodel);
	if (err == -1) {
		logger(MTD_LOG_ERRNO, "asprintf:");
		buf = NULL;
	}

	free(encsys);
	free(encrel);
	free(encvendor);
	free(encmodel);

	return buf;
}

static char *get_macaddrs(void *user_data __unused)
{
	struct ifaddrs *ifaddr;
	struct ifaddrs *ifa;
	int n;
	int maclen = 0;
	int err;
	char buf[BUF_SZ] = "\0";

	err = getifaddrs(&ifaddr);
	if (err)
		return strdup("");

	for (ifa = ifaddr, n = 0; ifa != NULL; ifa = ifa->ifa_next, n++) {
		char mac[18];
		char *encmac;

		err = get_ll_addr(ifa->ifa_addr, mac, sizeof(mac));
		if (err)
			continue;

		encmac = mtd_percent_encode(mac, -1);
		snprintf(buf + maclen, BUF_SZ - maclen, "%s,", encmac);
		maclen = strlen(buf);
		free(encmac);
	}
	buf[maclen - 1] = '\0';	/* trim trailing ',' */

	freeifaddrs(ifaddr);

	return strdup(buf);
}

struct ip_prefix {
	const char *ip;
	short prefix;
};

static const struct ip_prefix local_ip4s[] = {
	{ "127.0.0.0",		 8 },
	{ "10.0.0.0",		 8 },
	{ "172.16.0.0",		12 },
	{ "169.254.0.0",	16 }, /* link-local */
	{ "192.168.0.0",	16 },
};

static const struct ip_prefix local_ip6s[] = {
	{ "::1",		128 },
	{ "fc00::",		  7 },
	{ "fe80::",		 10 },
};

static bool _ipv4_isin(const char *network, short cidr,
		       const struct in_addr *addr)
{
	struct in_addr ip_addr;
	struct in_addr net_addr;

	inet_pton(AF_INET, network, &net_addr);

	ip_addr.s_addr = addr->s_addr & htonl(~0UL << (32 - cidr));

	return ip_addr.s_addr == net_addr.s_addr;
}

static bool _is_local_ip4(const struct sockaddr *sa)
{
	const struct in_addr *ip_addr = &((struct sockaddr_in *)sa)->sin_addr;
	int nr = sizeof(local_ip4s) / sizeof(local_ip4s[0]);
	int i;

	for (i = 0; i < nr; i++) {
		bool local;

		local = _ipv4_isin(local_ip4s[i].ip, local_ip4s[i].prefix,
				   ip_addr);
		if (local)
			return true;
	}

	return false;
}

static bool _ipv6_isin(const char *network, short prefixlen,
		       const unsigned char *addr)
{
	short i;
	unsigned char mask[sizeof(struct in6_addr)];
	unsigned char net[sizeof(struct in6_addr)];

	inet_pton(AF_INET6, network, net);

	/* Create a mask based on prefixlen */
	for (i = 0; i < 16; i++) {
		short s = (prefixlen > 8) ? 8 : prefixlen;

		prefixlen -= s;
		mask[i] = (0xffu << (8 - s));
	}

	for (i = 0; i < 16; i++) {
		if ((addr[i] & mask[i]) != net[i])
			return false;
	}

	return true;
}

static bool _is_local_ip6(const struct sockaddr *sa)
{
	const unsigned char *ip_addr =
		(unsigned char *)&((struct sockaddr_in6 *)sa)->sin6_addr;
	int nr = sizeof(local_ip6s) / sizeof(local_ip6s[0]);
	int i;

	for (i = 0; i < nr; i++) {
		bool local;

		local = _ipv6_isin(local_ip6s[i].ip, local_ip6s[i].prefix,
				   ip_addr);
		if (local)
			return true;
	}

	return false;
}

static bool _check_is_local_ip(const struct sockaddr *sa, int family)
{
	switch (family) {
	case AF_INET:
		return _is_local_ip4(sa);
	case AF_INET6:
		return _is_local_ip6(sa);
	}

	/*
	 * We should not be able to get to here, but GCC couldn't
	 * seem to work that out...
	 */
	return false;
}

static char *get_ipaddrs(void *user_data __unused)
{
	struct ifaddrs *ifaddr;
	struct ifaddrs *ifa;
	int n;
	int iplen = 0;
	int err;
	char buf[BUF_SZ] = "\0";

	err = getifaddrs(&ifaddr);
	if (err)
		return strdup("");

	for (ifa = ifaddr, n = 0; ifa != NULL; ifa = ifa->ifa_next, n++) {
		int family;
		char *encip;
		char ip[INET6_ADDRSTRLEN];

		if (ifa->ifa_addr == NULL)
			continue;

		family = ifa->ifa_addr->sa_family;
		if (family != AF_INET && family != AF_INET6)
			continue;

		if (!_check_is_local_ip(ifa->ifa_addr, family))
			continue;

		getnameinfo(ifa->ifa_addr,
			    family == AF_INET ? sizeof(struct sockaddr_in) :
						sizeof(struct sockaddr_in6),
			    ip, sizeof(ip), NULL, 0, NI_NUMERICHOST);
		encip = mtd_percent_encode(ip, -1);
		snprintf(buf + iplen, BUF_SZ - iplen, "%s,", encip);
		iplen = strlen(buf);
		free(encip);
	}
	buf[iplen - 1] = '\0';	/* trim trailing ',' */

	freeifaddrs(ifaddr);

	return strdup(buf);
}

static __thread int libcurl_sockfd;
static char *get_src_port(void *user_data __unused)
{
	struct sockaddr_storage ss;
	socklen_t addrlen = sizeof(ss);
	char port[6];

	getsockname(libcurl_sockfd, (struct sockaddr *)&ss, &addrlen);
	if (_check_is_local_ip((struct sockaddr *)&ss, ss.ss_family))
		return NULL;

	getnameinfo((struct sockaddr *)&ss, addrlen, NULL, 0, port,
		    sizeof(port), NI_NUMERICHOST|NI_NUMERICSERV);

	return strdup(port);
}

static char *get_src_addr(void *user_data __unused)
{
	struct sockaddr_storage ss;
	socklen_t addrlen = sizeof(ss);
	char host[INET6_ADDRSTRLEN];

	getsockname(libcurl_sockfd, (struct sockaddr *)&ss, &addrlen);
	if (_check_is_local_ip((struct sockaddr *)&ss, ss.ss_family))
		return NULL;

	getnameinfo((struct sockaddr *)&ss, addrlen, host, sizeof(host), NULL,
		    0, NI_NUMERICHOST|NI_NUMERICSERV);

	return strdup(host);
}

static char *get_ip_ts(void *user_data __unused)
{
	char buf[32];

	/*
	 * The timestamp wants to be in the following format
	 *
	 *     yyyy-MM-ddThh:mm:ss.sssZ
	 *
	 * E.g
	 *
	 *     2020-09-21T10:30:05.123Z
	 */
	gen_datestamp(buf, sizeof(buf));

	return strdup(buf);
}

static char *get_vendor_fwd(void *user_data __unused)
{
	return NULL;
}

static char *get_vendor_ip(void *user_data __unused)
{
	return NULL;
}

static char *get_tz(void *user_data __unused)
{
	time_t now = time(NULL);
	struct tm tm_res;
	const struct tm *tm = localtime_r(&now, &tm_res);
	char *buf;

	buf = malloc(strlen("UTC+HH:MM") + 1);
	if (!buf) {
		logger(MTD_LOG_ERRNO, "malloc:");
		return NULL;
	}

	/*
	 * TZ format wants to be UTC+/-HH:MM
	 * %z gives +/-HHMM
	 */
	strftime(buf, BUF_SZ, "UTC%z", tm);
	buf[7] = buf[6];
	buf[8] = buf[7];
	buf[6] = ':';
	buf[9] = '\0';

	return buf;
}

static char *get_user(void *user_data __unused)
{
	char *encuser;
	char *buf;
	int err;

	encuser = mtd_percent_encode(getenv("USER"), -1);
	err = asprintf(&buf, "os=%s", encuser);
	if (err == -1) {
		logger(MTD_LOG_ERRNO, "asprintf:");
		buf = NULL;
	}
	free(encuser);

	return buf;
}

static char *get_device_id(void *user_data __unused)
{
	char path[PATH_MAX];
	json_t *root;
	json_t *did;
	char *buf;
	int err;

	snprintf(path, sizeof(path), "%s/uuid.json", mtd_ctx.config_dir);
	root = json_load_file(path, 0, NULL);
	if (!root)
		return NULL;
	did = json_object_get(root, "device_id");
	if (!did)
		return NULL;

	err = asprintf(&buf, "%s", json_string_value(did));
	if (err == -1) {
		logger(MTD_LOG_ERRNO, "asprintf:");
		buf = NULL;
	}
	json_decref(root);

	return buf;
}

static char *(*lookup_fph_func(const enum fph_hdr hdr))(void *user_data)
{
	const struct mtd_fph_ops *f = &fph_ops;

	switch (hdr) {
	case FPH_C_PUBLIC_IP:
		return f->fph_srcip;
	case FPH_C_PUBLIC_PORT:
		return f->fph_srcport;
	case FPH_C_DEV_ID:
		return f->fph_device_id;
	case FPH_C_USER_ID:
		return f->fph_user;
	case FPH_C_TZ:
		return f->fph_tz;
	case FPH_C_LOCAL_IPS:
		return f->fph_ipaddrs;
	case FPH_C_MAC_ADDRS:
		return f->fph_macaddrs;
	case FPH_C_UA:
		return f->fph_ua;
	case FPH_C_MULTI_FACTOR:
		return f->fph_multi_factor;
	case FPH_C_SCREENS:
		return f->fph_screens;
	case FPH_C_WINDOW_SZ:
		return f->fph_window_sz;
	case FPH_C_BROWSER_PLUGINS:
		return f->fph_browser_plugins;
	case FPH_C_BROWSER_JS_UA:
		return f->fph_browser_js_ua;
	case FPH_C_BROWSER_DNT:
		return f->fph_browser_dnt;
	case FPH_C_LOCAL_IPS_TS:
		return f->fph_ipaddrs_ts;
	case FPH_C_PUBLIC_IP_TS:
		return f->fph_srcip_ts;
	case FPH_V_VERSION:
		return f->fph_version;
	case FPH_V_LICENSE_ID:
		return f->fph_license_id;
	case FPH_V_PUBLIC_IP:
		return f->fph_vendor_ip;
	case FPH_V_FWD:
		return f->fph_vendor_fwd;
	case FPH_V_PROD_NAME:
		return f->fph_prod_name;
	}

	return NULL;
}

extern __thread struct mtd_ctx mtd_ctx;
static void add_fph(struct curl_ctx *ctx, const enum fph_hdr hdr)
{
	int i;
	int n;
	char *val;
	char *(*fph_func)(void *user_data);
	const char *hstr = NULL;

	fph_func = lookup_fph_func(hdr);
	if (!fph_func)
		return;	/* Skip this header */

	val = fph_func(fph_ops.user_data);
	if (!val && !(mtd_ctx.opts & MTD_OPT_SND_EMPTY_HDRS))
		return;

	n = sizeof(fph_hdr_map) / sizeof(fph_hdr_map[0]);
	for (i = 0; i < n; i++) {
		if (fph_hdr_map[i].hdr == hdr) {
			hstr = fph_hdr_map[i].str;
			break;
		}
	}
	/*
	 * For cases where we have no suitable header value we may need
	 * to send an empty header. However, libcurl by default will just
	 * exclude any such headers.
	 *
	 * You can however force it to send an empty header by using
	 * the form
	 *
	 *     Empty-Header;
	 *
	 * note the ';' and there shouldn't be trailing space after
	 * it...
	 *
	 * So that's what the below slighty convoluted format specifier
	 * does.
	 */
	curl_add_hdr(ctx, "%s%s%s%s", hstr, val ? ":" : ";", val ? " " : "",
		     val ? val : "");
	free(val);
}

void set_anti_fraud_hdrs(struct curl_ctx *ctx)
{
	if (mtd_ctx.opts & MTD_OPT_NO_ANTI_FRAUD_HDRS)
		return;

	libcurl_sockfd = ctx->sockfd;

	curl_add_hdr(ctx, "Gov-Client-Connection-Method: %s",
		     fph_type_map[mtd_ctx.app_conn_type].str);

	add_fph(ctx, FPH_C_PUBLIC_IP);
	add_fph(ctx, FPH_C_PUBLIC_IP_TS);
	add_fph(ctx, FPH_C_PUBLIC_PORT);
	add_fph(ctx, FPH_C_DEV_ID);
	add_fph(ctx, FPH_C_USER_ID);
	add_fph(ctx, FPH_C_TZ);
	add_fph(ctx, FPH_C_SCREENS);
	add_fph(ctx, FPH_C_WINDOW_SZ);
	add_fph(ctx, FPH_C_BROWSER_PLUGINS);
	add_fph(ctx, FPH_C_BROWSER_JS_UA);
	add_fph(ctx, FPH_C_BROWSER_DNT);
	add_fph(ctx, FPH_C_LOCAL_IPS);
	add_fph(ctx, FPH_C_LOCAL_IPS_TS);
	add_fph(ctx, FPH_C_MAC_ADDRS);
	add_fph(ctx, FPH_C_UA);
	add_fph(ctx, FPH_C_MULTI_FACTOR);
	add_fph(ctx, FPH_V_LICENSE_ID);
	add_fph(ctx, FPH_V_PROD_NAME);
	add_fph(ctx, FPH_V_VERSION);
	add_fph(ctx, FPH_V_PUBLIC_IP);
	add_fph(ctx, FPH_V_FWD);
}

static const struct mtd_fph_ops dfl_fph_ops = {
	.fph_device_id		= get_device_id,
	.fph_user		= get_user,
	.fph_tz			= get_tz,
	.fph_ipaddrs		= get_ipaddrs,
	.fph_ipaddrs_ts		= get_ip_ts,
	.fph_macaddrs		= get_macaddrs,
	.fph_srcip		= get_src_addr,
	.fph_srcip_ts		= get_ip_ts,
	.fph_srcport		= get_src_port,
	.fph_screens		= get_screens,
	.fph_window_sz		= get_window_sz,
	.fph_browser_plugins	= get_browser_plugins,
	.fph_browser_js_ua	= get_browser_js_ua,
	.fph_browser_dnt	= get_browser_dnt,
	.fph_vendor_ip		= get_vendor_ip,
	.fph_vendor_fwd		= get_vendor_fwd,
	.fph_ua			= get_ua,
	.fph_multi_factor	= get_multi_factor,
	.fph_license_id		= get_license_id,
	.fph_prod_name		= get_prod_name,
	.fph_version		= get_version,
	.fph_version_cli	= NULL,
};

#define SET_FPH_FUNC(f, m) \
	fph_ops.m = fph_type & f ? (ops->m ? ops->m : dfl_fph_ops.m) : NULL;
void fph_set_ops(enum app_conn_type conn_type, const struct mtd_fph_ops *ops)
{
	unsigned int fph_type = fph_type_map[conn_type].fph;

	if (!ops)
		return;

	SET_FPH_FUNC(FPH_C_DEV_ID, MTD_FPH_CLI_DEV_ID);
	SET_FPH_FUNC(FPH_C_USER_ID, MTD_FPH_CLI_USER_ID);
	SET_FPH_FUNC(FPH_C_TZ, MTD_FPH_CLI_TZ);
	SET_FPH_FUNC(FPH_C_LOCAL_IPS, MTD_FPH_CLI_LOCAL_IPS);
	SET_FPH_FUNC(FPH_C_LOCAL_IPS_TS, MTD_FPH_CLI_LOCAL_IPS_TS);
	SET_FPH_FUNC(FPH_C_MAC_ADDRS, MTD_FPH_CLI_MAC_ADDRS);
	SET_FPH_FUNC(FPH_C_PUBLIC_IP, MTD_FPH_CLI_PUBLIC_IP);
	SET_FPH_FUNC(FPH_C_PUBLIC_IP_TS, MTD_FPH_CLI_PUBLIC_IP_TS);
	SET_FPH_FUNC(FPH_C_PUBLIC_PORT, MTD_FPH_CLI_PUBLIC_PORT);
	SET_FPH_FUNC(FPH_C_SCREENS, MTD_FPH_CLI_SCREENS);
	SET_FPH_FUNC(FPH_C_WINDOW_SZ, MTD_FPH_CLI_WINDOW_SZ);
	SET_FPH_FUNC(FPH_C_BROWSER_PLUGINS, MTD_FPH_CLI_BROWSER_PLUGINS);
	SET_FPH_FUNC(FPH_C_BROWSER_JS_UA, MTD_FPH_CLI_BROWSER_JS_UA);
	SET_FPH_FUNC(FPH_C_BROWSER_DNT, MTD_FPH_CLI_BROWSER_DNT);
	SET_FPH_FUNC(FPH_V_PUBLIC_IP, MTD_FPH_VEN_PUBLIC_IP);
	SET_FPH_FUNC(FPH_V_FWD, MTD_FPH_VEN_FWD);
	SET_FPH_FUNC(FPH_C_UA, MTD_FPH_CLI_UA);
	SET_FPH_FUNC(FPH_C_MULTI_FACTOR, MTD_FPH_CLI_MULTI_FACTOR);
	SET_FPH_FUNC(FPH_V_LICENSE_ID, MTD_FPH_VEN_LICENSE_ID);
	SET_FPH_FUNC(FPH_V_PROD_NAME, MTD_FPH_VEN_PROD_NAME);
	SET_FPH_FUNC(FPH_V_VERSION, MTD_FPH_VEN_VERSION);
	SET_FPH_FUNC(FPH_V_VERSION, MTD_FPH_VEN_VERSION_CLI);

	fph_ops.user_data = ops->user_data ? ops->user_data : NULL;
}
