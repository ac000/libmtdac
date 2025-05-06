NAME
====

**mtd_ep** - send query to API endpoint

LIBRARY
=======

libmtdac, *-lmtdac*

SYNOPSIS
========

**#include <libmtdac/mtd.h>**

| int mtd_ep(enum mtd_api_endpoint ep, const struct mtd_dsrc_ctx \*dsctx,
|            char \*\*buf, const char \*const params[]);

DESCRIPTION
===========

This function is used to send a query to a specified API endpoint.

(It replaces all the previous individual endpoint functions)

**ep**
   is an enum value specifying which endpoint to query. It is one of the
   enum values defined in the *mtd_api_endpoint* enum in
   **libmtdac_mtd.h(3)**

   You can see more information about each endpoint in
   **src/api_endpoints.h**

**dsctx**
   is a **struct mtd_dsrc_ctx** that defines the source of *POST*/*PUT*
   data. For endpoints that don’t take such data, this should be
   **NULL**.

**buf**
   this is the output buffer. It will be allocated by libmtdac and
   should be **free(3)**\ ’d by you.

**params**
   this specifies the parameters required for the endpoint including any
   optional or not query string. E.g.

   If an endpoint requires a *taxYear* and takes an *optional query
   string*, then params should have two entries, taxYear and either a
   valid query string or NULL if none is being used.

   The order of the entries in params *should* match the order they are
   used in the endpoint URL.

   It is OK to pass a params that has more entries than required, the
   extras will be ignored.

   There is no need to pass in how many entries is in params (or for it
   to be NULL terminated) as this is dictated by each endpoint.

   For any endpoints which don’t require *any* parameters, this can be
   **NULL**. If an endpoint *only* takes an *optional* query stirng,
   then you still need a params with a single entry either containing
   NULL or a valid query string.

   **NOTE:** nino (National Insurance Number) is not included in the
   above as that is acquired by libmtdac via other means.

RETURN VALUE
============

On success, zero (**MTD_ERR_NONE**) is returned. On error, one of the
below errors is returned.

*buf* will always either be returned as **NULL** *or* a valid result
buffer containing JSON which should be **free(3)**\ ’d.

ERRORS
======

**MTD_ERR_OS**
   There was an OS level failure.
**MTD_ERR_CURL**
   There was a libcurl error.
**MTD_ERR_REQUEST**
   There was an error with the request.

SEE ALSO
========

**libmtdac(3)**,
**libmtdac_mtd.h(3)**,
**mtd_dscrx_ctx(3type)**
