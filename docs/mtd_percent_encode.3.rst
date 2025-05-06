NAME
====

**mtd_percent_encode** - percent encode a string

LIBRARY
=======

libmtdac, *-lmtdac*

SYNOPSIS
========

**#include <libmtdac/mtd.h>**

char \*mtd_percent_encode(const char \*str, ssize_t len);

DESCRIPTION
===========

This function percent encodes the given string.

**str**
    A pointer to a buffer containing the string to be encoded.

**len**
    The length of the string pointed to by *str*. Use **-1** for
    len to be taken as *strlen(str)*.

RETURN VALUE
============

On success, a pointer to a dynamically allocated buffer containing the
encoded string, which should be **free(2)**'d when finished with.

On error, **NULL** is returned. The only way this fails is if there was
insufficient memory to allocate the new buffer.

SEE ALSO
========

**libmtdac(3)**,
**libmtdac_mtd.h(3)**
