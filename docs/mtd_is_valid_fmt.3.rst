NAME
====

**mtd_is_valid_fmt** - check if given item is in a valid format

LIBRARY
=======

libmtdac, *-lmtdac*

SYNOPSIS
========

**#include <libmtdac/mtd.h>**

bool mtd_is_valid_fmt(enum mtd_vldt_fmt what, const char \*str);

DESCRIPTION
===========

This function checks that the specified item is in the correct format.

**what**
    Which item to check. One of the values from *enum mtd_vldt_fmt* or one
    of their aliases.

**str**
    The item to check.

RETURN VALUE
============

A boolean, true for valid, false otherwise.

SEE ALSO
========

**libmtdac(3)**,
**libmtdac_mtd.h(3)**
