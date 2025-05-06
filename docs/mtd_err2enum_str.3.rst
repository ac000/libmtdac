NAME
====

**mtd_err2enum_str** - error code enum as a string

LIBRARY
=======

libmtdac, *-lmtdac*

SYNOPSIS
========

**#include <libmtdac/mtd.h>**

const char \*mtd_err2enum_str(int err);

DESCRIPTION
===========

This function returns a string version of the given error code enum.

**err**
    An integer error code such as returned by the mtd\_* functions.

RETURN VALUE
============

A pointer to a constant character stringified version of the error code.

SEE ALSO
========

**libmtdac(3)**,
**libmtdac_mtd.h(3)**
