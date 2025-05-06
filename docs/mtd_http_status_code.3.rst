NAME
====

**mtd_http_status_code** - http status code of last request

LIBRARY
=======

libmtdac, *-lmtdac*

SYNOPSIS
========

**#include <libmtdac/mtd.h>**

enum mtd_http_status_code mtd_http_status_code(const char \*json);

DESCRIPTION
===========

This function given a JSON request result buffer will return the HTTP
status code of the last request contained in the buffer.

**json**
    A JSON result buffer as returned by the mtd\_* functions.

RETURN VALUE
============

An enum value corresponding to the HTTP status code

.. code-block:: c

  enum mtd_http_status_code {
          MTD_HTTP_OK                             = 200,
          MTD_HTTP_CREATED                        = 201,
          MTD_HTTP_ACCEPTED                       = 202,
          MTD_HTTP_NO_CONTENT                     = 204,
          MTD_HTTP_SEE_OTHER                      = 303,
          MTD_HTTP_BAD_REQUEST                    = 400,
          MTD_HTTP_UNAUTHORIZED                   = 401,
          MTD_HTTP_FORBIDDEN                      = 403,
          MTD_HTTP_NOT_FOUND                      = 404,
          MTD_HTTP_METHOD_NOT_ALLOWED             = 405,
          MTD_HTTP_NOT_ACCEPTABLE                 = 406,
          MTD_HTTP_GONE                           = 410,
          MTD_HTTP_REQUEST_ENTITY_TOO_LARGE       = 413,
          MTD_HTTP_UNSUPPORTED_MEDIA_TYPE         = 415,
          MTD_HTTP_UNPROCESSABLE_CONTENT          = 422,
          MTD_HTTP_TOO_MANY_REQUESTS              = 429,
          MTD_HTTP_INTERNAL_SERVER_ERROR          = 500,
          MTD_HTTP_NOT_IMPLEMENTED                = 501,
          MTD_HTTP_SERVICE_UNAVAILABLE            = 503,
          MTD_HTTP_GATEWAY_TIMEOUT                = 504,
  };

SEE ALSO
========

**libmtdac(3)**,
**libmtdac_mtd.h(3)**
