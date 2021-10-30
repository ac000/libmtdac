LIBNAME = libmtdac

GIT_VERSION  = \"$(shell git describe --long --dirty 2> /dev/null || cat ../.version)\"

SOVER	= 0
VERSION = $(SOVER).50.0

DEPDIR  := .d
$(shell mkdir -p $(DEPDIR) >/dev/null)
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$(@F).Td

CC	= gcc
CFLAGS += -Wall -Wextra -Wdeclaration-after-statement -Wvla \
	  -Wmissing-prototypes -Wstrict-prototypes -Wold-style-definition \
	  -std=gnu99 -g -O2 -Wp,-D_FORTIFY_SOURCE=2 --param=ssp-buffer-size=4 \
	  -fno-common -fstack-protector -fPIC -fexceptions \
	  -fvisibility=hidden -I../include/libmtdac \
	  -DLIBNAME=\"$(LIBNAME)\" -DGIT_VERSION=${GIT_VERSION} -pipe
LDFLAGS = -shared -Wl,-z,now,-z,relro,-z,defs,--as-needed
LIBS	= -lcurl -ljansson
POSTCOMPILE = @mv -f $(DEPDIR)/$(@F).Td $(DEPDIR)/$(@F).d && touch $@

ifeq ($(CC),clang)
        # It is quite common to do STRING + N, to skip N bytes of STRING
        CFLAGS += -Wno-string-plus-int
endif

UNAME_S := $(shell uname -s | tr A-Z a-z)
ifeq ($(UNAME_S),freebsd)
        # Don't fail the build on unresolved symbols, environ is _always_
        # undefined. In Ci we warp the build in .ci/freebsd-clang-build.sh
        # which will decide if the build fails or not.
        LDFLAGS := $(LDFLAGS),--warn-unresolved-symbols
        CFLAGS  += -I/usr/local/include
        LIBS    := -L/usr/local/lib $(LIBS) -luuid
endif

sources = $(wildcard platform/$(UNAME_S)/*.c platform/common/*.c *.c)
objects = $(sources:.c=.o)

v = @
ifeq ($V,1)
	v =
endif

all: $(LIBNAME)

$(LIBNAME): $(objects)
	@echo -e "  LNK\t$@"
	$(v)$(CC) $(LDFLAGS),-soname,$(LIBNAME).so.$(SOVER) -o $(LIBNAME).so.$(VERSION) $(objects) $(LIBS)
	$(v)ln -sf $(LIBNAME).so.$(VERSION) $(LIBNAME).so.$(SOVER)
	$(v)ln -sf $(LIBNAME).so.$(VERSION) $(LIBNAME).so

%.o: %.c
%.o: %.c $(DEPDIR)/%.o.d
	@echo -e "  CC\t$@"
	$(v)$(CC) $(DEPFLAGS) $(CFLAGS) -c -o $@ $<
	$(POSTCOMPILE)

platform/$(UNAME_S)/%.o: platform/$(UNAME_S)/%.c
platform/$(UNAME_S)/%.o: platform/$(UNAME_S)/%.c $(DEPDIR)/%.o.d
	@echo -e "  CC\t$@"
	$(v)$(CC) $(DEPFLAGS) $(CFLAGS) -c -o $@ $<
	$(POSTCOMPILE)

platform/common/%.o: platform/common/%.c
platform/common/%.o: platform/common/%.c $(DEPDIR)/%.o.d
	@echo -e "  CC\t$@"
	$(v)$(CC) $(DEPFLAGS) $(CFLAGS) -c -o $@ $<
	$(POSTCOMPILE)

# Build headers to ensure they have all the required includes
headers = $(wildcard *.h ../include/libmtdac/*.h)
hdrobjs = $(headers:.h=.gch)

hdrchk: $(hdrobjs)
%.gch: %.h
%.gch: %.h $(DEPDIR)/%.gch.d
	@echo -e "  CC\t$@"
	$(v)$(CC) $(DEPFLAGS) $(CFLAGS) -c -o $@ $<
	$(POSTCOMPILE)

../include/libmtdac/%.gch: ../include/libmtdac/%.h
../include/libmtdac/%.gch: ../include/libmtdac/%.h $(DEPDIR)/%.gch.d
	@echo -e "  CC\t$@"
	$(v)$(CC) $(DEPFLAGS) $(CFLAGS) -c -o $@ $<
	$(POSTCOMPILE)

$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d

include $(wildcard $(patsubst %,$(DEPDIR)/%.o.d,$(basename $(sources))) $(patsubst %,$(DEPDIR)/%.gch.d,$(basename $(headers))))

clean:
	$(v)rm -f *.o platform/*/*.o $(hdrobjs) $(LIBNAME).so*
	$(v)rm -f $(DEPDIR)/*
	$(v)rmdir $(DEPDIR)
