/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * platform.h - Platform dependant stuff
 *
 * Copyright (c) 2021		Andrew Clayton <andrew@digital-domain.net>
 */

#ifndef _PLATFORM_H_
#define _PLATFORM_H_

#include <sys/socket.h>

extern char *x_strerror_r(int errnum, char *buf, size_t buflen);
extern char *gen_uuid(char *buf);
extern int get_ll_addr(const struct sockaddr *sa, char *buf, size_t buflen);
extern void get_mach_info(char *ven, size_t ven_len, char *model,
			  size_t model_len);

#endif /* _PLATFORM_H_ */
