.. highlight:: c

NAME
====

**struct mtd_hmrc_err** - linked list of structures containing the errors
from a MTD API endpoint.

LIBRARY
=======

libmtdac, *-lmtdac*

SYNOPSIS
========

**#include <libmtdac/mtd.h>**

.. code-block::

  struct mtd_hmrc_err {
          enum mtd_hmrc_error error;
          const char *code;
          const char *msg;
          struct mtd_hmrc_err *next;
  };

DESCRIPTION
===========

This is used to return error from MTD API endpoints.

It is essentially a linked list of structures allocated by
**mtd_hrmc_get_error(3)**

There will be one structure per error and subsequent structures are linked
via the *->next* member.

This should be freed by **mtd_hmrc_free_error(3)** when finished with.

*error* is one of the enum mtd_hmrc_error values.

*code* is the string value of the error code as returned by HMRC.

*msg* is a description of the error.

*next* is either *NULL* or a pointer to the next error structure.

EXAMPLE
=======

General error checking omitted for brevity.

.. code-block::

  #include <stdio.h>
  #include <stdlib.h>

  #include <libmtdac/mtd.h>

  int main(int argc, char *argv[])
  {
          static const struct mtd_cfg cfg = {
                  .config_dir = "/home/foo/.config/my-app",
          };
          struct mtd_dsrc_ctx dsctx;
          char *buf;
          int err;

          mtd_init(MTD_OPT_GLOBAL_INIT, &cfg);

          dsctx.data_src.file = argv[1];
          dsctx.src_type = MTD_DATA_SRC_FILE;

          err = mtd_ep(MTD_API_EP_BSAS_TRIGGER, &dsctx, &buf, NULL);
          if (err == MTD_ERR_REQUEST) {
                  struct mtd_hmrc_err *mhe, *mhep;

                  err = mtd_hmrc_get_error(buf, &mhe);
                  if (err == MTD_ERR_OS)
                          exit(EXIT_FAILURE);

                  for (mhep = mhe; mhep; mhep = mhep->next)
                          fprintf(stderr, "ERROR: [%s]: %s\n",
                                  mhep->code, mhep->msg);

                  mtd_hmrc_free_error(mhe);
          }

          free(buf);
          mtd_deinit();

          exit(EXIT_SUCCESS);
  }

SEE ALSO
========

**libmtdac(3)**,
**libmtdac_mtd.h(3)**,
**mtd_hmrc_get_error(3)**,
**mtd_hmrc_free_error(3)**
