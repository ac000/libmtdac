1. [Overview](#overview)
2. [API](#api)
3. [Build it](#build-it)
4. [How to use](#how-to-use)
5. [Fraud Prevention Headers](#fraud-prevention-headers)
6. [Thread safety](#thread-safety)
7. [License](#license)
8. [Contributing](#contributing)

## Overview

An interface to the UK's HMRC [Make Tax Digital](https://developer.service.hmrc.gov.uk/api-documentation) API


## API

There are man pages [here](https://github.com/ac000/libmtdac/tree/master/man/man3) and a basic overview [here](https://github.com/ac000/libmtdac/blob/master/API.md).


## Build it

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


## How to use

Include the appropriate header file(s) as described above and link with
*-lmtdac* assuming you've built and installed the RPM or similar.


## Fraud Prevention Headers

HMRC describe various headers that should be added (currently only required
for VAT submissions, but seems the intent is to require them for everything)
for [Fraud Prevention](https://developer.service.hmrc.gov.uk/api-documentation/docs/fraud-prevention)

Some of the information sent in the headers contains somewhat potentially
sensitive information, such as OS username, local IP addresses/mac addresses
and a unique device id.

Caveat Emptor.

What is sent for each header can be overridden. See the
[Fraud Prevention Headers](API.md#fraud-prevention-headers) section of
**API.md** for more details.


## Thread safety

libmtdac should not at least be inherently thread unsafe.


## License

This library is licensed under the GNU Lesser General Public License (LGPL)
version 2.1

See *COPYING* in the repository root for details.


## Contributing

See *CodingStyle.md* & *Contributing.md*
