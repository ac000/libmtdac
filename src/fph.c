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

#include <curl/curl.h>

#include <jansson.h>

#include "mtd.h"
#include "mtd-priv.h"
#include "curler.h"

#define BUF_SZ	1024

static __thread struct mtd_fph_ops fph_ops;
static __thread CURL *curl;

extern __thread struct mtd_ctx mtd_ctx;

static char *get_license_id(void)
{
	return NULL;
}

static char *get_version(void)
{
	char ver[128];
	char *encname;
	char *encver;
	char *buf;
	int err;

	encname = curl_easy_escape(curl, LIBNAME, 0);
	snprintf(ver, sizeof(ver), "%d.%d.%d (%s)",
		 LIBMTDAC_MAJOR_VERSION, LIBMTDAC_MINOR_VERSION,
		 LIBMTDAC_MICRO_VERSION, GIT_VERSION + 1);
	encver = curl_easy_escape(curl, ver, 0);

	err = asprintf(&buf, "%s=%s", encname, encver);
	if (err == -1) {
		fprintf(stderr, "libmtdac/get_version: asprintf failed");
		buf = NULL;
	}

	curl_free(encname);
	curl_free(encver);

	return buf;
}

static char *get_multi_factpr(void)
{
	return NULL;
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
	encsys = curl_easy_escape(curl, un.sysname, 0);
	encrel = curl_easy_escape(curl, un.release, 0);

	fp = fopen("/proc/cpuinfo", "re");
	while (fgets(line, sizeof(line), fp)) {
		char *ptr;

		line[strlen(line) - 1] = '\0'; /* loose the trailing \n */

		ptr = strstr(line, "vendor_id");
		if (ptr && !encvendor) {
			ptr = strchr(line, ':');
			if (ptr)
				encvendor = curl_easy_escape(curl, ptr + 2, 0);
		}

		ptr = strstr(line, "model name");
		if (ptr && !encmodel) {
			ptr = strchr(line, ':');
			if (ptr)
				encmodel = curl_easy_escape(curl, ptr + 2, 0);
		}

		if (encvendor && encmodel)
			break;
	}
	fclose(fp);

	err = asprintf(&buf, "%s/%s (%s/%s)", encsys, encrel, encvendor,
		       encmodel);
	if (err == -1) {
		fprintf(stderr, "libmtdac/get_version: asprintf failed");
		buf = NULL;
	}

	curl_free(encsys);
	curl_free(encrel);
	curl_free(encvendor);
	curl_free(encmodel);

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
		encmac = curl_easy_escape(curl, mac, 0);
		maclen += snprintf(buf + maclen, BUF_SZ - maclen, "%s,",
				   encmac);
		curl_free(encmac);
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
		encip = curl_easy_escape(curl, ip, 0);
		iplen += snprintf(buf + iplen, BUF_SZ - iplen, "%s,", encip);
		curl_free(encip);
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
	getnameinfo((struct sockaddr *)&ss, addrlen, NULL, 0, port,
		    sizeof(port), NI_NUMERICHOST|NI_NUMERICSERV);

	return strdup(port);
}

static char *get_vendor_fwd(void)
{
	return NULL;
}

static char *get_vendor_ip(void)
{
	return NULL;
}

static char *get_src_addr(void)
{
	struct sockaddr_storage ss;
	socklen_t addrlen = sizeof(ss);
	char host[INET6_ADDRSTRLEN];

	getsockname(libcurl_sockfd, (struct sockaddr *)&ss, &addrlen);
	getnameinfo((struct sockaddr *)&ss, addrlen, host, sizeof(host), NULL,
		    0, NI_NUMERICHOST|NI_NUMERICSERV);

	return strdup(host);
}

static char *get_tz(void)
{
	time_t now = time(NULL);
	struct tm tm_res;
	const struct tm *tm = localtime_r(&now, &tm_res);
	char *buf;

	buf = malloc(strlen("UTC+HH:MM") + 1);
	if (!buf) {
		fprintf(stderr, "libmtdac/get_tz: malloc failed");
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

	encuser = curl_easy_escape(curl, getenv("USER"), 0);
	err = asprintf(&buf, "os=%s", encuser);
	if (err == -1) {
		fprintf(stderr, "libmtdac/get_user: asprintf failed");
		buf = NULL;
	}
	curl_free(encuser);

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
		fprintf(stderr, "libmtdac/get_device_id: asprintf failed");
		buf = NULL;
	}
	json_decref(root);

	return buf;
}

static void add_fph(struct curl_ctx *ctx, const char *hdr,
		    char *(*get_value)(void))
{
	char *val = get_value();

	curl_add_hdr(ctx, "%s:%s%s", hdr, val ? " " : "", val ? val : "");
	free(val);
}

static void get_other_server_hdrs(struct curl_ctx *ctx)
{
	curl_add_hdr(ctx, "Gov-Client-Connection-Method: OTHER_SERVER");

	add_fph(ctx, "Gov-Client-Public-IP", fph_ops.fph_srcip);
	add_fph(ctx, "Gov-Client-Public-Port", fph_ops.fph_srcport);
	add_fph(ctx, "Gov-Client-Device-ID", fph_ops.fph_device_id);
	add_fph(ctx, "Gov-Client-User-IDs", fph_ops.fph_user);
	add_fph(ctx, "Gov-Client-Timezone", fph_ops.fph_tz);
	add_fph(ctx, "Gov-Client-Local-IPs", fph_ops.fph_ipaddrs);
	add_fph(ctx, "Gov-Client-MAC-Addresses", fph_ops.fph_macaddrs);
	add_fph(ctx, "Gov-Client-User-Agent", fph_ops.fph_ua);
	add_fph(ctx, "Gov-Client-Multi-Factor", fph_ops.fph_multi_factor);
	add_fph(ctx, "Gov-Vendor-License-IDs", fph_ops.fph_license_id);
	add_fph(ctx, "Gov-Vendor-Version", fph_ops.fph_version);
	add_fph(ctx, "Gov-Vendor-Public-IP", fph_ops.fph_vendor_ip);
	add_fph(ctx, "Gov-Vendor-Forwarded", fph_ops.fph_vendor_fwd);
}

static void get_other_direct_hdrs(struct curl_ctx *ctx)
{
	curl_add_hdr(ctx, "Gov-Client-Connection-Method: OTHER_DIRECT");

	add_fph(ctx, "Gov-Client-Device-ID", fph_ops.fph_device_id);
	add_fph(ctx, "Gov-Client-User-IDs", fph_ops.fph_user);
	add_fph(ctx, "Gov-Client-Timezone", fph_ops.fph_tz);
	add_fph(ctx, "Gov-Client-Local-IPs", fph_ops.fph_ipaddrs);
	add_fph(ctx, "Gov-Client-MAC-Addresses", fph_ops.fph_macaddrs);
	add_fph(ctx, "Gov-Client-User-Agent", fph_ops.fph_ua);
	add_fph(ctx, "Gov-Vendor-Version", fph_ops.fph_version);
}

void set_anti_fraud_hdrs(const struct mtd_ctx *mtd_ctx, struct curl_ctx *ctx)
{
	if (mtd_ctx->opts & MTD_OPT_NO_ANTI_FRAUD_HDRS)
		return;

	curl = curl_easy_init();

	libcurl_sockfd = ctx->sockfd;

	switch (mtd_ctx->app_conn_type) {
	case MTD_ACT_MOBILE_APP_DIRECT:
	case MTD_ACT_DESKTOP_APP_DIRECT:
	case MTD_ACT_MOBILE_APP_VIA_SERVER:
	case MTD_ACT_DESKTOP_APP_VIA_SERVER:
	case MTD_ACT_WEB_APP_VIA_SERVER:
	case MTD_ACT_BATCH_PROCESS_DIRECT:
		break;
	case MTD_ACT_OTHER_DIRECT:
		get_other_direct_hdrs(ctx);
		break;
	case MTD_ACT_OTHER_VIA_SERVER:
		get_other_server_hdrs(ctx);
		break;
	}

	curl_easy_cleanup(curl);
}

void fph_set_ops(const struct mtd_fph_ops *ops)
{
	if (!ops)
		return;

	if (ops->fph_device_id)
		fph_ops.fph_device_id = ops->fph_device_id;
	if (ops->fph_user)
		fph_ops.fph_user = ops->fph_user;
	if (ops->fph_tz)
		fph_ops.fph_tz = ops->fph_tz;
	if (ops->fph_ipaddrs)
		fph_ops.fph_ipaddrs = ops->fph_ipaddrs;
	if (ops->fph_macaddrs)
		fph_ops.fph_macaddrs = ops->fph_macaddrs;
	if (ops->fph_srcip)
		fph_ops.fph_srcip = ops->fph_srcip;
	if (ops->fph_srcport)
		fph_ops.fph_srcport = ops->fph_srcport;
	if (ops->fph_vendor_ip)
		fph_ops.fph_vendor_ip = ops->fph_vendor_ip;
	if (ops->fph_vendor_fwd)
		fph_ops.fph_vendor_fwd = ops->fph_vendor_fwd;
	if (ops->fph_ua)
		fph_ops.fph_ua = ops->fph_ua;
	if (ops->fph_multi_factor)
		fph_ops.fph_multi_factor = ops->fph_multi_factor;
	if (ops->fph_license_id)
		fph_ops.fph_license_id = ops->fph_license_id;
	if (ops->fph_version)
		fph_ops.fph_version = ops->fph_version;
}

static const struct mtd_fph_ops dfl_fph_ops = {
	.fph_device_id		= get_device_id,
	.fph_user		= get_user,
	.fph_tz			= get_tz,
	.fph_ipaddrs		= get_ipaddrs,
	.fph_macaddrs		= get_macaddrs,
	.fph_srcip		= get_src_addr,
	.fph_srcport		= get_src_port,
	.fph_vendor_ip		= get_vendor_ip,
	.fph_vendor_fwd		= get_vendor_fwd,
	.fph_ua			= get_ua,
	.fph_multi_factor	= get_multi_factpr,
	.fph_license_id		= get_license_id,
	.fph_version		= get_version,
};

void fph_init_ops(void)
{
	memcpy(&fph_ops, &dfl_fph_ops, sizeof(struct mtd_fph_ops));
}
