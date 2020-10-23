TARGETS = libmtdac man

.PHONY: all $(TARGETS)
all: $(TARGETS)

MAKE_OPTS = --no-print-directory V=$V

.PHONY: libmtdac
libmtdac:
	@echo -e "Building: libmtdac"
	@$(MAKE) $(MAKE_OPTS) -C src/

.PHONY: man
man:
	@echo -e "Building: man-pages"
	@$(MAKE) $(MAKE_OPTS) -C man/man3/

.PHONY: rpm
rpm:
	@echo -e "Building: rpm"
ifeq ($(wildcard ~/rpmbuild/),)
	@echo "***"
	@echo "*** ~/rpmbuild not found, create with"
	@echo "***"
	@echo "***    mkdir -p ~/rpmbuild/{BUILD,RPMS,SOURCES,SPECS,SRPMS}"
	@echo "***"
	@false
else
	@version=$$(git describe | tail -c +2); echo "Building $${version}"; \
		git archive --prefix=libmtdac-$${version%%-*}/ -o ~/rpmbuild/SOURCES/libmtdac-$${version%%-*}.tar HEAD; \
		git describe | tail -c +2 > .version; \
		tar -rf ~/rpmbuild/SOURCES/libmtdac-$${version%%-*}.tar --transform "s,^,libmtdac-$${version%%-*}/," .version
	@rpmbuild -bb libmtdac.spec
endif

.PHONY: clean
clean:
	@echo -e "Cleaning: libmtdac man"
	@$(MAKE) $(MAKE_OPTS) -C src/ clean
	@$(MAKE) $(MAKE_OPTS) -C man/man3 clean
	@rm -f .version
