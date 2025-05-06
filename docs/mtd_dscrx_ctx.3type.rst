.. highlight:: c

NAME
====

**struct mtd_dsrc_ctx** - describes the source for the JSON payload to be sent

LIBRARY
=======

libmtdac, *-lmtdac*

SYNOPSIS
========

**#include <libmtdac/mtd.h>**

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

DESCRIPTION
===========

This is used to define the source of the JSON payload data to be sent to
*POST*/*PUT* endpoints that require a request body.

You define a *struct mtd_dsrc_ctx* and set *data_src* to either a buffer
that contains the data, a filename of a file containing the data, a stdio
*FILE ** pointer or a file descriptor of an already opened file containing
the data to send.

You then set *src_type* to the appropriate *enum mtd_data_src_type* value.

In the case of using a buffer you also need to set the length in bytes of the
data in the buffer via *data_len*.

EXAMPLE
=======

Examples showing the four use cases, error checking omitted for brevity.

Specifying a filename
---------------------

.. code-block::

  #include <stdio.h>
  #include <stdlib.h>

  #include <libmtdac/mtd.h>

  int main(int argc, char *argv[])
  {
          const struct mtd_cfg cfg = {
                  .config_dir = "/home/foo/.config/my-app",
          };
          struct mtd_dsrc_ctx dsctx;
          char *buf;

          mtd_init(MTD_OPT_GLOBAL_INIT, &cfg);

          dsctx.data_src.file = argv[1];
          dsctx.src_type = MTD_DATA_SRC_FILE;

          mtd_ep(MTD_API_EP_BSAS_TRIGGER, &dsctx, &buf, NULL);

          printf("%s\n", buf);

          free(buf);
          mtd_deinit();

          exit(EXIT_SUCCESS);
  }

Using a buffer
--------------

.. code-block::

  #include <stdio.h>
  #include <stdlib.h>

  #include <libmtdac/mtd.h>

  int main(void)
  {
          const struct mtd_cfg cfg = {
                  .config_dir = "/home/foo/.config/my-app",
          };
          struct mtd_dsrc_ctx dsctx;
          char json[1024];
          char *buf;

          mtd_init(MTD_OPT_GLOBAL_INIT, &cfg);

          dsctx.data_len =
                  snprintf(json, sizeof(json),
                           "{\"accountingPeriod\": {"
                           "\"startDate\": \"2024-04-06\","
                           "\"endDate\": \"2025-04-05\"},"
                           "\"typeOfBusiness\": \"self-employment\","
                           "\"businessId\": \"XVIS12345678901\"}");

          dsctx.data_src.buf = json;
          dsctx.src_type = MTD_DATA_SRC_BUF;

          mtd_ep(MTD_API_EP_BSAS_TRIGGER, &dsctx, &buf, NULL);

          printf("%s\n", buf);

          free(buf);
          mtd_deinit();

          exit(EXIT_SUCCESS);
  }

Using a FILE *
--------------

.. code-block::

  #include <stdio.h>
  #include <stdlib.h>

  #include <libmtdac/mtd.h>

  int main(int argc, char *argv[])
  {
          const struct mtd_cfg cfg = {
                  .config_dir = "/home/foo/.config/my-app",
          };
          struct mtd_dsrc_ctx dsctx;
          char *buf;

          mtd_init(MTD_OPT_GLOBAL_INIT, &cfg);

          dsctx.data_src.fp = fopen(argv[1], "re");
          dsctx.src_type = MTD_DATA_SRC_FP;

          mtd_ep(MTD_API_EP_BSAS_TRIGGER, &dsctx, &buf, NULL);

          printf("%s\n", buf);

          fclose(dsctx.data_src.fp);
          free(buf);

          mtd_deinit();

          exit(EXIT_SUCCESS);
  }

Using a file descriptor
-----------------------

.. code-block::

  #include <stdio.h>
  #include <stdlib.h>
  #include <fcntl.h>
  #include <unistd.h>

  #include <libmtdac/mtd.h>

  int main(int argc, char *argv[])
  {
          const struct mtd_cfg cfg = {
                  .config_dir = "/home/foo/.config/my-app",
          };
          struct mtd_dsrc_ctx dsctx;
          char *buf;

          mtd_init(MTD_OPT_GLOBAL_INIT, &cfg);

          dsctx.data_src.fd = open(argv[1], O_RDONLY);
          dsctx.src_type = MTD_DATA_SRC_FD;

          mtd_ep(MTD_API_EP_BSAS_TRIGGER, &dsctx, &buf, NULL);

          printf("%s\n", buf);

          close(dsctx.data_src.fd);
          free(buf);

          mtd_deinit();

          exit(EXIT_SUCCESS);
  }

SEE ALSO
========

**libmtdac(3)**,
**libmtdac_mtd.h(3)**,
**libmtdac-result_buffer(3type)**
