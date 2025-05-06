NAME
====

**mtd_init_creds** - initialise creds.json

LIBRARY
=======

libmtdac, *-lmtdac*

SYNOPSIS
========

**#include <libmtdac/mtd.h>**

int mtd_init_creds(enum mtd_api_scope scope);

DESCRIPTION
===========

This function is used to create an initial
**~/.config/APP/libmtdac/{prod,test}-api/creds.json** file that contains
the users *client_id* & *client_secret*.

This will prompt the user to enter the above tokens after which the file
will be created.

This should just be called once during an initial setup routine.

**scope**
    is an enum value specifying which Make Tax Digital API this is adding
    authorisations for and is one of the following values

**MTD_API_SCOPE_ITSA**
    For Income TAX Self-Assessment

**MTD_API_SCOPE_VAT**
    For VAT

The above can optionally be bitwise **OR**'d with

**MTD_API_SCOPE_ADD**
    Which says don't reset the oauth.json file when writing it, instead
    add this OAuth as another entry (or overwrite an existing entry
    without resetting the file).

RETURN VALUE
============

On success, zero (**MTD_ERR_NONE**) is returned. On error, one of the
below errors is returned.

ERRORS
======

**MTD_ERR_OS**
    There was an OS level failure.

**MTD_ERR_INVALID_SCOPE**
    Invalid scope was provied.

**MTD_ERR_CONFIG_DIR_INVALID**
    The supplied config directory is invalid.

FILES
=====

~/.config/APP/libmtdac/{prod,test}-api/creds.json

SEE ALSO
========

**libmtdac(3)**,
**libmtdac_mtd.h(3)**
