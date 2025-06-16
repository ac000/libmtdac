libmtdac
========

|Builds| |FreeBSD Build Status| |CodeQL|

1. `Overview <#overview>`__
2. `API <#api>`__
3. `Build it <#build-it>`__
4. `Register with HMRC <#register-with-hmrc>`__
5. `How to use <#how-to-use>`__
6. `Fraud Prevention Headers <#fraud-prevention-headers>`__
7. `Thread safety <#thread-safety>`__
8. `License <#license>`__
9. `Contributing <#contributing>`__

Overview
--------

An interface to the UK’s HMRC `Make Tax
Digital <https://developer.service.hmrc.gov.uk/api-documentation>`__ API

It currently supports the following MTD APIs

=====================================	 =======
API					 Version
=====================================	 =======
`Business Details`_			 1.0
`Business Income Source Summary`_	 3.0
`Business Source Adjustable Summary`_	 6.0
`Individual Calculations`_		 7.0
`Individuals Employments Income`_	 1.0
`Individuals Expenses`_			 3.0
`Individual Losses`_			 5.0
`Individuals Pensions Income`_		 1.0
`Individuals Savings Income`_		 1.0
`Obligations`_				 3.0
`Property Business`_			 5.0
`Self Assessment Accounts`_		 3.0
`Self Assessment Assist`_		 1.0
`Self Assessment Individual Details`_	 2.0
`Self Employment Business`_		 4.0
\
`VAT`_					 1.0
\
`Create Test User`_			 1.0
`Self Assessment Test Support`_		 1.0
`Test Fraud Prevention Headers`_	 1.0
=====================================	 =======

.. _Business Details: https://developer.service.hmrc.gov.uk/api-documentation/docs/api/service/business-details-api/1.0/oas/page
.. _Business Income Source Summary: https://developer.service.hmrc.gov.uk/api-documentation/docs/api/service/self-assessment-biss-api/3.0/oas/page
.. _Business Source Adjustable Summary: https://developer.service.hmrc.gov.uk/api-documentation/docs/api/service/self-assessment-bsas-api/6.0/oas/page
.. _Individual Calculations: https://developer.service.hmrc.gov.uk/api-documentation/docs/api/service/individual-calculations-api/7.0/oas/page
.. _Individuals Employments Income: https://developer.service.hmrc.gov.uk/api-documentation/docs/api/service/individuals-employments-income-api/1.0/oas/page
.. _Individuals Expenses: https://developer.service.hmrc.gov.uk/api-documentation/docs/api/service/individuals-expenses-api/3.0/oas/page
.. _Individual Losses: https://developer.service.hmrc.gov.uk/api-documentation/docs/api/service/individual-losses-api/5.0/oas/page
.. _Individuals Pensions Income: https://developer.service.hmrc.gov.uk/api-documentation/docs/api/service/individuals-pensions-income-api/1.0/oas/page
.. _Individuals Savings Income: https://developer.service.hmrc.gov.uk/api-documentation/docs/api/service/individuals-savings-income-api/1.0/oas/page
.. _Obligations: https://developer.service.hmrc.gov.uk/api-documentation/docs/api/service/obligations-api/3.0/oas/page
.. _Property Business: https://developer.service.hmrc.gov.uk/api-documentation/docs/api/service/property-business-api/5.0/oas/page
.. _Self Assessment Accounts: https://developer.service.hmrc.gov.uk/api-documentation/docs/api/service/self-assessment-accounts-api/3.0/oas/page
.. _Self Assessment Assist: https://developer.service.hmrc.gov.uk/api-documentation/docs/api/service/self-assessment-assist/1.0/oas/page
.. _Self Assessment Individual Details: https://developer.service.hmrc.gov.uk/api-documentation/docs/api/service/self-assessment-individual-details-api/2.0/oas/page
.. _Self Employment Business: https://developer.service.hmrc.gov.uk/api-documentation/docs/api/service/self-employment-business-api/4.0/oas/page
.. _VAT: https://developer.service.hmrc.gov.uk/api-documentation/docs/api/service/vat-api/1.0/oas/page
.. _Create Test User: https://developer.service.hmrc.gov.uk/api-documentation/docs/api/service/api-platform-test-user/1.0/oas/page
.. _Self Assessment Test Support: https://developer.service.hmrc.gov.uk/api-documentation/docs/api/service/mtd-sa-test-support-api/1.0/oas/page
.. _Test Fraud Prevention Headers: https://developer.service.hmrc.gov.uk/api-documentation/docs/api/service/txm-fph-validator-api/1.0/oas/page

API
---

There are some docs
`here <https://github.com/ac000/libmtdac/tree/master/docs/>`__ and in
man page
`format <https://github.com/ac000/libmtdac/tree/master/docs/man/>`__.

Build it
--------

libmtdac is primarily developed under Linux but it also builds and runs
under FreeBSD.

**NOTE:** This requires jansson 2.14.1 or later with DTOA support (the
default) for the proper handling of real numbers.

Linux
~~~~~

libmtdac only has a couple of direct dependencies, *libcurl* & *jansson*

On Red Hat/Fedora/etc you will need the

::

   libcurl{,-devel}
   jansson{,-devel}

packages.

On Debian (something like…)

::

   libcurl4{,-openssl-dev}
   libjansson{4,-dev}

The simplest way is to build the rpm (on Red Hat/Fedora) by simply doing

::

   $ make rpm

in the repository root. It just needs a suitable *rpmbuild/* directory
structure which can be created with

::

   $ mkdir -p ~/rpmbuild/{BUILD,RPMS,SOURCES,SPECS,SRPMS}

and the *rpmbuild* tool which can be found in the *rpm-build* package.

FreeBSD
~~~~~~~

As as well as curl and jansson libraries it also requires libuuid and
gmake to build

::

   $ sudo pkg install gmake jansson curl e2fsprogs-libuuid

then build with

::

   $ gmake

or if you don’t have GCC installed

::

   $ gmake CC=clang

Register with HMRC
------------------

Before you can actually make use of this, you will need to register the
application with the HMRC developer hub. You can register an account
`here <https://developer.service.hmrc.gov.uk/developer/registration>`__.

Once you’ve registered you can then regsiter your application by the
following steps

1. Goto https://developer.service.hmrc.gov.uk/developer/applications

   Click on “Add an application to the sandbox” on the left hand side.

2. Select required endpoints (can be changed later)

   No harm in at least selecting all the ones marked *MTD*

3. Generate and take note of the ‘client_id’ & ‘client_secret’

4. Click on “View your application details”

5. Click on “Redirect URIs”

6. Click on “Add a redirect URI”

7. Paste in (minus the quotes) “urn:ietf:wg:oauth:2.0:oob”

How to use
----------

.. code:: c

   #include <libmtdac/mtd.h>

Link with ``-lmtdac``.

Examples
~~~~~~~~

For real world examples of usage see

-  `mtd-cli <https://github.com/ac000/mtd-cli>`__
-  `itsa <https://github.com/ac000/itsa>`__

Fraud Prevention Headers
------------------------

HMRC describe various headers that must be added for `Fraud
Prevention <https://developer.service.hmrc.gov.uk/api-documentation/docs/fraud-prevention>`__

Some of the information sent in the headers contains somewhat
potentially sensitive information, such as OS username, local IP
addresses/mac addresses and a unique device id.

Caveat Emptor.

What is sent for each header can be overridden. See the `Fraud
Prevention
Headers </docs/libmtdac_mtd.h.3.rst#fraud-prevention-headers>`__ section
of `libmtdac_mtd_h.3.rst </docs/libmtdac_mtd.h.3.rst>`__ for more
details.

Thread safety
-------------

libmtdac aims to be thread safe.

License
-------

This library is licensed under the GNU Lesser General Public License
(LGPL) version 2.1

See `COPYING </COPYING>`__ in the repository root for details.

Contributing
------------

See `CodingStyle.rst </CodingStyle.rst>`__ &
`Contributing.rst </Contributing.rst>`__

Andrew Clayton <ac@sigsegv.uk>

.. |Builds| image:: https://github.com/ac000/libmtdac/actions/workflows/build_tests.yaml/badge.svg
   :target: https://github.com/ac000/libmtdac/actions/workflows/build_tests.yaml
.. |FreeBSD Build Status| image:: https://api.cirrus-ci.com/github/ac000/libmtdac.svg
   :target: https://cirrus-ci.com/github/ac000/libmtdac
.. |CodeQL| image:: https://github.com/ac000/libmtdac/workflows/CodeQL/badge.svg
   :target: https://github.com/ac000/libmtdac/actions?query=workflow:CodeQL
