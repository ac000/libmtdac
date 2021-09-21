/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * auth.h
 *
 * Copyright (C) 2020 - 2021	Andrew Clayton <andrew@digital-domain.net>
 */

#ifndef _AUTH_H_
#define _AUTH_H_

#include "mtd.h"

enum file_type {
	FT_AUTH = 0,
	FT_AUTH_APPLICATION,
	FT_CREDS,
	FT_NINO,
};

extern char *load_token(const char *which, enum file_type type,
			enum mtd_ep_api api);
extern int oauther_refresh_access_token(enum mtd_ep_api api);
extern int oauther_get_application_token(enum mtd_ep_api api);
extern int oauther_dummy(enum mtd_ep_api api);

#endif /* _AUTH_H_ */
