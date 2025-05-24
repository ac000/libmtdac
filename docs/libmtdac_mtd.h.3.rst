.. highlight:: c

NAME
====

**libmtdac_mtd.h** - Core types / macros

SYNOPSIS
========

**#include <libmtdac/mtd.h>**

DESCRIPTION
===========

This header file contains the public interface to the library.

Library version
---------------

.. code-block::

  #define LIBMTDAC_MAJOR_VERSION
  #define LIBMTDAC_MINOR_VERSION
  #define LIBMTDAC_MICRO_VERSION

These are integers that contain the library version.

Log level
---------

.. code-block::

  #define MTD_OPT_LOG_ERR
  #define MTD_OPT_LOG_INFO
  #define MTD_OPT_LOG_DEBUG

These set the desired log level. Defaults to **MTD_OPT_LOG_ERR**

Connection type options
-----------------------

.. code-block::

  #define MTD_OPT_ACT_MOBILE_APP_DIRECT
  #define MTD_OPT_ACT_DESKTOP_APP_DIRECT
  #define MTD_OPT_ACT_MOBILE_APP_VIA_SERVER
  #define MTD_OPT_ACT_DESKTOP_APP_VIA_SERVER
  #define MTD_OPT_ACT_WEB_APP_VIA_SERVER
  #define MTD_OPT_ACT_BATCH_PROCESS_DIRECT
  #define MTD_OPT_ACT_OTHER_DIRECT
  #define MTD_OPT_ACT_OTHER_VIA_SERVER

These set the application connection method. **One** of these **should** be
set.

Header options
--------------

.. code-block::

  #define MTD_OPT_NO_ANTI_FRAUD_HDRS
  #define MTD_OPT_SND_EMPTY_HDRS

Don't sent anti-fraud headers and send headers that have no value rather
than skipping them respectively.

Misc. options
-------------

.. code-block::

  #define MTD_OPT_GLOBAL_INIT

Can be passed to **mtd_init(3)** in single threaded applications instead
of calling **mtd_global_init(3)**.

.. code-block::

  #define MTD_OPT_PRODUCTION_API

Use the production rather than sandbox API.

Errors (libmtdac)
-----------------

.. code-block::

  enum mtd_error {
          MTD_ERR_NONE = 0,
          MTD_ERR_OS,
          MTD_ERR_REQUEST,
          MTD_ERR_CURL,
          MTD_ERR_NEEDS_AUTHORISATION,
          MTD_ERR_UNKNOWN_FLAGS,
          MTD_ERR_UNKNOWN_SCOPES,
          MTD_ERR_LIB_TOO_OLD,
          MTD_ERR_CONFIG_DIR_UNSPEC,
          MTD_ERR_CONFIG_DIR_INVALID,
          MTD_ERR_NO_CONFIG,
          MTD_ERR_INVALID_SCOPE,

          MTD_ERR_INVALID_ERROR
  };

Errors (HMRC)
-------------

.. code-block::

  enum mtd_hmrc_error {
          MTD_HMRC_ERR_MULTIPLE = 0,

          MTD_HMRC_ERR_CHARGE_REFERENCE_INVALID,

          MTD_HMRC_ERR_CLIENT_OR_AGENT_NOT_AUTHORISED,

          MTD_HMRC_ERR_DATE_FROM_INVALID,
          MTD_HMRC_ERR_DATE_RANGE_INVALID,
          MTD_HMRC_ERR_DATE_RANGE_TOO_LARGE,
          MTD_HMRC_ERR_DATE_TO_INVALID,

          MTD_HMRC_ERR_DUPLICATE_SUBMISSION,
          MTD_HMRC_ERR_DUPLICATE_SUBMITTED_ON,

          MTD_HMRC_ERR_FORMAT_BUSINESS_ID,
          MTD_HMRC_ERR_FORMAT_BUSINESS_INCOME_2_YEARS_PRIOR,
          MTD_HMRC_ERR_FORMAT_CALC_ID,
          MTD_HMRC_ERR_FORMAT_CALCULATION_ID,
          MTD_HMRC_ERR_FORMAT_CALCULATION_TYPE,
          MTD_HMRC_ERR_FORMAT_CESSATION_DATE,
          MTD_HMRC_ERR_FORMAT_CHECKPOINT_ID,
          MTD_HMRC_ERR_FORMAT_CLASS_4_EXEMPTION_REASON,
          MTD_HMRC_ERR_FORMAT_CLASS_OF_SHARES_ACQUIRED,
          MTD_HMRC_ERR_FORMAT_CLASS_OF_SHARES_AWARDED,
          MTD_HMRC_ERR_FORMAT_COUNTRY_CODE,
          MTD_HMRC_ERR_FORMAT_CUSTOMER_REF,
          MTD_HMRC_ERR_FORMAT_DATE,
          MTD_HMRC_ERR_FORMAT_DOUBLE_TAXATION_ARTICLE,
          MTD_HMRC_ERR_FORMAT_DOUBLE_TAXATION_TREATY,
          MTD_HMRC_ERR_FORMAT_EMPLOYER_NAME,
          MTD_HMRC_ERR_FORMAT_EMPLOYER_REF,
          MTD_HMRC_ERR_FORMAT_EMPLOYMENT_ID,
          MTD_HMRC_ERR_FORMAT_END_DATE,
          MTD_HMRC_ERR_FORMAT_FINAL_DECLARATION,
          MTD_HMRC_ERR_FORMAT_FROM_DATE,
          MTD_HMRC_ERR_FORMAT_FUTURE_YEARS,
          MTD_HMRC_ERR_FORMAT_HISTORY,
          MTD_HMRC_ERR_FORMAT_LOSS_ID,
          MTD_HMRC_ERR_FORMAT_NINO,
          MTD_HMRC_ERR_FORMAT_PAYROLL_ID,
          MTD_HMRC_ERR_FORMAT_PERIOD_ID,
          MTD_HMRC_ERR_FORMAT_QOPS_REF,
          MTD_HMRC_ERR_FORMAT_REPORT_ID,
          MTD_HMRC_ERR_FORMAT_SAVINGS_ACCOUNT_ID,
          MTD_HMRC_ERR_FORMAT_SCHEME_PLAN_TYPE,
          MTD_HMRC_ERR_FORMAT_SF74_REF,
          MTD_HMRC_ERR_FORMAT_SOURCE,
          MTD_HMRC_ERR_FORMAT_START_DATE,
          MTD_HMRC_ERR_FORMAT_STATUS,
          MTD_HMRC_ERR_FORMAT_STATUS_REASON,
          MTD_HMRC_ERR_FORMAT_STRING,
          MTD_HMRC_ERR_FORMAT_SUBMISSION_ID,
          MTD_HMRC_ERR_FORMAT_SUBMITTED_ON,
          MTD_HMRC_ERR_FORMAT_TAX_YEAR,
          MTD_HMRC_ERR_FORMAT_TO_DATE,
          MTD_HMRC_ERR_FORMAT_TYPE_OF_BUSINESS,
          MTD_HMRC_ERR_FORMAT_TYPE_OF_LOSS,
          MTD_HMRC_ERR_FORMAT_VALUE,

          MTD_HMRC_ERR_INVALID_CREDENTIALS,
          MTD_HMRC_ERR_INVALID_DATE_FROM,
          MTD_HMRC_ERR_INVALID_DATE_RANGE,
          MTD_HMRC_ERR_INVALID_DATE_TO,
          MTD_HMRC_ERR_INVALID_MONETARY_AMOUNT,
          MTD_HMRC_ERR_INVALID_NUMERIC_VALUE,
          MTD_HMRC_ERR_INVALID_STATUS,
          MTD_HMRC_ERR_INVALID_TAX_YEAR_PARAMETER,

          MTD_HMRC_ERR_MATCHING_CALCULATION_ID_NOT_FOUND,
          MTD_HMRC_ERR_MATCHING_RESOURCE_NOT_FOUND,

          MTD_HMRC_ERR_MISSING_FROM_DATE,
          MTD_HMRC_ERR_MISSING_OFF_PAYROLL_WORKER,
          MTD_HMRC_ERR_MISSING_TO_DATE,
          MTD_HMRC_ERR_MISSING_TYPE_OF_BUSINESS,

          MTD_HMRC_ERR_NOT_ALLOWED_OFF_PAYROLL_WORKER,
          MTD_HMRC_ERR_NOT_FINALISED,
          MTD_HMRC_ERR_NOT_FOUND,

          MTD_HMRC_ERR_PERIOD_KEY_INVALID,

          MTD_HMRC_ERR_RANGE_TO_DATE_BEFORE_FROM_DATE,

          MTD_HMRC_ERR_RULE_ACCOUNTING_PERIOD_NOT_ENDED,
          MTD_HMRC_ERR_RULE_ACCOUNTING_PERIOD_NOT_SUPPORTED,
          MTD_HMRC_ERR_RULE_ADVANCE_SUBMISSION_REQUIRES_PERIOD_END_DATE,
          MTD_HMRC_ERR_RULE_ALLOWANCE_NOT_SUPPORTED,
          MTD_HMRC_ERR_RULE_ALREADY_ADJUSTED,
          MTD_HMRC_ERR_RULE_BFL_NOT_SUPPORTED_FOR_FHL_PROPERTIES,
          MTD_HMRC_ERR_RULE_BOTH_ALLOWANCES_SUPPLIED,
          MTD_HMRC_ERR_RULE_BOTH_EXPENSES_SUPPLIED,
          MTD_HMRC_ERR_RULE_BOTH_PROPERTIES_SUPPLIED,
          MTD_HMRC_ERR_RULE_BUILDING_NAME_NUMBER,
          MTD_HMRC_ERR_RULE_BUSINESS_ID_NOT_FOUND,
          MTD_HMRC_ERR_RULE_BUSINESS_ID_STATE_CONFLICT,
          MTD_HMRC_ERR_RULE_BUSINESS_INCOME_PERIOD_RESTRICTION,
          MTD_HMRC_ERR_RULE_BUSINESS_VALIDATION_FAILURE,
          MTD_HMRC_ERR_RULE_CALCULATION_IN_PROGRESS,
          MTD_HMRC_ERR_RULE_CALCULATION_TYPE_NOT_ALLOWED,
          MTD_HMRC_ERR_RULE_CESSATION_DATE_BEFORE_START_DATE,
          MTD_HMRC_ERR_RULE_CESSATION_DATE_BEFORE_TAX_YEAR_START,
          MTD_HMRC_ERR_RULE_COUNTRY_CODE,
          MTD_HMRC_ERR_RULE_CUSTOM_EMPLOYMENT,
          MTD_HMRC_ERR_RULE_DATE_RANGE_INVALID,
          MTD_HMRC_ERR_RULE_DECLARATION_NOT_RECEIVED,
          MTD_HMRC_ERR_RULE_DELETE_AFTER_FINAL_DECLARATION,
          MTD_HMRC_ERR_RULE_DELETE_FORBIDDEN,
          MTD_HMRC_ERR_RULE_DUPLICATE_COUNTRY_CODE,
          MTD_HMRC_ERR_RULE_DUPLICATE_SUBMISSION,
          MTD_HMRC_ERR_RULE_EARLY_DATA_SUBMISSION_NOT_ACCEPTED,
          MTD_HMRC_ERR_RULE_END_DATE_BEFORE_START_DATE,
          MTD_HMRC_ERR_RULE_END_DATE_NOT_ALIGNED_WITH_REPORTING_TYPE,
          MTD_HMRC_ERR_RULE_FINAL_DECLARATION_IN_PROGRESS,
          MTD_HMRC_ERR_RULE_FINAL_DECLARATION_RECEIVED,
          MTD_HMRC_ERR_RULE_FINAL_DECLARATION_TAX_YEAR,
          MTD_HMRC_ERR_RULE_FROM_DATE_NOT_SUPPORTED,
          MTD_HMRC_ERR_RULE_INCOME_SOURCES_CHANGED,
          MTD_HMRC_ERR_RULE_INCOME_SOURCES_INVALID,
          MTD_HMRC_ERR_RULE_INCORRECT_GOV_TEST_SCENARIO,
          MTD_HMRC_ERR_RULE_INCORRECT_OR_EMPTY_BODY_SUBMITTED,
          MTD_HMRC_ERR_RULE_INSOLVENT_TRADER,
          MTD_HMRC_ERR_RULE_INVALID_SUBMISSION_PENSION_SCHEME,
          MTD_HMRC_ERR_RULE_LUMP_SUMS,
          MTD_HMRC_ERR_RULE_MISALIGNED_PERIOD,
          MTD_HMRC_ERR_RULE_MISSING_SUBMISSION_DATES,
          MTD_HMRC_ERR_RULE_NO_ACCOUNTING_PERIOD,
          MTD_HMRC_ERR_RULE_NO_CHANGE,
          MTD_HMRC_ERR_RULE_NO_INCOME_SUBMISSIONS_EXIST,
          MTD_HMRC_ERR_RULE_NOT_ALLOWED_CONSOLIDATED_EXPENSES,
          MTD_HMRC_ERR_RULE_NOT_CONTIGUOUS_PERIOD,
          MTD_HMRC_ERR_RULE_OBLIGATIONS_NOT_MET,
          MTD_HMRC_ERR_RULE_OUTSIDE_AMENDMENT_WINDOW,
          MTD_HMRC_ERR_RULE_OVER_CONSOLIDATED_EXPENSES_THRESHOLD,
          MTD_HMRC_ERR_RULE_OVERLAPPING_PERIOD,
          MTD_HMRC_ERR_RULE_PREMATURE_FINALISATION,
          MTD_HMRC_ERR_RULE_PROPERTY_INCOME_ALLOWANCE,
          MTD_HMRC_ERR_RULE_PROPERTY_INCOME_ALLOWANCE_CLAIMED,
          MTD_HMRC_ERR_RULE_QUARTERLY_PERIOD_UPDATING,
          MTD_HMRC_ERR_RULE_RECENT_SUBMISSIONS_EXIST,
          MTD_HMRC_ERR_RULE_REQUEST_CANNOT_BE_FULFILLED,
          MTD_HMRC_ERR_RULE_RESIDENCY_CHANGED,
          MTD_HMRC_ERR_RULE_RESULTING_VALUE_NOT_PERMITTED,
          MTD_HMRC_ERR_RULE_START_AND_END_DATE_NOT_ALLOWED,
          MTD_HMRC_ERR_RULE_START_DATE_AFTER_TAX_YEAR_END,
          MTD_HMRC_ERR_RULE_START_DATE_NOT_ALIGNED_TO_COMMENCEMENT_DATE,
          MTD_HMRC_ERR_RULE_START_DATE_NOT_ALIGNED_WITH_REPORTING_TYPE,
          MTD_HMRC_ERR_RULE_SUBMISSION_END_DATE_CANNOT_MOVE_BACKWARDS,
          MTD_HMRC_ERR_RULE_SUBMISSION_FAILED,
          MTD_HMRC_ERR_RULE_SUMMARY_STATUS_INVALID,
          MTD_HMRC_ERR_RULE_SUMMARY_STATUS_SUPERSEDED,
          MTD_HMRC_ERR_RULE_TAX_YEAR_NOT_ENDED,
          MTD_HMRC_ERR_RULE_TAX_YEAR_NOT_SUPPORTED,
          MTD_HMRC_ERR_RULE_TAX_YEAR_RANGE_INVALID,
          MTD_HMRC_ERR_RULE_TO_DATE_BEFORE_FROM_DATE,
          MTD_HMRC_ERR_RULE_TRADING_INCOME_ALLOWANCE_CLAIMED,
          MTD_HMRC_ERR_RULE_TYPE_OF_BUSINESS_INCORRECT,
          MTD_HMRC_ERR_RULE_UPDATE_FORBIDDEN,
          MTD_HMRC_ERR_RULE_WRONG_TPA_AMOUNT_SUBMITTED,

          MTD_HMRC_ERR_TAX_PERIOD_NOT_ENDED,

          MTD_HMRC_ERR_VAT_NET_VALUE,
          MTD_HMRC_ERR_VAT_TOTAL_VALUE,

          MTD_HMRC_ERR_VRN_INVALID,

          /* Generic top-level errors */
          MTD_HMRC_ERR_BUSINESS_ERROR,
          MTD_HMRC_ERR_INVALID_REQUEST,

          MTD_HMRC_ERR_UNKNOWN
  };

HTTP status codes (relevant to the MTD API)
-------------------------------------------

.. code-block::

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

OAuth Scopes
------------

.. code-block::

  enum mtd_scope {
          MTD_SCOPE_RD_SA         = 0x1,
          MTD_SCOPE_WR_SA         = 0x2,
          MTD_SCOPE_RD_SAASS      = 0x4,
          MTD_SCOPE_WR_SAASS      = 0x8,
          MTD_SCOPE_RD_VAT        = 0x10,
          MTD_SCOPE_WR_VAT        = 0x20,
  };

RD = Read, WR = write, SA = Self-Assessment (ITSA). SAASS Self-Assessment
Assist (ITSA). They can be OR'd together.

They represent the various OAuth scopes that an application can be
authorised for.

.. code-block::

  enum mtd_api_scope {
          MTD_API_SCOPE_UNSET             = 0x0,

          MTD_API_SCOPE_SA                = 0x1,
          MTD_API_SCOPE_SAASS             = 0x2,
          MTD_API_SCOPE_VAT               = 0x4,

          /*
           * Special value to tell we are adding more API
           * OAuths and _not_ to reset the oauth.json file.
           *
           * This can be bitwise OR'd with any of the above.
           */
          MTD_API_SCOPE_ADD               = (1 << 29),
  };
  #define MTD_API_SCOPE_ITSA MTD_API_SCOPE_SA

What API the above scopes belong to. They can be OR'd together.
*MTD_API_SCOPE_ADD* can be used to avoid resetting the oauth.json file
when writing to it. Say you added SA but then later want to also add
SAASS...

MTD API Endpoints
-----------------

.. code-block::

  enum mtd_api_endpoint {
          /* Business Details */
          MTD_API_EP_BD_LIST = 0,
          MTD_API_EP_BD_GET,
          MTD_API_EP_BD_AMEND_QPT,

          /* Business Income Source Summary */
          MTD_API_EP_BISS_GET,

          /* Business Source Adjustable Summary */
          MTD_API_EP_BSAS_LIST,
          MTD_API_EP_BSAS_TRIGGER,
          /* Self-Employment */
          MTD_API_EP_BSAS_SE_GET,
          MTD_API_EP_BSAS_SE_SUBMIT,
          /* UK Property */
          MTD_API_EP_BSAS_PB_GET,
          MTD_API_EP_BSAS_PB_SUBMIT,
          /* Foreign Property */
          MTD_API_EP_BSAS_FP_GET,
          MTD_API_EP_BSAS_FP_SUBMIT,

          /* Individual Calculations - Tax Calculations */
          MTD_API_EP_ICAL_TRIGGER,
          MTD_API_EP_ICAL_LIST_OLD,
          MTD_API_EP_ICAL_LIST,
          MTD_API_EP_ICAL_GET,
          /* Final Declaration */
          MTD_API_EP_ICAL_FINAL_DECLARATION,

          /* Individuals Employments Income - Employments */
          MTD_API_EP_IEI_E_LIST,
          MTD_API_EP_IEI_E_ADD,
          MTD_API_EP_IEI_E_GET,
          MTD_API_EP_IEI_E_AMEND,
          MTD_API_EP_IEI_E_DELETE,
          MTD_API_EP_IEI_E_IGNORE,
          MTD_API_EP_IEI_E_UNIGNORE,
          MTD_API_EP_IEI_E_EFD_GET,
          MTD_API_EP_IEI_E_EFD_AMEND,
          MTD_API_EP_IEI_E_EFD_DELETE,
          /* Non-PAYE Employment Income */
          MTD_API_EP_IEI_NPE_GET,
          MTD_API_EP_IEI_NPE_AMEND,
          MTD_API_EP_IEI_NPE_DELETE,
          /* Other Employment Income */
          MTD_API_EP_IEI_OE_GET,
          MTD_API_EP_IEI_OE_AMEND,
          MTD_API_EP_IEI_OE_DELETE,

          /* Individual Losses - Brought Forward */
          MTD_API_EP_ILOS_BF_CREATE,
          MTD_API_EP_ILOS_BF_AMEND_AMNT,
          MTD_API_EP_ILOS_BF_LIST,
          MTD_API_EP_ILOS_BF_GET,
          MTD_API_EP_ILOS_BF_DELETE,
          /* Loss Claims */
          MTD_API_EP_ILOS_LC_CREATE,
          MTD_API_EP_ILOS_LC_LIST,
          MTD_API_EP_ILOS_LC_GET,
          MTD_API_EP_ILOS_LC_DELETE,
          MTD_API_EP_ILOS_LC_AMEND_TYPE,
          MTD_API_EP_ILOS_LC_AMEND_ORDER,

          /* Individuals Pensions Income */
          MTD_API_EP_IPI_GET,
          MTD_API_EP_IPI_AMEND,
          MTD_API_EP_IPI_DELETE,

          /* Individuals Savings Income - UK Savings Account */
          MTD_API_EP_ISI_SI_UK_LIST,
          MTD_API_EP_ISI_SI_UK_ADD,
          MTD_API_EP_ISI_SI_UK_GET_AS,
          MTD_API_EP_ISI_SI_UK_UPDATE_AS,
          /* Savings Income */
          MTD_API_EP_ISI_SI_O_GET,
          MTD_API_EP_ISI_SI_O_UPDATE,
          MTD_API_EP_ISI_SI_O_DELETE,

          /* Obligations */
          MTD_API_EP_OB_GET_IEO,
          MTD_API_EP_OB_GET_FDO,
          MTD_API_EP_OB_GET_EPSO,

          /* Property Business - UK Property Business Annual Submission */
          MTD_API_EP_PB_UKPBAS_GET,
          MTD_API_EP_PB_UKPBAS_CREATE,
          /* UK Property Income & Expenses Period Summary */
          MTD_API_EP_PB_UKPIEPS_CREATE,
          MTD_API_EP_PB_UKPIEPS_GET,
          MTD_API_EP_PB_UKPIEPS_AMEND,
          /* UK Property Cumulative Period Summary */
          MTD_API_EP_PB_UKPCPS_GET,
          MTD_API_EP_PB_UKPCPS_CREATE,
          /* Historic FHL UK Property Business Annual Submission */
          MTD_API_EP_PB_HFHL_UKPBAS_CREATE,
          MTD_API_EP_PB_HFHL_UKPBAS_GET,
          MTD_API_EP_PB_HFHL_UKPBAS_DELETE,
          /* Historic non-FHL UK Property Business Annual Submission */
          MTD_API_EP_PB_HNFHL_UKPBAS_CREATE,
          MTD_API_EP_PB_HNFHL_UKPBAS_GET,
          MTD_API_EP_PB_HNFHL_UKPBAS_DELETE,
          /* Historic FHL UK Property Income & Expenses Period Summary */
          MTD_API_EP_PB_HFHL_UKPIEPS_LIST,
          MTD_API_EP_PB_HFHL_UKPIEPS_CREATE,
          MTD_API_EP_PB_HFHL_UKPIEPS_AMEND,
          MTD_API_EP_PB_HFHL_UKPIEPS_GET,
          /* Historic non-FHL UK Property Income & Expenses Period Summary */
          MTD_API_EP_PB_HNFHL_UKPIEPS_LIST,
          MTD_API_EP_PB_HNFHL_UKPIEPS_CREATE,
          MTD_API_EP_PB_HNFHL_UKPIEPS_GET,
          MTD_API_EP_PB_HNFHL_UKPIEPS_AMEND,
          /* Foreign Property Income & Expenses Period Summary */
          MTD_API_EP_PB_FPIEPS_CREATE,
          MTD_API_EP_PB_FPIEPS_GET,
          MTD_API_EP_PB_FPIEPS_AMEND,
          /* Foreign Property Cumulative Period Summary */
          MTD_API_EP_PB_FPCPS_GET,
          MTD_API_EP_PB_FPCPS_AMEND,
          /* Foreign Property Annual Submission */
          MTD_API_EP_PB_FPAS_GET,
          MTD_API_EP_PB_FPAS_AMEND,
          /* UK or Foreign Property Annual Submission Deletion */
          MTD_API_EP_PB_AS_DELETE,
          /* UK or Foreign Property Income and Expenses Period Summaries List */
          MTD_API_EP_PB_PIEPS_LIST,

          /* Self Assessment Assist */
          MTD_API_EP_SAASS_REPORT_GEN,
          MTD_API_EP_SAASS_REPORT_ACK,

          /* Self Assessment Individual Details */
          MTD_API_EP_SAID_STATUS,

          /* Self Employment Business - Self-Employment Annual Submission */
          MTD_API_EP_SEB_SEAS_AMEND,
          MTD_API_EP_SEB_SEAS_GET,
          MTD_API_EP_SEB_SEAS_DELETE,
          /* Self-Employment Period Summaries */
          MTD_API_EP_SEB_SEPS_CREATE,
          MTD_API_EP_SEB_SEPS_LIST,
          MTD_API_EP_SEB_SEPS_AMEND,
          MTD_API_EP_SEB_SEPS_GET,
          /* Self-Employment Cumulative Period Summary */
          MTD_API_EP_SEB_SECPS_AMEND,
          MTD_API_EP_SEB_SECPS_GET,

          /* VAT */
          MTD_API_EP_VAT_LIST_OBLIGATIONS,
          MTD_API_EP_VAT_SUBMIT,
          MTD_API_EP_VAT_VIEW_RETURN,
          MTD_API_EP_VAT_GET_LIABILITIES,
          MTD_API_EP_VAT_GET_PAYMENTS,
          MTD_API_EP_VAT_GET_PENALTIES,
          MTD_API_EP_VAT_GET_FINANCIAL_DETAILS,

          /* Create Test User */
          MTD_API_EP_TEST_CU_CREATE_INDIVIDUAL,
          MTD_API_EP_TEST_CU_CREATE_ORGANISATION,
          MTD_API_EP_TEST_CU_CREATE_AGENT,
          MTD_API_EP_TEST_CU_LIST_SERVICES,

          /* Test Fraud Prevention Headers */
          MTD_API_EP_TEST_FPH_VALIDATE,
          MTD_API_EP_TEST_FPH_FEEDBACK,

          /* Self Assessment Test Support */
          MTD_API_EP_TEST_SATS_DELETE,
          /* Self Assessment Test Support Checkpoint for Vendor Data */
          MTD_API_EP_TEST_SATS_CHKPT_LIST,
          MTD_API_EP_TEST_SATS_CHKPT_CREATE,
          MTD_API_EP_TEST_SATS_CHKPT_DELETE,
          MTD_API_EP_TEST_SATS_CHKPT_RESTORE,
          /* Self Assessment Test Support Business Income Source */
          MTD_API_EP_TEST_SATS_BIS_CREATE,
          MTD_API_EP_TEST_SATS_BIS_DELETE,
          /* Self Assessment Test Support ITSA Status */
          MTD_API_EP_TEST_SATS_IS_AMEND,

          /* OAuth */
          MTD_API_EP_OA_REFRESH_TOKEN,
          MTD_API_EP_OA_EXCHANGE_AUTH_CODE,
          MTD_API_EP_OA_APPLICATION_TOKEN,
  };

Used to tell **mtd_ep(3)** which API endpoint to query.

Data Source
-----------

.. code-block::

  enum mtd_data_src_type {
          MTD_DATA_SRC_FILE = 0,
          MTD_DATA_SRC_BUF,
          MTD_DATA_SRC_FP,
          MTD_DATA_SRC_FD,
  };

  typedef union mtd_data_src {
          const void *buf;
          const char *file;
          FILE *fp;
          int fd;
  } mtd_data_src_t;

  struct mtd_dsrc_ctx {
          mtd_data_src_t data_src;
          size_t data_len;

          enum mtd_data_src_type src_type;
  };

These define the source of *POST*/*PUT* data for an endpoint.

You define a *struct mtd_dsrc_ctx* and set *data_src* to either a buffer
that contains the data, a filename of a file containing the data, a
*stdio FILE* pointer or a file descriptor of an already opened file
containing the data to send.

You then set *src_type* to the appropriate *enum mtd_data_src_type* value.

In the case of using a *buffer* you also need to set the length in bytes
of the data in the buffer via *data_len*.

Fraud Prevention Headers
------------------------

.. code-block::

  #define MTD_FPH_SET_FUNC(s, m, f)          s.m = f

.. code-block::

  #define MTD_FPH_CLI_PUBLIC_IP              fph_srcip
  #define MTD_FPH_CLI_PUBLIC_PORT            fph_srcport
  #define MTD_FPH_CLI_DEV_ID                 fph_device_id
  #define MTD_FPH_CLI_USER_ID                fph_user
  #define MTD_FPH_CLI_TZ                     fph_tz
  #define MTD_FPH_CLI_LOCAL_IPS              fph_ipaddrs
  #define MTD_FPH_CLI_MAC_ADDRS              fph_macaddrs
  #define MTD_FPH_CLI_UA                     fph_ua
  #define MTD_FPH_CLI_MULTI_FACTOR           fph_multi_factor
  #define MTD_FPH_CLI_SCREENS                fph_screens
  #define MTD_FPH_CLI_WINDOW_SZ              fph_window_sz
  #define MTD_FPH_CLI_BROWSER_PLUGINS        fph_browser_plugins
  #define MTD_FPH_CLI_BROWSER_JS_UA          fph_browser_js_ua
  #define MTD_FPH_CLI_BROWSER_DNT            fph_browser_dnt
  #define MTD_FPH_CLI_LOCAL_IPS_TS           fph_ipaddrs_ts
  #define MTD_FPH_CLI_PUBLIC_IP_TS           fph_srcip_ts
  #define MTD_FPH_VEN_VERSION                fph_version
  #define MTD_FPH_VEN_VERSION_CLI            fph_version_cli
  #define MTD_FPH_VEN_LICENSE_ID             fph_license_id
  #define MTD_FPH_VEN_PUBLIC_IP              fph_vendor_ip
  #define MTD_FPH_VEN_FWD                    fph_vendor_fwd
  #define MTD_FPH_VEN_PROD_NAME              fph_prod_name

.. code-block::

  struct mtd_fph_ops {
          char *(*fph_device_id)(void *user_data);
          char *(*fph_user)(void *user_data);
          char *(*fph_tz)(void *user_data);
          char *(*fph_ipaddrs)(void *user_data);
          char *(*fph_ipaddrs_ts)(void *user_data);
          char *(*fph_macaddrs)(void *user_data);
          char *(*fph_srcip)(void *user_data);
          char *(*fph_srcip_ts)(void *user_data);
          char *(*fph_srcport)(void *user_data);
          char *(*fph_screens)(void *user_data);
          char *(*fph_window_sz)(void *user_data);
          char *(*fph_browser_plugins)(void *user_data);
          char *(*fph_browser_js_ua)(void *user_data);
          char *(*fph_browser_dnt)(void *user_data);
          char *(*fph_vendor_ip)(void *user_data);
          char *(*fph_vendor_fwd)(void *user_data);
          char *(*fph_ua)(void *user_data);
          char *(*fph_multi_factor)(void *user_data);
          char *(*fph_license_id)(void *user_data);
          char *(*fph_version)(void *user_data);
          char *(*fph_version_cli)(void *user_data);
          char *(*fph_prod_name)(void *user_data);

          void *user_data;
  };

This can be used to override the in built functions that generate the
various fraud prevention header values. You can set any or all of them,
any that are set to NULL will use the appropriate inbuilt function.

These functions should return a pointer to a dynamically allocated buffer
that will be free(3)'d by *libmtdac*.

You should probably take a quick look at the
**[Fraud Prevention Headers](/README.md#fraud-prevention-headers)** section
of the **[README.md](/README.md)** to understand the possible privacy
implications.

Config
------

.. code-block::

  struct mtd_cfg {
          const struct mtd_fph_ops *fph_ops;
          const char * const *extra_hdrs;

          const char *config_dir;
  };

This is a structure that can be passed into **mtd_init(3)** to
provide/override configuration data.

Firstly, it can be used for overriding the fraud prevention headers. A user
could declare a struct mtd_fph_ops and set various members to their own
functions then set mtd_cfg.fph_ops to this structure and pass it into
**mtd_init(3)** e.g.

.. code-block::

  const struct mtd_fph_ops fph_ops = {
          .fph_user    = my_user,
          .fph_version = my_ver
  };

Finally you **need** to specify the directory that libmtdac will use for
its config data.

.. code-block::

  const struct mtd_cfg cfg = {
          .fph_ops    = &fph_ops,
          .config_dir = "/home/foo/.config/my-app"
  };

  err = mtd_init(flags, &cfg);

You can also use the MTD_FPH_SET_FUNC() macro, e.g.

.. code-block::

  struct mtd_fph_ops fph_ops = {};
  const struct mtd_cfg cfg = {
          .fph_ops    = &fph_ops,
          .config_dir = "/home/foo/.config/my-app"
  };

  MTD_FPH_SET_FUNC(fph_ops, MTD_FPH_CLI_USER_ID, my_user);
  MTD_FPH_SET_FUNC(fph_ops, MTD_FPH_VEN_VERSION, my_ver);
