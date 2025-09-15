/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * platform/linux/gen_uuid.c - generate a version 4 UUID
 *
 * Copyright (C) 2021 - 2022	Andrew Clayton <andrew@digital-domain.net>
 */

#include <stdio.h>

#include "../../platform.h"

#define UUID_LEN	36

char *gen_uuid(char *buf)
{
	FILE *fp;
	size_t bytes_read;

	fp = fopen("/proc/sys/kernel/random/uuid", "r");
	if (!fp)
		return NULL;

	bytes_read = fread(buf, 1, UUID_LEN, fp);
	fclose(fp);

	if (bytes_read != UUID_LEN)
		return NULL;

	buf[UUID_LEN] = '\0';

	return buf;
}
