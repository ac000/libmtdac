NAME
====

**libmtdac** - UK HMRC Make Tax Digital API interface

SYNOPSIS
========

Main types / defines / functions

**#include <libmtdac/mtd.h>**

DESCRIPTION
===========

This is a C library for interacting with the UK's HMRC Make Tax Digital API.

Supported API endpoints
-----------------------

Business Details 1.0

https://developer.service.hmrc.gov.uk/api-documentation/docs/api/service/business-details-api/1.0/oas/page

Business Income Source Summary 3.0

https://developer.service.hmrc.gov.uk/api-documentation/docs/api/service/self-assessment-biss-api/3.0/oas/page

Business Source Adjustable Summary 7.0

https://developer.service.hmrc.gov.uk/api-documentation/docs/api/service/self-assessment-bsas-api/7.0/oas/page

Individual Calculations 7.0

https://developer.service.hmrc.gov.uk/api-documentation/docs/api/service/individual-calculations-api/7.0/oas/page

Individuals Disclosures 2.0

https://developer.service.hmrc.gov.uk/api-documentation/docs/api/service/individuals-disclosures-api/2.0/oas/page

Individuals Employments Income 2.0

https://developer.service.hmrc.gov.uk/api-documentation/docs/api/service/individuals-employments-income-api/2.0/oas/page

Individuals Expenses 3.0

https://developer.service.hmrc.gov.uk/api-documentation/docs/api/service/individuals-expenses-api/3.0/oas/page

Individual Losses 6.0

https://developer.service.hmrc.gov.uk/api-documentation/docs/api/service/individual-losses-api/6.0/oas/page

Individuals Pensions Income 2.0

https://developer.service.hmrc.gov.uk/api-documentation/docs/api/service/individuals-pensions-income-api/2.0/oas/page

Individuals Savings Income 2.0

https://developer.service.hmrc.gov.uk/api-documentation/docs/api/service/individuals-savings-income-api/2.0/oas/page

Individuals State Benefits 2.0

https://developer.service.hmrc.gov.uk/api-documentation/docs/api/service/individuals-state-benefits-api/2.0/oas/page

Obligations 3.0

https://developer.service.hmrc.gov.uk/api-documentation/docs/api/service/obligations-api/3.0/oas/page

Property Business 6.0

https://developer.service.hmrc.gov.uk/api-documentation/docs/api/service/property-business-api/6.0/oas/page

Self Assessment Accounts 4.0

https://developer.service.hmrc.gov.uk/api-documentation/docs/api/service/self-assessment-accounts-api/4.0/oas/page

Self Assessment Assist 1.0

https://developer.service.hmrc.gov.uk/api-documentation/docs/api/service/self-assessment-assist/1.0/oas/page

Self Assessment Individual Details 2.0

https://developer.service.hmrc.gov.uk/api-documentation/docs/api/service/self-assessment-individual-details-api/2.0/oas/page

Self-Employment Business 5.0

https://developer.service.hmrc.gov.uk/api-documentation/docs/api/service/self-employment-business-api/5.0/oas/page

VAT 1.0

https://developer.service.hmrc.gov.uk/api-documentation/docs/api/service/vat-api/1.0/oas/page

Create Test User 1.0

https://developer.service.hmrc.gov.uk/api-documentation/docs/api/service/api-platform-test-user/1.0/oas/page

Self Assessment Test Support 1.0

https://developer.service.hmrc.gov.uk/api-documentation/docs/api/service/mtd-sa-test-support-api/1.0/oas/page

Test Fraud Prevention Headers 1.0

https://developer.service.hmrc.gov.uk/api-documentation/docs/api/service/txm-fph-validator-api/1.0/oas/page

USING
=====

Include the above header in your program and link with **-lmtdac**

THREAD_SAFETY
=============

libmtdac aims to be thread safe.

LICENSE
=======

libmtdac is licensed under the GNU Lesser General Public License (LGPL)
version 2.1

SOURCE
======

Source available at <https://github.com/ac000/libmtdac>

AUTHOR
======

Andrew Clayton <ac@sigsegv.uk>

SEE ALSO
========

**libmtdac_mtd.h(3)**
