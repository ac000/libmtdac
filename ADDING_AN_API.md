# How to Add Support For An Endpoint API

Adding support for a new endpoint/API is relatively straightforward. Existing
APIs are found in the *src/mtd-\*.c* files.

See this [commit](https://github.com/ac000/libmtdac/commit/bda805a616facfa410e23baf09b61c47f4344331) for an example of adding a new API.

## The quick guide

- Create src/mtd-API.c & include/mtd/mtd-API.h
- Add entries to src/endpoints.{c,h} & (order of entries must match)
- Update API.md
- Update man/man3/libmtdac.3
- Create man/man3/libmtdac\_mtd-API.h.3
- Create man/man3/mtd\_API\_\*.3

- Finally. Wire up support in [mtd-cli](https://github.com/ac000/mtd-cli/)

## The detailed guide

### Code

As an example lets start with *src/mtd-ic.c*

These files should be named after the API they represent. Generally just take
the first character of each of the words in the API name. E.g for

    Individual Calculations

you'd call it

    mtd-ic.c

If there is already a file called that, then adjust the name as appropriate.

Lets take a look at the start of that file


```C
/* SPDX-License-Identifier: LGPL-2.1 */

/*
 * mtd-ic.c - Make Tax Digital - Individual Calculations API
 *
 * Copyright (C) 2020 - 2021	Andrew Clayton <andrew@digital-domain.net>
 */

#include <stddef.h>

#include "mtd-ic.h"		/* for default (public) visibility */
#include "endpoints.h"

#define API_VER			"Accept: application/vnd.hmrc.2.0+json"
```

Should be relatively self-explanatory. The first line is the license header,
this is always the same.

Then we have a simple line describing the file in question and what API it is
for.

Next is the copyright line (there is no copyright assignment for this project,
just put your details there).

You will likely always need to include stddef.h for 'NULL'.

Then we have a couple of internal includes, the first is the corresponding
header file for this API (more on that later) and then we have endpoint.h for
the definition of do\_ep().

After that we define the functions for each API endpoint. These usually just
make a call to do\_ep() and nothing else. E.g

```C
/*
 * [POST]
 * /individuals/calculations/crystallisation/{nino}/{taxYear}/crystallise
 */
int mtd_ic_cr_crystallise(const struct mtd_dsrc_ctx *dsctx,
                          const char *tax_year, char **buf)
{
        return do_ep(IC_CR_CRYSTALLISE, API_VER,
                     dsctx, buf, tax_year, (char *)NULL);
}
```

It starts with a simple comment header describing the HTTP method the call
uses and the path of the request.

This function takes a *const struct mtd_dsrc_ctx* as the first argument, this
describes where the POST data is to be found.

Here's another example

```C
/*
 * [GET ]
 * /individuals/calculations/{nino}/self-assessment/{calculationId}/messages[?ty
pe={info,warning,error}[[&type=...], ...]
 */
int mtd_ic_sa_get_messages(const char *cid, const char *query_string,
                           char **buf)
{
        return do_ep(IC_SA_GET_MSGS, API_VER,
                     NULL, buf, cid, query_string, (char *)NULL);
}
```

This one is for a GET and doesn't take a *const struct mtd_dsrc_ctx* (takes
*NULL* instead).

*cid* represents the *calculationId* in the url.

*query_string* in this case is optional and can be NULL or it can be a query
string containing one or more *type=* parameters.

As you can see both functions have a *char \*\*buf* as the last argument,
every function would have at least this. This is a buffer that will contain
the output of the query.

You should follow the above argument order, Arguments corresponding to *{}*
values (except *nino*), query\_string *if* there is one and then the result
buffer.

The central tenet of the API support is the do\_ep() function, it has the
following prototype

```C
int do_ep(enum endpoint ep, const char *api_ver,
          const struct mtd_dsrc_ctx *dsctx, char **buf, ...);
```

The first argument is an enum value you define in *endpoints.h* and says what
API endpoint is being called.

Next we have the API version, this is the value used for the HTTP 'Accept'
header and tells HMRC what version of the API we are requesting, this is
*API_VER*.

Next we have *struct mtd_dsrc_ctx *\*, this is a structure describing where
to find the data for the POST, it will be *NULL* for GETs.

Next we have the output buffer, this is always provided.

Next we have zero or more *char *\* arguments terminated by a *(char \*)NULL*.
These represent the passed in values that get substituted in the url for the
bits inside *{}*. Except for *nino* which get passed in automatically. So in
the above we pass in *cid* but not *nino*.

Once you have the above and the enum values defined in *endpoints.h* (follow
the ordering in there). You can now add entries to the *endpoints[]* array
in *endpointd.c*. This an array of structures define as

```C
static const struct _endpoint {
        const char *tmpl;
        const enum http_method method;
        const enum content_type ctype;
        const enum oauth_authz authz;
        const enum mtd_ep_api api;
};
```

**NOTE**: The order of entries in *endpoints.c* **must** match the order in
*endpoints.h*.

**-** *tmpl* is basically the path part of the url of the endpoint being
called, with an optional *{query_params}* or *{optional_query_params}* if
there are query parameters (optional or not) to be provided. E.g

    /individuals/calculations/{nino}/self-assessment/{calculationId}/messages/{optional_query_params}

**-** *method* is the HTTP method being used. E.g

    M_GET

**-** *ctype* refers to how the data is being presented to the API, it is an
enum define as

```C
enum content_type {
        CONTENT_TYPE_NONE = 0,
        CONTENT_TYPE_URL_ENCODED,
        CONTENT_TYPE_JSON,
};
```

For GET & DELETE this will usually be *CONTENT_TYPE_NONE* and you can leave
that out as it will get set to that (0) by default.

For POST & PUT this will be *CONTENT_TYPE_JSON*.

CONTENT\_TYPE\_URL\_ENCODED is currently only used in the OAuth endpoints.

**-** *authz* is an enum defined as

```C
enum oauth_authz {
        AUTHZ_NONE = 0,
        AUTHZ_APPLICATION,
        AUTHZ_USER,
};
```

which defines what type of authorisation the endpoint requires. Most are
*AUTHZ_USER*.

**-** *api* defines which API this endpoint is for, currently there are only
two valid possibilities for this

```C
MTD_EP_API_ITSA
MTD_EP_API_VAT
```

*MTD_EP_API_ITSA* is for *Income Tax Self-Assessment* & the other is for *VAT*.

So that would give the following for IC\_SA\_GET\_MSGS

```C
[IC_SA_GET_MSGS] = {
        .tmpl   = "/individuals/calculations/{nino}/self-assessment/{calculationId}/messages/{optional_query_params}",
        .method = M_GET,
        .authz  = AUTHZ_USER,
        .api    = MTD_EP_API_ITSA
},
```

### Documentation

Yes, the *fun* bit is done, now it's time to update the documentation.

You can generally just follow what is already there and add in the new bits.
With the man pages you can usually find a suitable existing one to copy and
edit.

Again, you can follow the commit mentioned at the top to see what needs
updating/creating, but it essentially boils down to

- Update **API.md**. Add an index item at the top and then add a section
  describing the new functions.

- Update **man/man3/libmtdac.3**.

- Create **man/man3/libmtdac_mtd-API.h.3**.

- Create **man/man3/mtd_API_.3** for each API function.

### mtd-cli

You should also wire up this new API to [mtd-cli](https://github.com/ac000/mtd-cli). This is after all how you will test the above!

This is less involved than the above. Essentially

- Create **src/mtd-cli-API.c**. You can use an existing one as a template.

- Add a corresponding entry to **src/endpoints.h**.

- Add an entry to the **api_ep_map[]** array in **src/mtd-cli.c**.

- Update **README.md**.
