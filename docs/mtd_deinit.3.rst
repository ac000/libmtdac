NAME
====

**mtd_deinit** - library cleanup

LIBRARY
=======

libmtdac, *-lmtdac*

SYNOPSIS
========

**#include <libmtdac/mtd.h>**

void mtd_deinit(void);

DESCRIPTION
===========

This function should be called once **all** uses of libmtdac have finished.

ERRORS
======

None. This function always succeeds.

SEE ALSO
========

**libmtdac(3)**,
**libmtdac_mtd.h(3)**,
**mtd_global_init(3)**,
**mtd_init(3)**
