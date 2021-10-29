/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * platform/freebsd/gen_uuid.c - generate a UUID
 *
 * Copyright (C) 2021		Andrew Clayton <andrew@digital-domain.net>
 */

#include <stdio.h>

#include <uuid/uuid.h>

#include "../../platform.h"

char *gen_uuid(char *buf)
{
	uuid_t uuid;

	uuid_generate(uuid);
	uuid_unparse(uuid, buf);

	return buf;
}
