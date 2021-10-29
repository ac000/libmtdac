/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * platform/freebsd/get_ll_addr.c - Get a MAC address
 *
 * Copyright (C) 2021		Andrew Clayton <andrew@digital-domain.net>
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <net/if_dl.h>
#include <ifaddrs.h>

#include "../../platform.h"

/*
 * Get and format a link-level (MAC) address from a 'struct sockaddr'
 * passed in from a 'struct ifaddrs' as returned from getifaddrs(3)
 */
int get_ll_addr(const struct sockaddr *sa, char *buf, size_t buflen)
{
	unsigned char *ph;

	if (!sa || sa->sa_family != AF_LINK)
		return -1;

	/*
	 * A little ugly, but sockaddr_dl->sdl_data[] is char,
	 * and we need unsigned char for 0x00 -> 0xff.
	 */
	ph = (unsigned char *)LLADDR((struct sockaddr_dl *)sa);
	snprintf(buf, buflen, "%02x:%02x:%02x:%02x:%02x:%02x",
		 ph[0], ph[1], ph[2], ph[3], ph[4], ph[5]);

	return 0;
}
