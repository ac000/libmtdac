/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * auth.h
 *
 * Copyright (C) 2020	Andrew Clayton <andrew@digital-domain.net>
 */

#ifndef _AUTH_H_
#define _AUTH_H_

enum file_type {
	FT_AUTH = 0,
	FT_AUTH_APPLICATION,
	FT_CONFIG,
	FT_NINO,
};

extern char *load_token(const char *which, enum file_type type);
extern int oauther_refresh_access_token(void);
extern int oauther_get_application_token(void);
extern int oauther_dummy(void);

#endif /* _AUTH_H_ */
