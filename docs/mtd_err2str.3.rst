NAME
====

**mtd_err2str** - error code to description

LIBRARY
=======

libmtdac, *-lmtdac*

SYNOPSIS
========

**#include <libmtdac/mtd.h>**

const char \*mtd_err2str(int err);

DESCRIPTION
===========

This function returns a textual description for the given error code.

**err**
   An integer error code such as returned by the mtd\_* functions.

RETURN VALUE
============

A pointer to a constant character string describing the error.

SEE ALSO
========

**libmtdac(3)**,
**libmtdac_mtd.h(3)**
