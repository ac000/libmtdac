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

static char *get_version(CURL *curl, char *buf)
{
	char ver[128];
	char *encname;
	char *encver;

	encname = curl_easy_escape(curl, LIBNAME, 0);
	snprintf(ver, sizeof(ver), "%d.%d.%d (%s)",
		 LIBMTDAC_MAJOR_VERSION, LIBMTDAC_MINOR_VERSION,
		 LIBMTDAC_MICRO_VERSION, GIT_VERSION + 1);
	encver = curl_easy_escape(curl, ver, 0);

	snprintf(buf, BUF_SZ, "%s=%s", encname, encver);

	curl_free(encname);
	curl_free(encver);

	return buf;
}

static char *get_ua(CURL *curl, char *buf)
{
	struct utsname un;
	char *encsys;
	char *encrel;
	char *encvendor = NULL;
	char *encmodel = NULL;
	char line[BUF_SZ];
	FILE *fp;

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

	snprintf(buf, BUF_SZ, "%s/%s (%s/%s)", encsys, encrel, encvendor,
		 encmodel);

	curl_free(encsys);
	curl_free(encrel);
	curl_free(encvendor);
	curl_free(encmodel);

	return buf;
}

static char *get_macs(CURL *curl, char *buf)
{
	struct ifaddrs *ifaddr;
	struct ifaddrs *ifa;
	int n;
	int maclen = 0;
	int err;

	*buf = '\0';

	err = getifaddrs(&ifaddr);
	if (err)
		return buf;

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

	return buf;
}

struct ip_prefix {
	const char *ip;
	const short prefix;
};

static const struct ip_prefix local_ip4s[] = {
	{ "127.0.0.1",		32 },
	{ "10.0.0.0",		 8 },
	{ "172.16.0.0",		12 },
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

static char *get_ips(CURL *curl, char *buf)
{
	struct ifaddrs *ifaddr;
	struct ifaddrs *ifa;
	int n;
	int iplen = 0;
	int err;

	*buf = '\0';

	err = getifaddrs(&ifaddr);
	if (err)
		return buf;

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

	return buf;
}

static char *get_tz(char *buf)
{
	time_t now = time(NULL);
	struct tm tm_res;
	const struct tm *tm = localtime_r(&now, &tm_res);

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

static char *get_user(CURL *curl, char *buf)
{
	char *encuser;

	encuser = curl_easy_escape(curl, getenv("USER"), 0);
	snprintf(buf, BUF_SZ, "%s", encuser);
	curl_free(encuser);

	return buf;
}

static void get_other_direct_hdrs(CURL *curl, struct curl_ctx *ctx)
{
	char buf[BUF_SZ];

	curl_add_hdr(ctx, "Gov-Client-Connection-Method: OTHER_DIRECT");

	curl_add_hdr(ctx, "Gov-Client-User-IDs: os=%s", get_user(curl, buf));
	curl_add_hdr(ctx, "Gov-Client-Timezone: %s", get_tz(buf));
	curl_add_hdr(ctx, "Gov-Client-Local-IPs: %s", get_ips(curl, buf));
	curl_add_hdr(ctx, "Gov-Client-MAC-Addresses: %s", get_macs(curl, buf));
	curl_add_hdr(ctx, "Gov-Client-User-Agent: %s", get_ua(curl, buf));
	curl_add_hdr(ctx, "Gov-Vendor-Version: %s", get_version(curl, buf));
}

static char *get_device_id(char *buf)
{
	char path[PATH_MAX];
	json_t *root;
	json_t *did;

	snprintf(path, sizeof(path), MTD_CONFIG_FMT, getenv("HOME"),
		 "uuid.json");
	root = json_load_file(path, 0, NULL);
	if (!root)
		return NULL;
	did = json_object_get(root, "device_id");
	if (!did)
		return NULL;

	snprintf(buf, BUF_SZ, "%s", json_string_value(did));
	json_decref(root);

	return buf;
}

extern __thread struct mtd_ctx mtd_ctx;
void set_anti_fraud_hdrs(struct curl_ctx *ctx)
{
	CURL *curl;
	char buf[BUF_SZ];

	if (!(mtd_ctx.opts & MTD_OPT_SND_ANTI_FRAUD_HDRS))
		return;

	curl = curl_easy_init();

	curl_add_hdr(ctx, "Gov-Client-Device-ID: %s", get_device_id(buf));

	switch (mtd_ctx.app_conn_type) {
	case MTD_ACT_MOBILE_APP_DIRECT:
	case MTD_ACT_DESKTOP_APP_DIRECT:
	case MTD_ACT_MOBILE_APP_VIA_SERVER:
	case MTD_ACT_DESKTOP_APP_VIA_SERVER:
	case MTD_ACT_WEB_APP_VIA_SERVER:
	case MTD_ACT_BATCH_PROCESS_DIRECT:
		break;
	case MTD_ACT_OTHER_DIRECT:
		get_other_direct_hdrs(curl, ctx);
		break;
	case MTD_ACT_OTHER_VIA_SERVER:
		break;
	}

	curl_easy_cleanup(curl);
}
