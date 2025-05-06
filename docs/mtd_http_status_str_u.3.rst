NAME
====

**mtd_http_status_str_u** - http status code as string version of the enum

LIBRARY
=======

libmtdac, *-lmtdac*

SYNOPSIS
========

**#include <libmtdac/mtd.h>**

const char \*mtd_http_status_str_u(const char \*json);

DESCRIPTION
===========

This function given a JSON request result buffer will return the HTTP
status code as a string of the enum of the last request contained in
the buffer.

**json**
    A JSON result buffer as returned by the mtd\_* functions.

RETURN VALUE
============

A pointer to a constant character string of the last http request status
enum string.

SEE ALSO
========

**libmtdac(3)**,
**libmtdac_mtd.h(3)**
