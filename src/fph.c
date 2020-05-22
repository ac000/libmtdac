/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * fph.c - Make Tax Digital - Fraud Prevention Headers
 *
 * Copyright (C) 2020		Andrew Clayton <andrew@digital-domain.net>
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/utsname.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <ifaddrs.h>
#include <linux/if_packet.h>
#include <linux/limits.h>

#include <curl/curl.h>

#include <jansson.h>

#include "mtd.h"
#include "mtd-priv.h"
#include "curler.h"

#define BUF_SZ	1024

static char *get_ua(CURL *curl, char *buf)
{
	struct utsname un;
	char *encsys;
	char *encrel;

	uname(&un);
	encsys = curl_easy_escape(curl, un.sysname, 0);
	encrel = curl_easy_escape(curl, un.release, 0);

	snprintf(buf, BUF_SZ, "%s/%s (/)", encsys, encrel);

	curl_free(encsys);
	curl_free(encrel);

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
}

static char *get_device_id(char *buf)
{
	char path[PATH_MAX];
	json_t *root;
	json_t *did;

	snprintf(path, sizeof(path), MTD_CONFIG_DIR_FMT, getenv("HOME"),
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

extern int mtd_opts;
extern enum app_conn_type mtd_app_conn_type;
void set_anti_fraud_hdrs(struct curl_ctx *ctx)
{
	CURL *curl;
	char buf[BUF_SZ];

	if (!(mtd_opts & MTD_OPT_SND_ANTI_FRAUD_HDRS))
		return;

	curl = curl_easy_init();

	curl_add_hdr(ctx, "Gov-Client-Device-ID: %s", get_device_id(buf));

	switch (mtd_app_conn_type) {
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
