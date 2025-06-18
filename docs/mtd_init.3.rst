.. highlight:: c

NAME
====

**mtd_init** - per-instance/thread initialisation

LIBRARY
=======

libmtdac, *-lmtdac*

SYNOPSIS
========

**#include <libmtdac/mtd.h>**

int mtd_init(unsigned int flags, const struct mtd_cfg \*cfg);

DESCRIPTION
===========

This function should be called once in each thread (or program) where
libmtdac is to be used.

**flags**
    This is a bitwise OR of one or more of the following

**MTD_OPT_LOG_ERR**
    This sets the log level to ERROR (the default)

**MTD_OPT_LOG_INFO**
    This sets the log level to INFO

**MTD_OPT_LOG_DEBUG_DATA**
    This sets the log level to DEBUG_DATA and will cause JSON payloads to
    be logged. Both those sent and received.

**MTD_OPT_LOG_DEBUG_ALL**
    This sets the log level to DEBUG_ALL and as well as the above also
    includes curls debug logging.

**MTD_OPT_LOG_DEBUG**
    This is an alias for **MTD_OPT_LOG_DEBUG_ALL**

**At most, only one of the above should be set**

**MTD_OPT_ACT_MOBILE_APP_DIRECT**
    Installed mobile application connecting directly to HMRC

**MTD_OPT_ACT_DESKTOP_APP_DIRECT**
    Installed desktop application connecting directly to HMRC

**MTD_OPT_ACT_MOBILE_APP_VIA_SERVER**
    Installed mobile application connecting through intermediary servers
    to HMRC

**MTD_OPT_ACT_DESKTOP_APP_VIA_SERVER**
    Installed desktop application connecting through intermediary servers
    to HMRC

**MTD_OPT_ACT_WEB_APP_VIA_SERVER**
    Web application connecting through intermediary servers to HMRC

**MTD_OPT_ACT_BATCH_PROCESS_DIRECT**
    Batch process connecting directly to HMRC

**MTD_OPT_ACT_OTHER_DIRECT**
    The application connects directly to HMRC but the method does not fit
    into the architectures described above (the default).

**MTD_OPT_ACT_OTHER_VIA_SERVER**
    The application connects through intermediary servers to HMRC but the
    method does not fit into the architectures described above.

    The above **MTD_OPT_ACT_** flags specify the application connection
    type, this is used by the Fraud Prevention Headers.

**MTD_OPT_GLOBAL_INIT**
    Can be used in a non-threaded application instead of calling
    **mtd_global_init(3)**

**MTD_OPT_NO_ANTI_FRAUD_HDRS**
    Used to tell libmtdac to *not* send *Fraud Prevention Headers*. However
    these are currently mandatory.

**MTD_OPT_SND_EMPTY_HDRS**
    By default, any fraud prevention headers that have no value will not be
    sent. If you wich to send them as empty headers you can set this flag.

**cfg**
    is a **struct mtd_cfg** that is used to override/provide various bits of
    library configuration

.. code-block::

  struct mtd_cfg {
          const struct mtd_fph_ops *fph_ops;
          const char * const *extra_hdrs;

          const char *config_dir;

          const FILE *log_fp;
  };


**fph_ops** is a structure for setting the fraud prevention header value
functions

.. code-block::

  struct mtd_fph_ops {
          char *(*fph_device_id)(void);
          char *(*fph_user)(void);
          char *(*fph_tz)(void);
          char *(*fph_ipaddrs)(void);
          char *(*fph_ipaddrs_ts)(void);
          char *(*fph_macaddrs)(void);
          char *(*fph_srcip)(void);
          char *(*fph_srcip_ts)(void);
          char *(*fph_srcport)(void);
          char *(*fph_screens)(void);
          char *(*fph_window_sz)(void);
          char *(*fph_browser_plugins)(void);
          char *(*fph_browser_js_ua)(void);
          char *(*fph_browser_dnt)(void);
          char *(*fph_vendor_ip)(void);
          char *(*fph_vendor_fwd)(void);
          char *(*fph_ua)(void);
          char *(*fph_multi_factor)(void);
          char *(*fph_license_id)(void);
          char *(*fph_prod_name)(void);
          char *(*fph_version)(void);
          char *(*fph_version_cli)(void);

          void *user_data;
  };

If the *fph_ops* structure or any of its members are **NULL** then the
default inbuilt functions are used.

These functions should return a dynamically allocated buffer that will be
**free(3)**'d by libmtdac.

*extra_hdrs* can be used to send extra HTTP headers, it should point to a
NULL terminated array of character pointers.

*config_dir* should point to a string containing the path to the
configuration directory for this application. E.g. "/home/foo/.config/my-app"

*log_fp* can *optionally* point to a valid file-pointer of an already opened
file for writing to have logs sent to. This doesn't affect *MTD_LOG_ERROR*
logs which are still sent to *stderr*.

RETURN VALUE
============

On success, zero (**MTD_ERR_NONE**) is returned. On error, one of the below
errors is returned.

ERRORS
======

**MTD_ERR_OS**
    There was an OS level failure.

**MTD_ERR_UNKNOWN_FLAGS**
    One or more of the supplied flags weren't recognised.

**MTD_ERR_CONFIG_DIR_UNSPEC**
    The config directory was unspecified.

**MTD_ERR_CONFIG_DIR_INVALID**
    The specified config directory could not be opened.

**MTD_ERR_NO_CONFIG**
    No config was provided.

SEE ALSO
========

**libmtdac(3)**,
**libmtdac_mtd.h(3)**,
**mtd_global_init(3)**,
**mtd_deinit(3)**
