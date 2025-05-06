NAME
====

**mtd_global_init** - global initialisation

LIBRARY
=======

libmtdac, *-lmtdac*

SYNOPSIS
========

**#include <libmtdac/mtd.h>**

void mtd_global_init(void);

DESCRIPTION
===========

This function should be called before **any** other threads are running.
In a non-threaded application you can skip this function and pass
**MTD_OPT_GLOBAL_INIT** to **mtd_init(3)** instead.

ERRORS
======

None. This function always succeeds.

SEE ALSO
========

**libmtdac(3)**,
**libmtdac_mtd.h(3)**,
**mtd_init(3)**,
**mtd_deinit(3)**
