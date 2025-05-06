NAME
====

**mtd_init_nino** - initialise national insurance number

LIBRARY
=======

libmtdac, *-lmtdac*

SYNOPSIS
========

**#include <libmtdac/mtd.h>**

int mtd_init_nino(void);

DESCRIPTION
===========

This function is used to create an initial
**~/.config/APP/libmtdac/{prod,test}-api/nino.json** file that contains
the users *NINO* (National Insurance Number).

This will prompt the user to enter their *NINO* after which the file will
be created.

This should just be called once during an initial setup routine.

RETURN VALUE
============

On success, zero (**MTD_ERR_NONE**) is returned. On error, one of the
below errors is returned.

ERRORS
======

**MTD_ERR_OS**
    There was an OS level failure.

FILES
=====

~/.config/APP/libmtdac/{prod,test}-api/nino.json

SEE ALSO
========

**libmtdac(3)**,
**libmtdac_mtd.h(3)**
