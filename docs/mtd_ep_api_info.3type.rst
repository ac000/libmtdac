.. highlight:: c

NAME
====

**struct mtd_ep_api_info** - information about a MTD API

LIBRARY
=======

libmtdac, *-lmtdac*

SYNOPSIS
========

**#include <libmtdac/mtd.h>**

.. code-block::

  struct mtd_ep_api_info {
          enum mtd_ep_api api;
          const char *version;
  };

DESCRIPTION
===========

This is returned from the **mtd_ep_api_get_info(3)** function and is used
to provide information about a specific MTD API.

If the call to **mtd_ep_api_get_info(3)** was successful then the *api*
member will be set to the specified *api* else it will be set to
*MTD_EP_API_NULL*

EXAMPLE
=======

.. code-block::

  struct mtd_ep_api_info eai;

  eai = mtd_ep_api_get_info(MTD_EP_API_BD);
  if (eai.api != MTD_EP_API_NULL)
          printf("MTD_EP_API_BD [%s]\n", eai.version);

SEE ALSO
========

**libmtdac(3)**,
**libmtdac_mtd.h(3)**,
**mtd_ep_api_get_info(3)**
