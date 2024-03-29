1. [API](#api)
  * [Library version](#library-version)
  * [Errors](#errors)
  * [Data Source](#data-source)
  * [Fraud Prevention Headers](#fraud-prevention-headers)
  * [Config](#config)
  * [Initialisation functions](#initialisation-functions)
    - [Misc. functions](#misc-functions)
  * [Make Tax Digital - Self-Assessment API functions](#make-tax-digital---self-assessment-api-functions)
    - [Self-Assessment - Self-Employment](#self-assessment---self-employment)
    - [Self-Assessment - UK Property Business](#self-assessment---uk-property-business)
    - [Self-Assessment - Dividends Income](#self-assessment---dividends-income)
    - [Self-Assessment - Savings Accounts](#self-assessment---savings-accounts)
    - [Self-Assessment - Charitable Giving](#self-assessment---charitable-giving)
  * [Make Tax Digital - Self-Assessment Accounts API functions [test-only]](#make-tax-digital---self-assessment-accounts-api-functions-test-only)
    - [Self-Assessment Accounts - Payments and Liabilities](#self-assessment-accounts---payments-and-liabilities)
    - [Self-Assessment Accounts - Coding Out Underpayments and Debts](#self-assessment-accounts---coding-out-underpayments-and-debts)
  * [Make Tax Digital - Individuals Business End of Period Statement API functions](#make-tax-digital---individuals-business-end-of-period-statement-api-functions)
  * [Make Tax Digital - Individual Calculations API functions](#make-tax-digital---individual-calculations-api-functions)
    - [Individual Calculations - TAX Calculations](#individual-calculations---tax-calculations)
    - [Individual Calculations - Final Declaration](#individual-calculations---final-declaration)
  * [Make Tax Digital - Individual Loses API functions](#make-tax-digital---individual-loses-api-functions)
    - [Individual Loses - Brought Forward](#individual-loses---brought-forward)
    - [Individual Loses - Loss Claims](#individual-loses---loss-claims)
  * [Make Tax Digital - Individuals Disclosures API functions](#make-tax-digital---individuals-disclosures-api-functions)
    - [Individuals Disclosures - Marriage Allowance](#individuals-disclosures---marriage-allowance)
  * [Make Tax Digital - Business Income Source Summary API functions](#make-tax-digital---business-income-source-summary-api-functions)
  * [Make Tax Digital - Business Source Adjustable Summary API functions](#make-tax-digital---business-source-adjustable-summary-api-functions)
    - [Business Source Adjustable Summary - Self-Employment](#business-source-adjustable-summary---self-employment)
    - [Business Source Adjustable Summary - UK Property Business](#business-source-adjustable-summary---uk-property-business)
    - [Business Source Adjustable Summary - Foreign Property Business](#business-source-adjustable-summary---foreign-property-business)
  * [Make Tax Digital - Obligations API functions](#make-tax-digital---obligations-api-functions)
  * [Make Tax Digital - Business Details API functions](#make-tax-digital---business-details-api-functions)
  * [Make Tax Digital - VAT API functions](#make-tax-digital---vat-api-functions)
  * [Make Tax Digital - Create Test User API functions [test-only]](#make-tax-digital---create-test-user-api-functions-test-only)
  * [Make Tax Digital - Test Fraud Prevention Headers API functions [test-only]](#make-tax-digital---test-fraud-prevention-headers-api-functions-test-only)

## API

```C
#include <libmtdac/mtd.h>
```

   This header is included by each of the API specific header files.

### Library version

```C
#define LIBMTDAC_MAJOR_VERSION
#define LIBMTDAC_MINOR_VERSION
#define LIBMTDAC_MICRO_VERSION
```

### Errors

#### General library errors

```C
enum mtd_error {
        MTD_ERR_NONE,
        MTD_ERR_OS,
        MTD_ERR_REQUEST,
        MTD_ERR_CURL,
        MTD_ERR_NEEDS_AUTHORISATION,
        MTD_ERR_UNKNOWN_FLAGS,
        MTD_ERR_LIB_TOO_OLD,
        MTD_ERR_CONFIG_DIR_UNSPEC,
        MTD_ERR_CONFIG_DIR_INVALID,
        MTD_ERR_NO_CONFIG,
        MTD_ERR_INVALID_EP_API,
        MTD_ERR_INVALID_ERROR
};
```

Apart form *MTD_ERR_NONE* these errors are returned as **-**MTD\_ERR_...

#### HMRC MTD endpoint errors

```C
enum mtd_hmrc_error {
        MTD_HMRC_ERR_MULTIPLE,
        MTD_HMRC_ERR_NO_MESSAGES_PRESENT,
        MTD_HMRC_ERR_MATCHING_RESOURCE_NOT_FOUND,
        MTD_HMRC_ERR_BUSINESS_ERROR,
        MTD_HMRC_ERR_INVALID_REQUEST,
        MTD_HMRC_ERR_UNKNOWN
};
```

These represent error messages returned from the various endpoints (obviously
only a few are currently handled).

### Data Source

For requests that send (POST/PUT) data to an endpoint, the source for this data
is defined via the following interface

```C
enum mtd_data_src_type {
        MTD_DATA_SRC_FILE,
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
```

You define a *struct mtd_dsrc_ctx* and set *data_src* to either a buffer that
contains the data, a filename of a file containing the data, a *stdio FILE*
pointer or a file descriptor of an already opened file containing the data to
send.

You then set *src_type* to the appropriate *enum mtd_data_src_type* value.

In the case of using a buffer you also need to set the length in bytes of the
data in the buffer via *data_len*


### Fraud Prevention Headers

```C
#define MTD_FPH_SET_FUNC(s, m, f)
```

```C
#define MTD_FPH_CLI_PUBLIC_IP                   fph_srcip
#define MTD_FPH_CLI_PUBLIC_PORT                 fph_srcport
#define MTD_FPH_CLI_DEV_ID                      fph_device_id
#define MTD_FPH_CLI_USER_ID                     fph_user
#define MTD_FPH_CLI_TZ                          fph_tz
#define MTD_FPH_CLI_LOCAL_IPS                   fph_ipaddrs
#define MTD_FPH_CLI_MAC_ADDRS                   fph_macaddrs
#define MTD_FPH_CLI_UA                          fph_ua
#define MTD_FPH_CLI_MULTI_FACTOR                fph_multi_factor
#define MTD_FPH_CLI_SCREENS                     fph_screens
#define MTD_FPH_CLI_WINDOW_SZ                   fph_window_sz
#define MTD_FPH_CLI_BROWSER_PLUGINS             fph_browser_plugins
#define MTD_FPH_CLI_BROWSER_JS_UA               fph_browser_js_ua
#define MTD_FPH_CLI_BROWSER_DNT                 fph_browser_dnt
#define MTD_FPH_CLI_LOCAL_IPS_TS                fph_ipaddrs_ts
#define MTD_FPH_CLI_PUBLIC_IP_TS                fph_srcip_ts
#define MTD_FPH_VEN_VERSION                     fph_version
#define MTD_FPH_VEN_VERSION_CLI                 fph_version_cli
#define MTD_FPH_VEN_LICENSE_ID                  fph_license_id
#define MTD_FPH_VEN_PUBLIC_IP                   fph_vendor_ip
#define MTD_FPH_VEN_FWD                         fph_vendor_fwd
#define MTD_FPH_VEN_PROD_NAME                   fph_prod_name
```

```C
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
```

This can be used to override the in built functions that generate the various
fraud prevention header values. You can set any or all of them, any that are
set to NULL will use the appropriate inbuilt function.

These functions should return a pointer to a dynamically allocated buffer that
will be free(3)'d by the library.

You should probably take a quick look at the
[Fraud Prevention Headers](README.md#fraud-prevention-headers) section of the
**README.md** to understand the possible privacy implications.


### Config

```C
struct mtd_cfg {
        const struct mtd_fph_ops *fph_ops;
        const char * const *extra_hdrs;

        const char *config_dir;
};
```

This is a structure that can be passed into mtd\_init() to provide/override
configuration data.

Firstly, it can be used for overriding the fraud prevention headers. A user
could declare a struct mtd\_fph\_ops and set various members to their own
functions then set mtd\_cfg.fph\_ops to this structure and pass it into
mtd\_init() e.g

Finally you **need** to specify the directory that libmtdac will use for
its config data.

Typically this would be something like *${HOME}/.config/${APP_NAME}*

```C
const struct mtd_fph_ops fph_ops = {
        .fph_user = my_user,
        .fph_version = my_ver
};
const struct mtd_cfg cfg = {
        .fph_ops = &fph_ops,
        .config_dir = "/home/foo/.config/mtd-cli"
};

err = mtd_init(flags, &cfg);
```

You can also use the MTD\_FPH\_SET\_FUNC() macro, e.g

```C
struct mtd_fph_ops fph_ops = { NULL };
const struct mtd_cfg cfg = {
        .fph_ops = &fph_ops,
        .config_dir = "/home/foo/.config/mtd-cli"
};

MTD_FPH_SET_FUNC(fph_ops, MTD_FPH_CLI_USER_ID, my_user);
MTD_FPH_SET_FUNC(fph_ops, MTD_FPH_VEN_VERSION, my_ver);
```

The macro takes a struct mtd\_fph\_ops, the name of the structure member to
set (here we use the defined macro names, but you can use either) and the
function to set it to.

Secondly, you can use the *extra_hdrs* member to add extra HTTP headers to be
sent, e.g a Gov-Test-Scenario header. E.g

```C
const char *hdrs[2] = { NULL };
const struct mtd_cfg cfg = {
        .extra_hdrs = hdrs,
        .config_dir = "/home/foo/.config/mtd-cli"
};

hdrs[0] = getenv("MTD_CLI_HDRS");
```

*extra_hdrs* should point to a NULL terminated array of character pointers.


### Initialisation functions

#### mtd\_global\_init - initialise the library (globally)

```C
void mtd_global_init(void);
```

   This function should be called before any other threads are running. In
   a non-threaded application you can skip this function and pass

       MTD_OPT_GLOBAL_INIT

   to mtd\_init() instead.

#### mtd\_init - initialise the library

```C
int mtd_init(unsigned int flags, const struct mtd_cfg *cfg);
```

   This function should be called once in each thread where *libmtdac* is to be
   used.

   *flags* can currently be one of the following to specify the desired log
   level (defaults to MTD\_OPT\_LOG\_ERR)

        MTD_OPT_LOG_ERR
        MTD_OPT_LOG_INFO
        MTD_OPT_LOG_DEBUG

   it can also be OR'd with one of the following to specify the application
   connection type. Only used when sending fraud prevention headers. Defaults
   to MTD\_OPT\_ACT\_OTHER\_DIRECT if not specified.

        MTD_OPT_ACT_MOBILE_APP_DIRECT
        MTD_OPT_ACT_DESKTOP_APP_DIRECT
        MTD_OPT_ACT_MOBILE_APP_VIA_SERVER
        MTD_OPT_ACT_DESKTOP_APP_VIA_SERVER
        MTD_OPT_ACT_WEB_APP_VIA_SERVER
        MTD_OPT_ACT_BATCH_PROCESS_DIRECT
        MTD_OPT_ACT_OTHER_DIRECT
        MTD_OPT_ACT_OTHER_VIA_SERVER

   if you don't wish to send fraud prevention headers then you can set

        MTD_OPT_NO_ANTI_FRAUD_HDRS

   however they are mandatory...

   for headers that have no value, by default we will skip sending that
   header, however if you wish to send an empty header you can set

        MTD_OPT_SND_EMPTY_HDRS

   in a single-threaded application, when not calling mtd\_global\_init()

        MTD_OPT_GLOBAL_INIT

*cfg* is as described in [Config](#config) above.


#### mtd\_init\_auth - initialise oauth.json

```C
int mtd_init_auth(enum mtd_ep_api api, unsigned long scopes);
```

This function takes an enum describing what API (ITSA, VAT etc) this is adding
authorisations for, it takes one of the following

        MTD_EP_API_ITSA
        MTD_EP_API_VAT

the above can be optionally bitwise OR'd with

        MTD_EP_API_ADD

which says don't reset the oauth.json file when writing it, instead add this
OAuth as another entry.

The second argument is a bitwise OR'd list of scopes that the application
should be authorised for, the scopes available are

        MTD_SCOPE_RD_SA
        MTD_SCOPE_WR_SA
        MTD_SCOPE_RD_VAT
        MTD_SCOPE_WR_VAT

#### mtd\_init\_creds - initialise creds.json

```C
int mtd_init_creds(enum mtd_ep_api api);
```

#### mtd\_init\_nino - initialise nino.json

```C
int mtd_init_nino(void);
```

#### mtd\_deinit - de-initialise the library

```C
void mtd_deinit(void);
```

### Misc. functions

#### mtd\_percent\_encode - percent encode a string

```C
char *mtd_percent_encode(const char *str, ssize_t len);
```

#### mtd\_err2str - get a textual description of the given error code

```C
const char *mtd_err2str(int err);
```

#### mtd\_err2enum\_str - get a string version of the given error code enum

```C
const char *mtd_err2enum_str(int err);
```

#### mtd\_http\_status\_code - return the HTTP status code of the last request

```C
enum mtd_http_status_code mtd_http_status_code(const char *json);
```

Which returns one of the following enum values, which are mapped to the actual
status code value

```
        MTD_HTTP_OK
        MTD_HTTP_CREATED
        MTD_HTTP_ACCEPTED
        MTD_HTTP_NO_CONTENT
        MTD_HTTP_SEE_OTHER
        MTD_HTTP_BAD_REQUEST
        MTD_HTTP_UNAUTHORIZED
        MTD_HTTP_FORBIDDEN
        MTD_HTTP_NOT_FOUND
        MTD_HTTP_METHOD_NOT_ALLOWED
        MTD_HTTP_NOT_ACCEPTABLE
        MTD_HTTP_GONE
        MTD_HTTP_REQUEST_ENTITY_TOO_LARGE
        MTD_HTTP_UNSUPPORTED_MEDIA_TYPE
        MTD_HTTP_TOO_MANY_REQUESTS
        MTD_HTTP_INTERNAL_SERVER_ERROR
        MTD_HTTP_NOT_IMPLEMENTED
        MTD_HTTP_SERVICE_UNAVAILABLE
        MTD_HTTP_GATEWAY_TIMEOUT
```

#### mtd\_http\_status\_str\_u - return a string version of the enum

```C
const char *mtd_http_status_str_u(const char *json);
````

#### mtd\_http\_status\_str - return a string version of the status code

```C
const char *mtd_http_status_str(const char *json);
```

#### mtd\_hmrc\_error - return an enum representing an endpoint error code

```C
enum mtd_hmrc_error mtd_hmrc_error(const char *json);
```


### Make Tax Digital - Self-Assessment API functions

```C
#include <libmtdac/mtd-sa.h>
```

#### Self-Assessment - Self-Employment

#### mtd\_sa\_se\_create\_employment

```C
int mtd_sa_se_create_employment(const struct mtd_dsrc_ctx *dsctx,
                                char **buf)
```

#### mtd\_sa\_se\_list\_periods

```C
int mtd_sa_se_list_periods(const char *seid, char **buf);
```

#### mtd\_sa\_se\_create\_period

```C
int mtd_sa_se_create_period(const struct mtd_dsrc_ctx *dsctx,
                            const char *seid, char **buf);
```

#### mtd\_sa\_se\_get\_period

```C
int mtd_sa_se_get_period(const char *seid, const char *period_id,
                         char **buf);
```

#### mtd\_sa\_se\_update\_period

```C
int mtd_sa_se_update_period(const struct mtd_dsrc_ctx *dsctx,
                            const char *seid, const char *period_id,
                            char **buf);
```

#### mtd\_sa\_se\_get\_annual\_summary

```C
int mtd_sa_se_get_annual_summary(const char *seid, const char *tax_year,
                                 char **buf);
```

#### mtd\_sa\_se\_update\_annual\_summary

```C
int mtd_sa_se_update_annual_summary(const struct mtd_dsrc_ctx *dsctx,
                                    const char *seid,
                                    const char *tax_year, char **buf);
```


#### Self-Assessment - UK Property Business

#### mtd\_sa\_pb\_create\_property

```C
int mtd_sa_pb_create_property(const struct mtd_dsrc_ctx *dsctx,
                              char **buf);
```

#### mtd\_sa\_pb\_list\_non\_fhl\_periods

```C
int mtd_sa_pb_list_non_fhl_periods(char **buf);
```

#### mtd\_sa\_pb\_create\_non\_fhl\_period

```C
int mtd_sa_pb_create_non_fhl_period(const struct mtd_dsrc_ctx *dsctx,
                                    char **buf);
```

#### mtd\_sa\_pb\_get\_non\_fhl\_period

```C
int mtd_sa_pb_get_non_fhl_period(const char *pid, char **buf);
```

#### mtd\_sa\_pb\_update\_non\_fhl\_period

```C
int mtd_sa_pb_update_non_fhl_period(const struct mtd_dsrc_ctx *dsctx,
                                    const char *pid, char **buf);
```

#### mtd\_sa\_pb\_get\_non\_fhl\_annual\_summary

```C
int mtd_sa_pb_get_non_fhl_annual_summary(const char *tax_year,
                                         char **buf);
```

#### mtd\_sa\_pb\_update\_non\_fhl\_annual\_summary

```C
int mtd_sa_pb_update_non_fhl_annual_summary(const struct mtd_dsrc_ctx *dsctx,
                                            const char *tax_year,
                                            char **buf);
```

#### mtd\_sa\_pb\_list\_fhl\_periods

```C
int mtd_sa_pb_list_fhl_periods(char **buf);
```

#### mtd\_sa\_pb\_create\_fhl\_period

```C
int mtd_sa_pb_create_fhl_period(const struct mtd_dsrc_ctx *dsctx,
                                char **buf);
```

#### mtd\_sa\_pb\_get\_fhl\_period

```C
int mtd_sa_pb_get_fhl_period(const char *pid, char **buf);
```

#### mtd\_sa\_pb\_update\_fhl\_period

```C
int mtd_sa_pb_update_fhl_period(const struct mtd_dsrc_ctx *dsctx,
                                const char *pid, char **buf);
```

#### mtd\_sa\_pb\_get\_fhl\_annual\_summary

```C
int mtd_sa_pb_get_fhl_annual_summary(const char *tax_year, char **buf);
```

#### mtd\_sa\_pb\_update\_fhl\_annual\_summary

```C
int mtd_sa_pb_update_fhl_annual_summary(const struct mtd_dsrc_ctx *dsctx,
                                        const char *tax_year,
                                        char **buf);
```


#### Self-Assessment - Dividends Income

#### mtd\_sa\_di\_get\_annual\_summary

```C
int mtd_sa_di_get_annual_summary(const char *tax_year, char **buf);
```

#### mtd\_sa\_di\_update\_annual\_summary

```C
int mtd_sa_di_update_annual_summary(const struct mtd_dsrc_ctx *dsctx,
                                    const char *tax_year, char **buf);
```


#### Self-Assessment - Savings Accounts

#### mtd\_sa\_sa\_list\_accounts

```C
int mtd_sa_sa_list_accounts(char **buf);
```

#### mtd\_sa\_sa\_create\_account

```C
int mtd_sa_sa_create_account(const struct mtd_dsrc_ctx *dsctx, char **buf);
```

#### mtd\_sa\_sa\_get\_account

```C
int mtd_sa_sa_get_account(const char *said, char **buf);
```

#### mtd\_sa\_sa\_get\_annual\_summary

```C
int mtd_sa_sa_get_annual_summary(const char *said, const char *tax_year,
                                 char **buf);
```

#### mtd\_sa\_sa\_update\_annual\_summary

```C
int mtd_sa_sa_update_annual_summary(const struct mtd_dsrc_ctx *dsctx,
                                    const char *said, const char *tax_year,
                                    char **buf);
```


#### Self-Assessment - Charitable Giving

#### mtd\_sa\_cg\_get\_charitable\_giving

```C
int mtd_sa_cg_get_charitable_giving(const char *tax_year, char **buf);
```

#### mtd\_sa\_cg\_update\_charitable\_giving

```C
int mtd_sa_cg_update_charitable_giving(const struct mtd_dsrc_ctx *dsctx,
                                       const char *tax_year, char **buf);
```


### Make Tax Digital - Individuals Business End of Period Statement API functions

```C
#include <libmtdac/mtd-ibeops.h>
```

#### mtd\_ibeops\_submit\_eops

```C
int mtd_ibeops_submit_eops(const struct mtd_dsrc_ctx *dsctx, char **buf);
```


### Make Tax Digital - Self-Assessment Accounts API functions [test-only]

```C
#include <libmtdac/mtd-saac.h>
```

#### Self-Assessment Accounts - Payments and Liabilities

#### mtd\_saac\_get\_balance

```C
int mtd_saac_get_balance(char **buf);
```

#### mtd\_saac\_list\_transactions

```C
int mtd_saac_list_transactions(const char *query_string, char **buf);
```

#### mtd\_saac\_get\_transaction

```C
int mtd_saac_get_transaction(const char *trid, char **buf);
```

#### mtd\_saac\_list\_charges

```C
int mtd_saac_list_charges(const char *query_string, char **buf);
```

#### mtd\_saac\_get\_charge

```C
int mtd_saac_get_charge(const char *trid, char **buf);
```

#### mtd\_saac\_list\_payments

```C
int mtd_saac_list_payments(const char *query_string, char **buf);
```

#### mtd\_saac\_get\_payment

```C
int mtd_saac_get_payment(const char *pyid, char **buf);
```

#### Self-Assessment Accounts - Coding Out Underpayments and Debts

#### mtd\_saac\_get\_coding\_out\_uda

```C
int mtd_saac_get_coding_out_uda(const char *tax_year, char **buf);
```

#### mtd\_saac\_set\_coding\_out\_uda

```C
int mtd_saac_set_coding_out_uda(const struct mtd_dsrc_ctx *dsctx,
                                const char *tax_year, char **buf);
```

#### mtd\_saac\_delete\_coding\_out\_uda

```C
int mtd_saac_delete_coding_out_uda(const char *tax_year, char **buf);
```


### Make Tax Digital - Individual Calculations API functions

```C
#include <libmtdac/mtd-ic.h>
```

#### Individual Calculations - TAX Calculations

#### mtd\_ic\_list\_calculations

```C
int mtd_ic_list_calculations(const char *query_string, char **buf);
```

#### mtd\_ic\_trigger\_calculation

```C
int mtd_ic_trigger_calculation(const char *tax_year, const char *query_string,
                               char **buf);
```

#### mtd\_ic\_get\_calculation

```C
int mtd_ic_get_calculation(const char *tax_year, const char *cid, char **buf);
```


#### Individual Calculations - Final Declaration

#### mtd\_ic\_final\_decl

```C
int mtd_ic_final_decl(const char *tax_year, const char *cid, char **buf);
```


### Make Tax Digital - Individual Loses API functions

```C
#include <libmtdac/mtd-il.h>
```

#### Individual Loses - Brought Forward

#### mtd\_il\_bf\_list\_loses

```C
int mtd_il_bf_list_loses(const char *query_string, char **buf);
```

#### mtd\_il\_bf\_create\_loss

```C
int mtd_il_bf_create_loss(const struct mtd_dsrc_ctx *dsctx, char **buf);
```

#### mtd\_il\_bf\_get\_loss

```C
int mtd_il_bf_get_loss(const char *lid, char **buf);
```

#### mtd\_il\_bf\_delete\_loss

```C
int mtd_il_bf_delete_loss(const char *lid, char **buf);
```

#### mtd\_il\_bf\_update\_loss\_amnt

```C
int mtd_il_bf_update_loss_amnt(const struct mtd_dsrc_ctx *dsctx,
                               const char *lid, char **buf);
```


#### Individual Loses - Loss Claims

#### mtd\_il\_lc\_list\_loses

```C
int mtd_il_lc_list_loses(const char *query_string, char **buf);
```

#### mtd\_il\_lc\_create\_loss

```C
int mtd_il_lc_create_loss(const struct mtd_dsrc_ctx *dsctx, char **buf);
```

#### mtd\_il\_lc\_get\_loss

```C
int mtd_il_lc_get_loss(const char *cid, char **buf);
```

#### mtd\_il\_lc\_delete\_loss

```C
int mtd_il_lc_delete_loss(const char *cid, char **buf);
```


#### mtd\_il\_lc\_update\_loss\_type

```C
int mtd_il_lc_update_loss_type(const struct mtd_dsrc_ctx *dsctx,
                               const char *cid, char **buf);
```

#### mtd\_il\_lc\_update\_loss\_order [test-only]

```C
int mtd_il_lc_update_loss_order(const struct mtd_dsrc_ctx *dsctx,
                                const char *tax_year, char **buf);
```


### Make Tax Digital - Individuals Disclosures API functions

```C
#include <libmtdac/mtd-id.h>
```

#### mtd\_id\_get

```C
int mtd_id_get(const char *tax_year, char **buf);
```

#### mtd\_id\_set

```C
int mtd_id_set(const struct mtd_dsrc_ctx *dsctx, const char *tax_year,
               char **buf);
```

#### mtd\_id\_delete

```C
int mtd_id_delete(const char *tax_year, char **buf);
```


#### Individuals Disclosures - Marriage Allowance

#### mtd\_id\_ma\_create

```C
int mtd_id_ma_create(const struct mtd_dsrc_ctx *dsctx, char **buf);
```


### Make Tax Digital - Business Income Source Summary API functions

```C
#include <libmtdac/mtd-biss.h>
```

#### mtd\_biss\_get\_summary

```C
int mtd_biss_get_summary(const char *business_type, const char *tax_year,
                         const char *bid, char **buf);
```


### Make Tax Digital - Business Source Adjustable Summary API functions

```C
#include <libmtdac/mtd-bsas.h>
```

#### mtd\_bsas\_list\_summaries

```C
int mtd_bsas_list_summaries(const char *query_string, char **buf);
```

#### mtd\_bsas\_trigger\_summary

```C
int mtd_bsas_trigger_summary(const struct mtd_dsrc_ctx *dsctx, char **buf);
```


#### Business Source Adjustable Summary - Self-Employment

#### mtd\_bsas\_se\_get\_summary

```C
int mtd_bsas_se_get_summary(const char *cid, char **buf);
```

#### mtd\_bsas\_se\_update\_summary\_adjustments

```C
int mtd_bsas_se_update_summary_adjustments(const struct mtd_dsrc_ctx *dsctx,
                                           const char *cid, char **buf);
```


#### Business Source Adjustable Summary - UK Property Business

#### mtd\_bsas\_pb\_get\_summary

```C
int mtd_bsas_pb_get_summary(const char *cid, char **buf);
```

#### mtd\_bsas\_pb\_update\_summary\_adjustments

```C
int mtd_bsas_pb_update_summary_adjustments(const struct mtd_dsrc_ctx *dsctx,
                                           const char *cid, char **buf);
```


#### Business Source Adjustable Summary - Foreign Property Business

#### mtd\_bsas\_fp\_get\_summary

```C
int mtd_bsas_fp_get_summary(const char *cid, char **buf);
```

#### mtd\_bsas\_fp\_update\_summary\_adjustments

```C
int mtd_bsas_fp_update_summary_adjustments(const struct mtd_dsrc_ctx *dsctx,
                                           const char *cid, char **buf);
```


### Make Tax Digital - Obligations API functions

```C
#include <libmtdac/mtd-ob.h>
```

#### mtd\_ob\_list\_inc\_and\_expend\_obligations

```C
int mtd_ob_list_inc_and_expend_obligations(const char *query_string,
                                           char **buf);
```

#### mtd\_ob\_list\_crystallisation\_obligations

```C
int mtd_ob_list_crystallisation_obligations(const char *query_string,
                                            char **buf);
```

#### mtd\_ob\_list\_end\_of\_period\_obligations

```C
int mtd_ob_list_end_of_period_obligations(const char *query_string,
                                          char **buf);
```


### Make Tax Digital - Business Details API functions

```C
#include <libmtdac/mtd-bd.h>
```

#### mtd\_bd\_list

```C
int mtd_bd_list(char **buf);
```

#### mtd\_bd\_get

```C
int mtd_bd_get(const char *bid, char **buf);
```


### Make Tax Digital - VAT API functions

```C
#include <libmtdac/mtd-vat.h>
```

#### mtd\_vat\_list\_obligations

```C
int mtd_vat_list_obligations(const char *vrn, const char *query_string,
                             char **buf);
```

#### mtd\_vat\_submit\_period

```C
int mtd_vat_submit_period(const struct mtd_dsrc_ctx *dsctx, const char *vrn,
                          char **buf);
```

#### mtd\_vat\_get\_period

```C
int mtd_vat_get_period(const char *vrn, const char *period_key, char **buf);
```

#### mtd\_vat\_list\_liabilities

```C
int mtd_vat_list_liabilities(const char *vrn, const char *query_string,
                             char **buf);
```


#### mtd\_vat\_list\_payments

```C
int mtd_vat_list_payments(const char *vrn, const char *query_string,
                          char **buf);
```


### Make Tax Digital - Create Test User API functions [test-only]

```C
#include <libmtdac/mtd-test-cu.h>
```

#### mtd\_test\_cu\_create\_individual

```C
int mtd_test_cu_create_individual(const struct mtd_dsrc_ctx *dsctx,
                                  char **buf);
```

#### mtd\_test\_cu\_create\_organisation

```C
int mtd_test_cu_create_organisation(const struct mtd_dsrc_ctx *dsctx,
                                    char **buf);
```

#### mtd\_test\_cu\_create\_agent

```C
int mtd_test_cu_create_agent(const struct mtd_dsrc_ctx *dsctx, char **buf);
```

#### mtd\_test\_cu\_list\_services

```C
int mtd_test_cu_list_services(char **buf);
```


### Make Tax Digital - Test Fraud Prevention Headers API functions [test-only]

```C
#include <libmtdac/mtd-test-fph.h>
```

#### mtd\_test\_fph\_validate

```C
int mtd_test_fph_validate(char **buf);
```

#### mtd\_test\_fph\_feedback

```C
int mtd_test_fph_feedback(const char *api, const char *query_string,
                          char **buf);
```
