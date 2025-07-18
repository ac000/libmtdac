Adding a new API
================

- Add entries to the ``mtd_api_endpoint`` enum in ``include/libmtdac/mtd.h``
- Add entry to the ``ep_api`` enum in ``src/api_endpoints.h``
- Add entry to the ``mtd_ep_api`` enum in ``include/libmtdac/mtd.h``
- Add entry to ``api_default_values[]`` in ``src/api_endpoints.h``
- Add entries to ``endpoints[]`` in ``src/api_endpoints.h`` (order should match
  above)
- Add any new error codes to the ``mtd_hmrc_error`` enum in
  ``include/libmtdac/mtd.h``
  add matching entries to ``mtd_hmrc_err_map[]`` in ``src/mtd-hmrc-err.c``
- Add any new HTTP status codes to the ``mtd_http_status_code`` enum in
  ``include/libmtdac/mtd.h``, add matching entries to
  ``http_status_code_map[]`` in ``src/curler.c``
