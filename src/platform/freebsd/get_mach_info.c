/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * platform/freebsd/get_mach_info.c - get machine vendor/model
 *
 * Copyright (C) 2021		Andrew Clayton <andrew@digital-domain.net>
 */

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/sysctl.h>

#include "../../platform.h"

void get_mach_info(char *ven, size_t ven_len __unused, char *model,
		   size_t model_len)
{
	char hwmodel[256];
	char *sptr;
	char *eptr;
	size_t len = sizeof(hwmodel);

	*ven = '\0';
	*model = '\0';

	sysctlbyname("hw.model", hwmodel, &len, NULL, 0);

	sptr = hwmodel;
	eptr = strchr(hwmodel, ' ');
	snprintf(ven, (eptr-sptr) + 1, "%s", hwmodel);
	snprintf(model, model_len, "%s", hwmodel);
}
