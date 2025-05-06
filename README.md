# libmtdac

[![Builds](https://github.com/ac000/libmtdac/actions/workflows/build_tests.yaml/badge.svg)](https://github.com/ac000/libmtdac/actions/workflows/build_tests.yaml "GitHub workflow builds")
[![FreeBSD Build Status](https://api.cirrus-ci.com/github/ac000/libmtdac.svg)](https://cirrus-ci.com/github/ac000/libmtdac "Cirrus FreeBSD builds")
[![CodeQL](https://github.com/ac000/libmtdac/workflows/CodeQL/badge.svg)](https://github.com/ac000/libmtdac/actions?query=workflow:CodeQL "Code quality workflow status")

1. [Overview](#overview)
2. [API](#api)
3. [Build it](#build-it)
4. [Register with HMRC](#register-with-hmrc)
5. [How to use](#how-to-use)
6. [Fraud Prevention Headers](#fraud-prevention-headers)
7. [Thread safety](#thread-safety)
8. [License](#license)
9. [Contributing](#contributing)

## Overview

An interface to the UK's HMRC [Make Tax Digital](https://developer.service.hmrc.gov.uk/api-documentation) API

It currently supports the following MTD APIs

| API                                | Version | Documentation
|------------------------------------|---------|---------------|
| Business Details                   | 1.0     | <https://developer.service.hmrc.gov.uk/api-documentation/docs/api/service/business-details-api/1.0/oas/page> |
| Business Income Source Summary     | 3.0     | <https://developer.service.hmrc.gov.uk/api-documentation/docs/api/service/self-assessment-biss-api/3.0/oas/page> |
| Business Source Adjustable Summary | 6.0     | <https://developer.service.hmrc.gov.uk/api-documentation/docs/api/service/self-assessment-bsas-api/6.0/oas/page> |
| Individual Calculations            | 7.0     | <https://developer.service.hmrc.gov.uk/api-documentation/docs/api/service/individual-calculations-api/7.0/oas/page> |
| Individual Losses                  | 5.0     | <https://developer.service.hmrc.gov.uk/api-documentation/docs/api/service/individual-losses-api/5.0/oas/page> |
| Obligations                        | 3.0     | <https://developer.service.hmrc.gov.uk/api-documentation/docs/api/service/obligations-api/3.0/oas/page> |
| Property Business                  | 5.0     | <https://developer.service.hmrc.gov.uk/api-documentation/docs/api/service/property-business-api/5.0/oas/page> |
| Self Assessment Individual Details | 2.0     | <https://developer.service.hmrc.gov.uk/api-documentation/docs/api/service/self-assessment-individual-details-api/2.0/oas/page> |
| Self Employment Business           | 4.0     | <https://developer.service.hmrc.gov.uk/api-documentation/docs/api/service/self-employment-business-api/4.0/oas/page> |
|                                    |         |               |
| Create Test User                   | 1.0     | <https://developer.service.hmrc.gov.uk/api-documentation/docs/api/service/api-platform-test-user/1.0/oas/page> |
| Test Fraud Prevention Headers      | 1.0     | <https://developer.service.hmrc.gov.uk/api-documentation/docs/api/service/txm-fph-validator-api/1.0/oas/page> |


## API

There are some docs [here](https://github.com/ac000/libmtdac/tree/master/docs/) and in man page [format](https://github.com/ac000/libmtdac/tree/master/docs/man/).


## Build it

libmtdac is primarily developed under Linux but it also builds and runs under
FreeBSD.

**NOTE:** This requires jansson 2.14.1 or later with DTOA support (the
default) for the proper handling of real numbers.

### Linux

libmtdac only has a couple of direct dependencies, *libcurl* & *jansson*

On Red Hat/Fedora/etc you will need the

    libcurl{,-devel}
    jansson{,-devel}

packages.

On Debian (something like...)

    libcurl4{,-openssl-dev}
    libjansson{4,-dev}

The simplest way is to build the rpm (on Red Hat/Fedora) by simply doing

    $ make rpm

in the repository root. It just needs a suitable *rpmbuild/* directory
structure which can be created with

    $ mkdir -p ~/rpmbuild/{BUILD,RPMS,SOURCES,SPECS,SRPMS}

and the *rpmbuild* tool which can be found in the *rpm-build* package.

### FreeBSD

As as well as curl and jansson libraries it also requires libuuid and gmake
to build

    $ sudo pkg install gmake jansson curl e2fsprogs-libuuid

then build with

    $ gmake

or if you don't have GCC installed

    $ gmake CC=clang


## Register with HMRC

Before you can actually make use of this, you will need to register the
application with the HMRC developer hub. You can register an account
[here](https://developer.service.hmrc.gov.uk/developer/registration).

Once you've registered you can then regsiter your application by the following
steps

1. Goto https://developer.service.hmrc.gov.uk/developer/applications

    Click on "Add an application to the sandbox" on the left hand side.

2. Select required endpoints (can be changed later)

   No harm in at least selecting all the ones marked *MTD*

3. Generate and take note of the 'client\_id' & 'client\_secret'

4. Click on "View your application details"

5. Click on "Redirect URIs"

6. Click on "Add a redirect URI"

5. Paste in (minus the quotes) "urn:ietf:wg:oauth:2.0:oob"


## How to use

```c
#include <libmtdac/mtd.h>
```

Link with `-lmtdac`.

### Examples

For real world examples of usage see

- [mtd-cli](https://github.com/ac000/mtd-cli)
- [itsa](https://github.com/ac000/itsa)

## Fraud Prevention Headers

HMRC describe various headers that must be added for [Fraud Prevention](https://developer.service.hmrc.gov.uk/api-documentation/docs/fraud-prevention)

Some of the information sent in the headers contains somewhat potentially
sensitive information, such as OS username, local IP addresses/mac addresses
and a unique device id.

Caveat Emptor.

What is sent for each header can be overridden. See the
[Fraud Prevention Headers](/docs/libmtdac_mtd.h.3.rst#fraud-prevention-headers)
section of **[libmtdac_mtd_h.3.rst](/docs/libmtdac_mtd.h.3.rst)** for more
details.


## Thread safety

libmtdac aims to be thread safe.


## License

This library is licensed under the GNU Lesser General Public License (LGPL)
version 2.1

See *COPYING* in the repository root for details.


## Contributing

See [CodingStyle.md](/CodingStyle.md) & [Contributing.md](/Contributing.md)

Andrew Clayton <ac@sigsegv.uk>
