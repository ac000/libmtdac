/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * auth.h
 *
 * Copyright (C) 2020 - 2025	Andrew Clayton <ac@sigsegv.uk>
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

extern char *load_token(const char *which, enum file_type ft,
			enum mtd_api_scope scope);
extern int oauther_refresh_access_token(enum mtd_api_scope scope);
extern int oauther_get_application_token(enum mtd_api_scope scope);
extern int oauther_dummy(enum mtd_api_scope);

#endif /* _AUTH_H_ */
