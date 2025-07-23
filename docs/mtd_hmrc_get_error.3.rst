NAME
====

**mtd_hmrc_get_error** - get HMRC endpoint error(s)

LIBRARY
=======

libmtdac, *-lmtdac*

SYNOPSIS
========

**#include <libmtdac/mtd.h>**

| int mtd_hmrc_get_error(const char \*json, struct mtd_hmrc_err \*\*mhe);

.. code-block:: c

  struct mtd_hmrc_err {
          enum mtd_hmrc_error error;
          const char *code;
          const char *msg;
          struct mtd_hmrc_err *next;
  };

DESCRIPTION
===========

This function given a JSON request result buffer will return the HMRC MTD
endpoint error code(s) of the last request contained in the buffer.

**json**
    A JSON result buffer as returned by the mtd\_* functions.

**mhe**
   A pointer to a *struct mtd_hmrc_err*, this will be allocated by
   **mtd_hmrc_get_error(3)** and should be freed by
   **mtd_hmrc_free_error(3)**.

   This will either by a single structure for a single error or a linked
   list of structures with one structure per error.

   Subsequent structures are linked via the *->next* member.

   See **mtd_hmrc_err(3type)** for example usage.

RETURN VALUE
============

On success, zero (**MTD_ERR_NONE**) is returned. On error, one of the below
errors is returned.

ERRORS
======

**MTD_ERR_OS**
    There was an OS level failure. Specifically in this case there was a
    memory allocation failure.

SEE ALSO
========

**libmtdac(3)**,
**libmtdac_mtd.h(3)**,
**mtd_hmrc_free_error(3)**,
**mtd_hmrc_err(3type)**
