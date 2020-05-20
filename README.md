1. [Overview](#overview)
2. [API](#api)
  * [Library version](#library-version)
  * [Errors](#errors)
  * [Initialisation functions](#initialisation-functions)
  * [Make Tax Digital - Self-Assessment API functions](#make-tax-digital---self-assessment-api-functions)
  * [Make Tax Digital - Self-Assessment Accounts API functions [test-only]](#make-tax-digital---self-assessment-accounts-api-functions-test-only)
  * [Make Tax Digital - Individual Calculations API functions](#make-tax-digital---individual-calculations-api-functions)
  * [Make Tax Digital - National Insurance API functions](#make-tax-digital---national-insurance-api-functions)
  * [Make Tax Digital - Create Test User API functions [test-only]](#make-tax-digital---create-test-user-api-functions-test-only)
  * [Make Tax Digital - Test Fraud Prevention Headers API functions [test-only]](#make-tax-digital---test-fraud-prevention-headers-api-functions-test-only)
3. [Build it](#build-it)
4. [How to use](#how-to-use)
5. [Fraud Prevention Headers](#fraud-prevention-headers)
6. [Thread safety](#thread-safety)
7. [License](#license)
8. [Contributing](#contributing)

## Overview

An interface to the UK's HMRC [Make Tax Digital](https://developer.service.hmrc.gov.uk/api-documentation) API


## API

    #include <libmtdac/mtd.h>

   This header is included by each of the API specific header files.

### Library version

    #define LIBMTDAC_MAJOR_VERSION
    #define LIBMTDAC_MINOR_VERSION
    #define LIBMTDAC_MICRO_VERSION


### Errors

    enum mtd_error {
	    MTD_ERR_NONE = 0,
	    MTD_ERR_OS,
	    MTD_ERR_REQUEST,
	    MTD_ERR_CURL,
	    MTD_ERR_MISSING_CONFIG,
	    MTD_ERR_NEEDS_AUTHORISATION,
	    MTD_ERR_UNKNOWN_FLAGS,
    };


### Initialisation functions

#### mtd\_init - initialise the library

    int mtd_init(int flags)

   *flags* can currently be one of the following to specify the desired log
   level (defaults to MTD\_OPT\_LOG\_ERR)

        MTD_OPT_LOG_ERR
        MTD_OPT_LOG_INFO
        MTD_OPT_LOG_DEBUG

   it can also be OR'd with one of the following to specify the application
   connection type. Only used when sending fraud prevention headers

        MTD_OPT_ACT_MOBILE_APP_DIRECT
        MTD_OPT_ACT_DESKTOP_APP_DIRECT
        MTD_OPT_ACT_MOBILE_APP_VIA_SERVER
        MTD_OPT_ACT_DESKTOP_APP_VIA_SERVER
        MTD_OPT_ACT_WEB_APP_VIA_SERVER
        MTD_OPT_ACT_BATCH_PROCESS_DIRECT
        MTD_OPT_ACT_OTHER_DIRECT
        MTD_OPT_ACT_OTHER_VIA_SERVER

   when using one of the above, you should also set

        MTD_OPT_SND_ANTI_FRAUD_HDRS

   however you should read
   [Fraud Prevention Headers](#fraud-prevention-headers) to understand the
   possible privacy implications.

#### mtd\_init\_auth - initialise oauth.json

    int mtd_init_auth(void)

#### mtd\_init\_config - initialise config.json

    int mtd_init_config(void)

#### mtd\_deinit - de-initialise the library

    void mtd_deinit(void)


### Make Tax Digital - Self-Assessment API functions

    #include <libmtdac/mtd-sa.h>

#### mtd\_sa\_list\_employments

    int mtd_sa_list_employments(char **buf)

#### mtd\_sa\_get\_employment

    int mtd_sa_get_employment(const char *seid, char **buf)

#### mtd\_sa\_list\_obligations

    int mtd_sa_list_obligations(const char *seid, char **buf)

#### mtd\_sa\_list\_periods

    int mtd_sa_list_periods(const char *seid, char **buf)

#### mtd\_sa\_create\_period

    int mtd_sa_create_period(const char *src_file, const char *seid,
                             char **buf)

#### mtd\_sa\_get\_period

    int mtd_sa_get_period(const char *seid, const char *period_id,
                          char **buf)

#### mtd\_sa\_update\_period

    int mtd_sa_update_period(const char *src_file, const char *seid,
                             const char *period_id, char **buf)

#### mtd\_sa\_get\_annual\_summary

    int mtd_sa_get_annual_summary(const char *seid, const char *tax_year,
                                  char **buf)

#### mtd\_sa\_update\_annual\_summary

    int mtd_sa_update_annual_summary(const char *src_file, const char *seid,
                                     const char *tax_year, char **buf)

#### mtd\_sa\_submit\_end\_of\_period\_statement

    int mtd_sa_submit_end_of_period_statement(const char *src_file,
                                              const char *seid,
                                              const char *start,
                                              const char *end, char **buf)

#### mtd\_sa\_get\_end\_of\_period\_statement

    int mtd_sa_get_end_of_period_statement(const char *seid,
                                           const char *start,
                                           const char *end,
                                           char **buf)


### Make Tax Digital - Self-Assessment Accounts API functions [test-only]

    #include <libmtdac/mtd-saac.h>

#### mtd\_saac\_get\_balance

    int mtd_saac_get_balance(char **buf)

#### mtd\_saac\_list\_transactions

    int mtd_saac_list_transactions(const char *from, const char *to,
                                   char **buf)

#### mtd\_saac\_get\_transaction

    int mtd_saac_get_transaction(const char *trid, char **buf)

#### mtd\_saac\_list\_charges

    int mtd_saac_list_charges(const char *from, const char *to, char **buf)

#### mtd\_saac\_get\_charge

    int mtd_saac_get_charge(const char *trid, char **buf)

#### mtd\_saac\_list\_payments

    int mtd_saac_list_payments(const char *from, const char *to, char **buf)

#### mtd\_saac\_get\_payment

    int mtd_saac_get_payment(const char *pyid, char **buf)


### Make Tax Digital - Individual Calculations API functions

    #include <libmtdac/mtd-ic.h>

#### mtd\_ic\_list\_calculations

    int mtd_ic_list_calculations(char **buf)

#### mtd\_ic\_trigger\_calculation

    int mtd_ic_trigger_calculation(const char *src_file, char **buf)

#### mtd\_ic\_get\_calculation\_meta

    int mtd_ic_get_calculation_meta(const char *cid, char **buf)

#### mtd\_ic\_get\_income\_tax\_nics\_calc

    int mtd_ic_get_income_tax_nics_calc(const char *cid, char **buf)

#### mtd\_ic\_get\_taxable\_income

    int mtd_ic_get_taxable_income(const char *cid, char **buf)

#### mtd\_ic\_get\_allowances\_deductions\_reliefs

    int mtd_ic_get_allowances_deductions_reliefs(const char *cid, char **buf)

#### mtd\_ic\_get\_end\_of\_year\_est

    int mtd_ic_get_end_of_year_est(const char *cid, char **buf)

#### mtd\_ic\_get\_messages

    int mtd_ic_get_messages(const char *cid, char **buf)


### Make Tax Digital - National Insurance API functions

    #include <libmtdac/mtd-ni.h>

#### mtd\_ni\_get\_annual\_summary

    int mtd_ni_get_annual_summary(const char *utr, const char *tax_year,
                                  char **buf)


### Make Tax Digital - Create Test User API functions [test-only]

    #include <libmtdac/mtd-test-cu.h>

### mtd\_test\_cu\_create\_individual

    int mtd_test_cu_create_individual(const char *src_file, char **buf)

### mtd\_test\_cu\_create\_organisation

    int mtd_test_cu_create_organisation(const char *src_file, char **buf)

### mtd\_test\_cu\_create\_agent

    int mtd_test_cu_create_agent(const char *src_file, char **buf)

### mtd\_test\_cu\_list\_services

    int mtd_test_cu_list_services(char **buf)


### Make Tax Digital - Test Fraud Prevention Headers API functions [test-only]

    #include <libmtdac/mtd-test-fph.h>

#### mtd\_test\_fph\_validate

    int mtd_test_fph_validate(char **buf)


## Build it

The simplest way is to build the rpm by simply doing

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


## Thread safety

libmtdac should not at least be inherently thread unsafe.


## License

This library is licensed under the GNU Lesser General Public License (LGPL)
version 2.1

See *COPYING* in the repository root for details.


## Contributing

See *CodingStyle.md* & *Contributing.md*
