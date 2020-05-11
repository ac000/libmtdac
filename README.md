1. [Overview](#overview)
2. [API](#api)
  * [Library version](#library-version)
  * [Errors](#errors)
  * [Initialisation functions](#initialisation-functions)
  * [Make Tax Digital - Self-Assessment API functions](#make-tax-digital---self-assessment-api-functions)
3. [Build it](#build-it)
4. [How to use](#how-to-use)
5. [License](#license)
6. [Contributing](#contributing)

## Overview

An interface to the UK's HMRC [Make Tax Digital](https://developer.service.hmrc.gov.uk/api-documentation) API


## API

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

    flags should be set to either MTD_OPT_UI_CLI or MTD_OPT_UI_GUI

    it can also be OR'd with one of the following to specify the desired log
    level (defaults to MTD_OPT_LOG_ERR)

        MTD_OPT_LOG_ERR
        MTD_OPT_LOG_INFO
        MTD_OPT_LOG_DEBUG

#### mtd\_init\_auth - initialise oauth.json

    int mtd_init_auth(void)

#### mtd\_init\_config - initialise config.json

    int mtd_init_config(void)

#### mtd\_deinit - de-initialise the library

    void mtd_deinit(void)


### Make Tax Digital - Self-Assessment API functions

#### mtd\_sa\_list\_employments

    int mtd_sa_list_employments(char **buf)

#### mtd\_sa\_get\_employment

    int mtd_sa_get_employment(const char *employment_id, char **buf)

#### mtd\_sa\_list\_obligations

    int mtd_sa_list_obligations(char **buf)


#### mtd\_sa\_list\_periods

    int mtd_sa_list_periods(char **buf)

#### mtd\_sa\_create\_period

    int mtd_sa_create_period(const char *src_file, char **buf)

#### mtd\_sa\_get\_period

    int mtd_sa_get_period(const char *period_id, char **buf)

#### mtd\_sa\_update\_period

    int mtd_sa_update_period(const char *src_file, const char *period_id,
                             char **buf)

#### mtd\_sa\_get\_annual\_summary

    int mtd_sa_get_annual_summary(const char *tax_year, char **buf)

#### mtd\_sa\_update\_annual\_summary

    int mtd_sa_update_annual_summary(const char *src_file,
                                     const char *tax_year, char **buf)

#### mtd\_sa\_submit\_end\_of\_period\_statement

    int mtd_sa_submit_end_of_period_statement(const char *src_file,
                                              const char *start,
                                              const char *end, char **buf)

#### mtd\_sa\_get\_end\_of\_period\_statement

    int mtd_sa_get_end_of_period_statement(const char *start,
                                           const char *end,
                                           char **buf)


## Build it

The simplest way is to build the rpm by simply doing

    $ make rpm

in the repository root. It just needs a suitable *rpmbuild/* directory
structure which can be created with

    $ mkdir -p ~/rpmbuild/{BUILD,RPMS,SOURCES,SPECS,SRPMS}

and the *rpmbuild* tool which can be found in the *rpm-build* package.


## How to use

Just

    #include <libmtdac/mtd-sa.h>

in your program and link with *-lmtdac* assuming you've built and installed the
RPM or similar.


## Thread safety

libmtdac should not at least be inherently thread unsafe.


## License

This library is licensed under the GNU Lesser General Public License (LGPL)
version 2.1

See *COPYING* in the repository root for details.


## Contributing

See *CodingStyle.md* & *Contributing.md*
