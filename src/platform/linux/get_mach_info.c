/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * platform/linux/get_mach_info.c - get machine vendor/model
 *
 * Copyright (C) 2021		Andrew Clayton <andrew@digital-domain.net>
 */

#include <stdio.h>
#include <string.h>

void get_mach_info(char *ven, size_t ven_len, char *model, size_t model_len)
{
	FILE *fp;
	char line[256];

	*ven = '\0';
	*model = '\0';

	fp = fopen("/proc/cpuinfo", "re");
	while (fgets(line, sizeof(line), fp)) {
		char *ptr;

		line[strlen(line) - 1] = '\0'; /* loose the trailing \n */
		ptr = strstr(line, "vendor_id");
		if (ptr && strlen(ven) == 0) {
			ptr = strchr(line, ':');
			if (ptr)
				snprintf(ven, ven_len, "%s", ptr + 2);
		}

		ptr = strstr(line, "model name");
		if (ptr && strlen(model) == 0) {
			ptr = strchr(line, ':');
			if (ptr)
				snprintf(model, model_len, "%s", ptr + 2);
		}

		if (strlen(ven) > 0 && strlen(model) > 0)
			break;
	}
	fclose(fp);
}
