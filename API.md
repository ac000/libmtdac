1. [API](#api)
  * [Library version](#library-version)
  * [Errors](#errors)
  * [Initialisation functions](#initialisation-functions)
  * [Make Tax Digital - Self-Assessment API functions](#make-tax-digital---self-assessment-api-functions)
    - [Self-Assessment - Self-Employment](#self-assessment---self-employment)
    - [Self-Assessment - UK Property Business](#self-assessment---uk-property-business)
    - [Self-Assessment - Dividends Income](#self-assessment---dividends-income)
    - [Self-Assessment - Savings Accounts](#self-assessment---savings-accounts)
    - [Self-Assessment - Charitable Giving](#self-assessment---charitable-giving)
    - [Self-Assessment - Tax Calculations](#self-assessment---tax-calculations)
    - [Self-Assessment - Crystallisation](#self-assessment---crystallisation)
  * [Make Tax Digital - Self-Assessment Accounts API functions [test-only]](#make-tax-digital---self-assessment-accounts-api-functions-test-only)
  * [Make Tax Digital - Individual Calculations API functions](#make-tax-digital---individual-calculations-api-functions)
  * [Make Tax Digital - Individual Loses API functions](#make-tax-digital---individual-loses-api-functions)
    - [Individual Loses - Brought Forward](#individual-loses---brought-forward)
    - [Individual Loses - Loss Claims](#individual-loses---loss-claims)
  * [Make Tax Digital - National Insurance API functions](#make-tax-digital---national-insurance-api-functions)
  * [Make Tax Digital - Business Income Source Summary API functions](#make-tax-digital---business-income-source-summary-api-functions)
  * [Make Tax Digital - Business Source Adjustable Summary API functions](#make-tax-digital---business-source-adjustable-summary-api-functions)
    - [Business Source Adjustable Summary - Self-Employment](#business-source-adjustable-summary---self-employment)
    - [Business Source Adjustable Summary - UK Property Business](#business-source-adjustable-summary---uk-property-business)
  * [Make Tax Digital - Create Test User API functions [test-only]](#make-tax-digital---create-test-user-api-functions-test-only)
  * [Make Tax Digital - National Insurance Test Support API functions [test-only]](#make-tax-digital---national-insurance-test-support-api-functions-test-only)
  * [Make Tax Digital - Test Fraud Prevention Headers API functions [test-only]](#make-tax-digital---test-fraud-prevention-headers-api-functions-test-only)

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
	    MTD_ERR_NEEDS_AUTHORISATION,
	    MTD_ERR_UNKNOWN_FLAGS,
    };


### Initialisation functions

#### mtd\_global\_init - initialise the library (globally)

    void mtd_global_init(void)

   This function should be called before any other threads are running. In
   a non-threaded application you can skip this function and pass

       MTD_OPT_GLOBAL_INIT

   to mtd\_init() instead.

#### mtd\_init - initialise the library

    int mtd_init(int flags)

   This function should be called once in each thread where *libmtdac* is to be
   used.

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

   in a single-threaded application, when not calling mtd\_global\_init()

        MTD_OPT_GLOBAL_INIT

#### mtd\_init\_auth - initialise oauth.json

    int mtd_init_auth(void)

#### mtd\_init\_config - initialise config.json

    int mtd_init_config(void)

#### mtd\_init\_nino - initialise nino.json

    int mtd_init_nino(void)

#### mtd\_deinit - de-initialise the library

    void mtd_deinit(void)

#### mtd\_hdrs\_add - add extra headers

    void mtd_hdrs_add(const char * const hdrs[])

#### mtd\_hdrs\_reset - clear extra headers

    void mtd_hdrs_reset(void)

#### mtd\_set\_src\_data - set a buffer containing the json payload

    void mtd_set_src_data(const void *buf, size_t len)

#### mtd\_unset\_src\_data - reset/clear the json payload buffer


### Make Tax Digital - Self-Assessment API functions

    #include <libmtdac/mtd-sa.h>

#### Self-Assessment - Self-Employment

#### mtd\_sa\_se\_list\_employments

    int mtd_sa_se_list_employments(char **buf)

#### mtd\_sa\_se\_create\_employment

    int mtd_sa_se_create_employment(const char *src_file, char **buf)

#### mtd\_sa\_se\_get\_employment

    int mtd_sa_se_get_employment(const char *seid, char **buf)

#### mtd\_sa\_se\_list\_obligations

    int mtd_sa_se_list_obligations(const char *seid, char **buf)

#### mtd\_sa\_se\_list\_periods

    int mtd_sa_se_list_periods(const char *seid, char **buf)

#### mtd\_sa\_se\_create\_period

    int mtd_sa_se_create_period(const char *src_file, const char *seid,
                                char **buf)

#### mtd\_sa\_se\_get\_period

    int mtd_sa_se_get_period(const char *seid, const char *period_id,
                             char **buf)

#### mtd\_sa\_se\_update\_period

    int mtd_sa_se_update_period(const char *src_file, const char *seid,
                                const char *period_id, char **buf)

#### mtd\_sa\_se\_get\_annual\_summary

    int mtd_sa_se_get_annual_summary(const char *seid, const char *tax_year,
                                     char **buf)

#### mtd\_sa\_se\_update\_annual\_summary

    int mtd_sa_se_update_annual_summary(const char *src_file,
                                        const char *seid,
                                        const char *tax_year, char **buf)

#### mtd\_sa\_se\_submit\_end\_of\_period\_statement

    int mtd_sa_se_submit_end_of_period_statement(const char *src_file,
                                                 const char *seid,
                                                 const char *start,
                                                 const char *end,
                                                 char **buf)

#### mtd\_sa\_se\_get\_end\_of\_period\_statement

    int mtd_sa_se_get_end_of_period_statement(const char *seid,
                                              const char *query_string,
                                              char **buf)

#### Self-Assessment - UK Property Business

#### mtd\_sa\_pb\_get\_property

    int mtd_sa_pb_get_property(char **buf)

#### mtd\_sa\_pb\_create\_property

    int mtd_sa_pb_create_property(const char *src_file, char **buf)

#### mtd\_sa\_pb\_list\_obligations

    int mtd_sa_pb_list_obligations(char **buf)

#### mtd\_sa\_pb\_list\_non\_fhl\_periods

    int mtd_sa_pb_list_non_fhl_periods(char **buf)

#### mtd\_sa\_pb\_create\_non\_fhl\_period

    int mtd_sa_pb_create_non_fhl_period(const char *src_file, char **buf)

#### mtd\_sa\_pb\_get\_non\_fhl\_period

    int mtd_sa_pb_get_non_fhl_period(const char *pid, char **buf)

#### mtd\_sa\_pb\_update\_non\_fhl\_period

    int mtd_sa_pb_update_non_fhl_period(const char *src_file,
                                        const char *pid, char **buf)

#### mtd\_sa\_pb\_get\_non\_fhl\_annual\_summary

    int mtd_sa_pb_get_non_fhl_annual_summary(const char *tax_year,
                                             char **buf)

#### mtd\_sa\_pb\_update\_non\_fhl\_annual\_summary

    int mtd_sa_pb_update_non_fhl_annual_summary(const char *src_file,
                                                const char *tax_year,
                                                char **buf)

#### mtd\_sa\_pb\_list\_fhl\_periods

    int mtd_sa_pb_list_fhl_periods(char **buf)

#### mtd\_sa\_pb\_create\_fhl\_period

    int mtd_sa_pb_create_fhl_period(const char *src_file, char **buf)

#### mtd\_sa\_pb\_get\_fhl\_period

    int mtd_sa_pb_get_fhl_period(const char *pid, char **buf)

#### mtd\_sa\_pb\_update\_fhl\_period

    int mtd_sa_pb_update_fhl_period(const char *src_file, const char *pid,
                                    char **buf)

#### mtd\_sa\_pb\_get\_fhl\_annual\_summary

    int mtd_sa_pb_get_fhl_annual_summary(const char *tax_year, char **buf)

#### mtd\_sa\_pb\_update\_fhl\_annual\_summary

    int mtd_sa_pb_update_fhl_annual_summary(const char *src_file,
                                            const char *tax_year,
                                            char **buf)

#### mtd\_sa\_pb\_submit\_end\_of\_period\_statement

    int mtd_sa_pb_submit_end_of_period_statement(const char *src_file,
                                                 const char *start,
                                                 const char *end,
                                                 char **buf)

#### mtd\_sa\_pb\_get\_end\_of\_period\_statement

    int mtd_sa_pb_get_end_of_period_statement(const char *query_string,
                                              char **buf)


#### Self-Assessment - Dividends Income

#### mtd\_sa\_di\_get\_annual\_summary

    int mtd_sa_di_get_annual_summary(const char *tax_year, char **buf)

#### mtd\_sa\_di\_update\_annual\_summary

    int mtd_sa_di_update_annual_summary(const char *src_file,
                                        const char *tax_year, char **buf)

#### Self-Assessment - Savings Accounts

#### mtd\_sa\_sa\_list\_accounts

    int mtd_sa_sa_list_accounts(char **buf)

#### mtd\_sa\_sa\_create\_account

    int mtd_sa_sa_create_account(const char *src_file, char **buf)

#### mtd\_sa\_sa\_get\_account

    int mtd_sa_sa_get_account(const char *said, char **buf)

#### mtd\_sa\_sa\_get\_annual\_summary

    int mtd_sa_sa_get_annual_summary(const char *said, const char *tax_year,
                                     char **buf)

#### mtd\_sa\_sa\_update\_annual\_summary

    int mtd_sa_sa_update_annual_summary(const char *src_file, const char *said,
                                        const char *tax_year, char **buf)

#### Self-Assessment - Charitable Giving

#### mtd\_sa\_cg\_get\_charitable\_giving

    int mtd_sa_cg_get_charitable_giving(const char *tax_year, char **buf)

#### mtd\_sa\_cg\_update\_charitable\_giving

    int mtd_sa_cg_update_charitable_giving(const char *src_file,
                                           const char *tax_year, char **buf)

#### Self-Assessment - Tax Calculations

#### mtd\_sa\_tc\_calculate - [EOL July 2020]

    int mtd_sa_tc_calculate(const char *src_file, char **buf)

#### int mtd\_sa\_tc\_get\_calculation - [EOL July 2020]

    int mtd_sa_tc_get_calculation(const char *cid, char **buf)

#### mtd\_sa\_tc\_get\_validation\_msgs - [EOL July 2020]

    int mtd_sa_tc_get_validation_msgs(const char *cid, char **buf)

#### Self-Assessment - Crystallisation

#### mtd\_sa\_cr\_intent\_to\_crystallise

    int mtd_sa_cr_intent_to_crystallise(const char *tax_year, char **buf)

#### mtd\_sa\_cr\_crystallise

    int mtd_sa_cr_crystallise(const char *src_file, const char *tax_year,
                              char **buf)

#### mtd\_sa\_cr\_list\_obligations

    int mtd_sa_cr_list_obligations(const char *query_string, char **buf)


### Make Tax Digital - Self-Assessment Accounts API functions [test-only]

    #include <libmtdac/mtd-saac.h>

#### mtd\_saac\_get\_balance

    int mtd_saac_get_balance(char **buf)

#### mtd\_saac\_list\_transactions

    int mtd_saac_list_transactions(const char *query_string, char **buf)

#### mtd\_saac\_get\_transaction

    int mtd_saac_get_transaction(const char *trid, char **buf)

#### mtd\_saac\_list\_charges

    int mtd_saac_list_charges(const char *query_string, char **buf)

#### mtd\_saac\_get\_charge

    int mtd_saac_get_charge(const char *trid, char **buf)

#### mtd\_saac\_list\_payments

    int mtd_saac_list_payments(const char *query_string, char **buf)

#### mtd\_saac\_get\_payment

    int mtd_saac_get_payment(const char *pyid, char **buf)


### Make Tax Digital - Individual Calculations API functions

    #include <libmtdac/mtd-ic.h>

#### mtd\_ic\_list\_calculations

    int mtd_ic_list_calculations(const char *query_string, char **buf)

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


### Make Tax Digital - Individual Loses API functions

    #include <libmtdac/mtd-il.h>

#### Individual Loses - Brought Forward

#### mtd\_il\_bf\_list\_loses

    int mtd_il_bf_list_loses(const char *query_string, char **buf)

#### mtd\_il\_bf\_create\_loss

    int mtd_il_bf_create_loss(const char *src_file, char **buf)

#### mtd\_il\_bf\_get\_loss

    int mtd_il_bf_get_loss(const char *lid, char **buf)

#### mtd\_il\_bf\_delete\_loss

    int mtd_il_bf_delete_loss(const char *lid, char **buf)

#### mtd\_il\_bf\_update\_loss\_amnt

    int mtd_il_bf_update_loss_amnt(const char *src_file, const char *lid,
                                   char **buf)

#### Individual Loses - Loss Claims

#### mtd\_il\_lc\_list\_loses

    int mtd_il_lc_list_loses(const char *query_string, char **buf)

#### mtd\_il\_lc\_create\_loss

    int mtd_il_lc_create_loss(const char *src_file, char **buf)

#### mtd\_il\_lc\_get\_loss

    int mtd_il_lc_get_loss(const char *cid, char **buf)

#### mtd\_il\_lc\_delete\_loss

    int mtd_il_lc_delete_loss(const char *cid, char **buf)

#### mtd\_il\_lc\_update\_loss\_type

    int mtd_il_lc_update_loss_type(const char *src_file, const char *cid,
                                   char **buf)

#### mtd\_il\_lc\_update\_loss\_order

    int mtd_il_lc_update_loss_order(const char *src_file, char **buf)


### Make Tax Digital - National Insurance API functions

    #include <libmtdac/mtd-ni.h>

#### mtd\_ni\_get\_annual\_summary

    int mtd_ni_get_annual_summary(const char *utr, const char *tax_year,
                                  char **buf)


### Make Tax Digital - Business Income Source Summary API functions

    #include <libmtdac/mtd-biss.h>

#### mtd\_biss\_get\_self\_employment

    int mtd_biss_get_self_employment(const char *query_string, char **buf)

#### mtd\_biss\_get\_property

    int mtd_biss_get_property(const char *query_string, char **buf)


### Make Tax Digital - Business Source Adjustable Summary API functions

    #include <libmtdac/mtd-bsas.h>

#### mtd\_bsas\_list\_summaries

    int mtd_bsas_list_summaries(const char *query_string, char **buf)

#### mtd\_bsas\_trigger\_summary

    int mtd_bsas_trigger_summary(const char *src_file, char **buf)

#### Business Source Adjustable Summary - Self-Employment

#### mtd\_bsas\_se\_get\_summary

    int mtd_bsas_se_get_summary(const char *bid, const char *query_string,
                                char **buf)

#### mtd\_bsas\_se\_list\_summary\_adjustments

    int mtd_bsas_se_update_summary_adjustments(const char *src_file,
                                               const char *bid, char **buf

#### mtd\_bsas\_se\_update\_summary\_adjustments

    int mtd_bsas_se_update_summary_adjustments(const char *src_file,
                                               const char *bid, char **buf)

#### Business Source Adjustable Summary - UK Property Business

#### mtd\_bsas\_pb\_get\_summary

    int mtd_bsas_pb_get_summary(const char *bid, const char *query_string,
                                char **buf

#### mtd\_bsas\_pb\_list\_summary\_adjustments

    int mtd_bsas_pb_list_summary_adjustments(const char *bid, char **buf)

#### mtd\_bsas\_pb\_update\_summary\_adjustments

    int mtd_bsas_pb_update_summary_adjustments(const char *src_file,
                                               const char *bid, char **buf)


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


### Make Tax Digital - National Insurance Test Support API functions [test-only]

    #include <libmtdac/mtd-test-ni.h>

#### mtd\_test\_ni\_create\_annual\_summary

    mtd_test_ni_create_annual_summary(const char *src_file, const char *utr,
                                      const char *tax_year, char **buf)


### Make Tax Digital - Test Fraud Prevention Headers API functions [test-only]

    #include <libmtdac/mtd-test-fph.h>

#### mtd\_test\_fph\_validate

    int mtd_test_fph_validate(char **buf)
