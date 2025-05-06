NAME
====

**mtd_init_auth** - authorise with HMRC account

LIBRARY
=======

libmtdac, *-lmtdac*

SYNOPSIS
========

**#include <libmtdac/mtd.h>**

int mtd_init_auth(enum mtd_api_scope scope, unsigned long scopes);

DESCRIPTION
===========

This function is used to authorise the library with the users HMRC
account.

It will load the HMRC site in a new browser tab for the user to login
and then accept the specified access scopes.

It will prompt the user for the \fBauthorisation code\fP that is shown
to the user after completing the above step.

Once the user has entered the code it will create a
**~/.config/APP/libmtdac/{prod,test}-api/oauth.json** file.

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

**scopes**
    is a bitwise **OR** of one or more of the following

**MTD_SCOPE_RD_SA** & **MTD_SCOPE_WR_SA**
    Scopes for reading and writing Self-Assessment

**MTD_SCOPE_RD_VAT** & **MTD_SCOPE_WR_VAT**
    Scopes for reading and writing VAT

RETURN VALUE
============

On success, zero (**MTD_ERR_NONE**) is returned. On error, one of the
below errors is returned.

ERRORS
======

**MTD_ERR_OS**
    There was an OS level failure.

**MTD_ERR_UNKNOWN_SCOPES**
    No scopes or one or more provided scopes were not recognised.

**MTD_ERR_REQUEST**
    There was an error with the request.

**MTD_ERR_CONFIG_DIR_INVALID**
    The supplied config directory is invalid.

FILES
=====

~/.config/APP/libmtdac/{prod,test}-api/oauth.json

SEE ALSO
========

**libmtdac(3)**,
**libmtdac_mtd.h(3)**
