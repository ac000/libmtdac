TARGETS = libmtdac

.PHONY: all $(TARGETS)
all: $(TARGETS)

MAKE_OPTS = --no-print-directory V=$V

.PHONY: libmtdac
libmtdac:
	@echo -e "Building: libmtdac"
	@$(MAKE) $(MAKE_OPTS) -C src/

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
		git archive --prefix=libmtdac-$${version%%-*}/ -o ~/rpmbuild/SOURCES/libmtdac-$${version%%-*}.tar HEAD
	@rpmbuild -bb libmtdac.spec
endif

.PHONY: clean
clean:
	@echo -e "Cleaning: libmtdac"
	@$(MAKE) $(MAKE_OPTS) -C src/ clean
