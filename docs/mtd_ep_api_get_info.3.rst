NAME
====

**mtd_ep_api_get_info** - query endpoint API information

LIBRARY
=======

libmtdac, *-lmtdac*

SYNOPSIS
========

**#include <libmtdac/mtd.h>**

| struct mtd_ep_api_info mtd_ep_api_get_info(enum mtd_ep_api api);

DESCRIPTION
===========

This function is used to get information about a specific MTD API.

**api**
   is an enum value specifying which API to query. It is one of the
   enum values defined in the *mtd_ep_api* enum in **libmtdac_mtd.h(3)**

RETURN VALUE
============

A *struct mtd_ep_api_info* is returned.

On success, the *api* member will be set to the value of *api* as passed in.

Currently two other members are set, *version* which is the version number of
the specified API as a string and *name* which is the full name of the API.

On failure, the *api* member will be set to *MTD_EP_API_NULL*.

SEE ALSO
========

**libmtdac(3)**,
**libmtdac_mtd.h(3)**,
