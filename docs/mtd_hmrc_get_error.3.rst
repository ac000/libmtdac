NAME
====

**mtd_hmrc_get_error** - get HMRC endpoint error code

LIBRARY
=======

libmtdac, *-lmtdac*

SYNOPSIS
========

**#include <libmtdac/mtd.h>**

enum mtd_hmrc_error mtd_hmrc_get_error(const char \*json);

DESCRIPTION
===========

This function given a JSON request result buffer will return the HMRC MTD
endpoint error code of the last request contained in the buffer.

**json**
    A JSON result buffer as returned by the mtd\_* functions.

RETURN VALUE
============

An enum value corresponding to the HMRC MTD endpoint error code.

SEE ALSO
========

**libmtdac(3)**,
**libmtdac_mtd.h(3)**
