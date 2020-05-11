/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * auth.h
 *
 * Copyright (C) 2020	Andrew Clayton <andrew@digital-domain.net>
 */

#ifndef _AUTH_H_
#define _AUTH_H_

#ifdef __cplusplus
extern "C" {
#endif

enum file_type {
	FT_AUTH = 0,
	FT_CONFIG,
};

extern char *load_token(const char *which, enum file_type type);
extern int refresh_access_token(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _AUTH_H_ */
