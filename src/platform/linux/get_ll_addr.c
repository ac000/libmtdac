/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * platform/linux/get_ll_addr.c - Get a MAC address
 *
 * Copyright (C) 2021		Andrew Clayton <andrew@digital-domain.net>
 */

#include <stdio.h>
#include <sys/socket.h>
#include <linux/if_packet.h>

#include "../../platform.h"

/*
 * Get and format a link-level (MAC) address from a 'struct sockaddr'
 * passed in from a 'struct ifaddrs' as returned from getifaddrs(3)
 */
int get_ll_addr(const struct sockaddr *sa, char *buf, size_t buflen)
{
	unsigned char *ph;

	if (!sa || sa->sa_family != AF_PACKET)
		return -1;

	ph = ((struct sockaddr_ll *)sa)->sll_addr;
	snprintf(buf, buflen, "%02x:%02x:%02x:%02x:%02x:%02x",
		 ph[0], ph[1], ph[2], ph[3], ph[4], ph[5]);

	return 0;
}
