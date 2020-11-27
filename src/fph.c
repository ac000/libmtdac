/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * fph.c - Make Tax Digital - Fraud Prevention Headers
 *
 * Copyright (C) 2020		Andrew Clayton <andrew@digital-domain.net>
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
#include <linux/if_packet.h>
#include <linux/limits.h>

#include <jansson.h>

#include "mtd.h"
#include "mtd-priv.h"
#include "curler.h"
#include "logger.h"

#define BUF_SZ	1024

enum {
	FPH_C_PUBLIC_IP		= 0x1,
	FPH_C_PUBLIC_PORT	= 0x2,
	FPH_C_DEV_ID		= 0x4,
	FPH_C_USER_ID		= 0x8,
	FPH_C_TZ		= 0x10,
	FPH_C_LOCAL_IP		= 0x20,
	FPH_C_MAC_ADDR		= 0x40,
	FPH_C_UA		= 0x80,
	FPH_C_MULTI_FACTOR	= 0x100,
	FPH_C_SCREENS		= 0x200,
	FPH_C_WINDOW_SZ		= 0x400,
	FPH_C_BROWSER_PLUGINS	= 0x800,
	FPH_C_BROWSER_JS_UA	= 0x1000,
	FPH_C_BROWSER_DNT	= 0x2000,
	FPH_V_VERSION		= 0x100000,
	FPH_V_LICENSE_ID	= 0x200000,
	FPH_V_PUBLIC_IP		= 0x400000,
	FPH_V_FWD		= 0x800000,
};

#define _FPH_COMMON \
	FPH_C_DEV_ID	   | \
	FPH_C_USER_ID	   | \
	FPH_C_TZ	   | \
	FPH_C_LOCAL_IP	   | \
	FPH_C_MAC_ADDR	   | \
	FPH_C_UA	   | \
	FPH_C_MULTI_FACTOR | \
	FPH_V_VERSION	   | \
	FPH_V_LICENSE_ID

#define _FPH_COMMON_DESKTOP \
	_FPH_COMMON	| \
	FPH_C_SCREENS	| \
	FPH_C_WINDOW_SZ

#define FPH_DESKTOP_APP_DIRECT		_FPH_COMMON_DESKTOP

#define FPH_DESKTOP_APP_VIA_SERVER \
	FPH_C_PUBLIC_IP	    | \
	FPH_C_PUBLIC_PORT   | \
	_FPH_COMMON_DESKTOP | \
	FPH_V_PUBLIC_IP	    | \
	FPH_V_FWD

#define FPH_WEB_APP_VIA_SERVER \
	((FPH_DESKTOP_APP_VIA_SERVER) & ~FPH_C_MAC_ADDR & ~FPH_C_UA)	| \
	FPH_C_BROWSER_PLUGINS						| \
	FPH_C_BROWSER_JS_UA						| \
	FPH_C_BROWSER_DNT

#define FPH_MOBILE_APP_DIRECT		FPH_DESKTOP_APP_DIRECT

#define FPH_MOBILE_APP_VIA_SERVER	FPH_DESKTOP_APP_VIA_SERVER

#define FPH_BATCH_PROCESS_DIRECT \
	(_FPH_COMMON) & ~FPH_C_MULTI_FACTOR

#define FPH_OTHER_DIRECT		_FPH_COMMON

#define FPH_OTHER_VIA_SERVER \
	FPH_C_PUBLIC_IP	  | \
	FPH_C_PUBLIC_PORT | \
	_FPH_COMMON	  | \
	FPH_V_PUBLIC_IP	  | \
	FPH_V_FWD

static const struct _fph_type_map {
	const unsigned int fph;
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

static __thread struct mtd_fph_ops fph_ops;

extern __thread struct mtd_ctx mtd_ctx;

static char *get_license_id(void)
{
	return NULL;
}

static char *get_multi_factor(void)
{
	return NULL;
}

static char *get_screens(void)
{
	return NULL;
}

static char *get_window_sz(void)
{
	return NULL;
}

static char *get_browser_plugins(void)
{
	return NULL;
}

static char *get_browser_js_ua(void)
{
	return NULL;
}

static char *get_browser_dnt(void)
{
	return NULL;
}

static char *get_version(void)
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
		ver_cli = fph_ops.fph_version_cli();

	err = asprintf(&buf, "%s=%s%s%s", encname, encver, ver_cli ? "&" : "",
		       ver_cli ? ver_cli : "");
	if (err == -1) {
		logger(MTD_LOG_ERRNO, "asprintf:");
		buf = NULL;
	}

	free(encname);
	free(encver);
	free(ver_cli);

	return buf;
}

static char *get_ua(void)
{
	struct utsname un;
	char *buf;
	char *encsys;
	char *encrel;
	char *encvendor = NULL;
	char *encmodel = NULL;
	char line[BUF_SZ];
	FILE *fp;
	int err;

	uname(&un);
	encsys = mtd_percent_encode(un.sysname, -1);
	encrel = mtd_percent_encode(un.release, -1);

	fp = fopen("/proc/cpuinfo", "re");
	while (fgets(line, sizeof(line), fp)) {
		char *ptr;

		line[strlen(line) - 1] = '\0'; /* loose the trailing \n */

		ptr = strstr(line, "vendor_id");
		if (ptr && !encvendor) {
			ptr = strchr(line, ':');
			if (ptr)
				encvendor = mtd_percent_encode(ptr + 2, -1);
		}

		ptr = strstr(line, "model name");
		if (ptr && !encmodel) {
			ptr = strchr(line, ':');
			if (ptr)
				encmodel = mtd_percent_encode(ptr + 2, -1);
		}

		if (encvendor && encmodel)
			break;
	}
	fclose(fp);

	err = asprintf(&buf, "%s/%s (%s/%s)", encsys, encrel, encvendor,
		       encmodel);
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

static char *get_macaddrs(void)
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
		int family;
		char mac[18];
		char *encmac;
		unsigned char *ph;

		if (ifa->ifa_addr == NULL)
			continue;

		family = ifa->ifa_addr->sa_family;
		if (family != AF_PACKET)
			continue;

		ph = ((struct sockaddr_ll *)ifa->ifa_addr)->sll_addr;
		snprintf(mac, sizeof(mac), "%02x:%02x:%02x:%02x:%02x:%02x",
			 ph[0], ph[1], ph[2], ph[3], ph[4], ph[5]);
		encmac = mtd_percent_encode(mac, -1);
		maclen += snprintf(buf + maclen, BUF_SZ - maclen, "%s,",
				   encmac);
		free(encmac);
	}
	buf[maclen - 1] = '\0';	/* trim trailing ',' */

	freeifaddrs(ifaddr);

	return strdup(buf);
}

struct ip_prefix {
	const char *ip;
	const short prefix;
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

static char *get_ipaddrs(void)
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
		iplen += snprintf(buf + iplen, BUF_SZ - iplen, "%s,", encip);
		free(encip);
	}
	buf[iplen - 1] = '\0';	/* trim trailing ',' */

	freeifaddrs(ifaddr);

	return strdup(buf);
}

static __thread int libcurl_sockfd;
static char *get_src_port(void)
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

static char *get_src_addr(void)
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

static char *get_vendor_fwd(void)
{
	return NULL;
}

static char *get_vendor_ip(void)
{
	return NULL;
}

static char *get_tz(void)
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

static char *get_user(void)
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

static char *get_device_id(void)
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

static void add_fph(struct curl_ctx *ctx, const char *hdr,
		    char *(*get_value)(void))
{
	char *val;

	if (!get_value)
		return;	/* Skip this header */

	val = get_value();
	/*
	 * For cases where we have no suitable header value we need to
	 * send an empty header. However, libcurl by default will just
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
	curl_add_hdr(ctx, "%s%s%s%s", hdr, val ? ":" : ";", val ? " " : "",
		     val ? val : "");
	free(val);
}

void set_anti_fraud_hdrs(const struct mtd_ctx *mtd_ctx, struct curl_ctx *ctx)
{
	if (mtd_ctx->opts & MTD_OPT_NO_ANTI_FRAUD_HDRS)
		return;

	libcurl_sockfd = ctx->sockfd;

	curl_add_hdr(ctx, "Gov-Client-Connection-Method: %s",
		     fph_type_map[mtd_ctx->app_conn_type].str);

	add_fph(ctx, "Gov-Client-Public-IP", fph_ops.fph_srcip);
	add_fph(ctx, "Gov-Client-Public-Port", fph_ops.fph_srcport);
	add_fph(ctx, "Gov-Client-Device-ID", fph_ops.fph_device_id);
	add_fph(ctx, "Gov-Client-User-IDs", fph_ops.fph_user);
	add_fph(ctx, "Gov-Client-Timezone", fph_ops.fph_tz);
	add_fph(ctx, "Gov-Client-Screens", fph_ops.fph_screens);
	add_fph(ctx, "Gov-Client-Window-Size", fph_ops.fph_window_sz);
	add_fph(ctx, "Gov-Client-Browser-Plugins",
		fph_ops.fph_browser_plugins);
	add_fph(ctx, "Gov-Client-Browser-JS-User-Agent",
		fph_ops.fph_browser_js_ua);
	add_fph(ctx, "Gov-Client-Browser-Do-Not-Track",
		fph_ops.fph_browser_dnt);
	add_fph(ctx, "Gov-Client-Local-IPs", fph_ops.fph_ipaddrs);
	add_fph(ctx, "Gov-Client-MAC-Addresses", fph_ops.fph_macaddrs);
	add_fph(ctx, "Gov-Client-User-Agent", fph_ops.fph_ua);
	add_fph(ctx, "Gov-Client-Multi-Factor", fph_ops.fph_multi_factor);
	add_fph(ctx, "Gov-Vendor-License-IDs", fph_ops.fph_license_id);
	add_fph(ctx, "Gov-Vendor-Version", fph_ops.fph_version);
	add_fph(ctx, "Gov-Vendor-Public-IP", fph_ops.fph_vendor_ip);
	add_fph(ctx, "Gov-Vendor-Forwarded", fph_ops.fph_vendor_fwd);
}

static const struct mtd_fph_ops dfl_fph_ops = {
	.fph_device_id		= get_device_id,
	.fph_user		= get_user,
	.fph_tz			= get_tz,
	.fph_ipaddrs		= get_ipaddrs,
	.fph_macaddrs		= get_macaddrs,
	.fph_srcip		= get_src_addr,
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
	.fph_version		= get_version,
	.fph_version_cli	= NULL,
};

#define SET_FPH_FUNC(f, m) \
	fph_type & f ? (ops && ops->m ? ops->m : dfl_fph_ops.m) : NULL;
void fph_set_ops(enum app_conn_type conn_type, const struct mtd_fph_ops *ops)
{
	unsigned int fph_type = fph_type_map[conn_type].fph;

	fph_ops.fph_device_id = SET_FPH_FUNC(FPH_C_DEV_ID, fph_device_id);
	fph_ops.fph_user = SET_FPH_FUNC(FPH_C_USER_ID, fph_user);
	fph_ops.fph_tz = SET_FPH_FUNC(FPH_C_TZ, fph_tz);
	fph_ops.fph_ipaddrs = SET_FPH_FUNC(FPH_C_LOCAL_IP, fph_ipaddrs);
	fph_ops.fph_macaddrs = SET_FPH_FUNC(FPH_C_MAC_ADDR, fph_macaddrs);
	fph_ops.fph_srcip = SET_FPH_FUNC(FPH_C_PUBLIC_IP, fph_srcip);
	fph_ops.fph_srcport = SET_FPH_FUNC(FPH_C_PUBLIC_PORT, fph_srcport);
	fph_ops.fph_screens = SET_FPH_FUNC(FPH_C_SCREENS, fph_screens);
	fph_ops.fph_window_sz = SET_FPH_FUNC(FPH_C_WINDOW_SZ, fph_window_sz);
	fph_ops.fph_browser_plugins = SET_FPH_FUNC(FPH_C_BROWSER_PLUGINS,
						   fph_browser_plugins);
	fph_ops.fph_browser_js_ua = SET_FPH_FUNC(FPH_C_BROWSER_JS_UA,
						 fph_browser_js_ua);
	fph_ops.fph_browser_dnt = SET_FPH_FUNC(FPH_C_BROWSER_DNT,
					       fph_browser_dnt);
	fph_ops.fph_vendor_ip = SET_FPH_FUNC(FPH_V_PUBLIC_IP, fph_vendor_ip);
	fph_ops.fph_vendor_fwd = SET_FPH_FUNC(FPH_V_FWD, fph_vendor_fwd);
	fph_ops.fph_ua = SET_FPH_FUNC(FPH_C_UA, fph_ua);
	fph_ops.fph_multi_factor = SET_FPH_FUNC(FPH_C_MULTI_FACTOR,
						fph_multi_factor);
	fph_ops.fph_license_id = SET_FPH_FUNC(FPH_V_LICENSE_ID,
					      fph_license_id);
	fph_ops.fph_version = SET_FPH_FUNC(FPH_V_VERSION, fph_version);
	fph_ops.fph_version_cli = SET_FPH_FUNC(FPH_V_VERSION, fph_version_cli);
}
