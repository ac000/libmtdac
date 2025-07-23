NAME
====

**mtd_hmrc_free_error** - free a linked list of struct mtd_hmrc_err

LIBRARY
=======

libmtdac, *-lmtdac*

SYNOPSIS
========

**#include <libmtdac/mtd.h>**

| void mtd_hmrc_free_error(struct mtd_hmrc_err \*mhe);

DESCRIPTION
===========

This function frees the given linked list of *struct mtd_hmrc_err* as
allocated by **mtd_hmrc_get_error(3)**.

**mhe**
   A pointer to a *struct mtd_hmrc_err*.

SEE ALSO
========

**libmtdac(3)**,
**libmtdac_mtd.h(3)**,
**mtd_hmrc_get_error(3)**,
**mtd_hmrc_err(3type)**
