LIBNAME = libmtdac

GIT_VERSION  = \"$(shell git describe --long --dirty 2> /dev/null || cat ../.version)\"

SOVER	= 0
VERSION = $(SOVER).50.0

DEPDIR  := .d
$(shell mkdir -p $(DEPDIR) >/dev/null)
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td

CC	= gcc
CFLAGS	= -Wall -Wextra -Wdeclaration-after-statement -Wvla -std=gnu99 -g -O2 \
	  -Wp,-D_FORTIFY_SOURCE=2 --param=ssp-buffer-size=4 -fstack-protector \
	  -fPIC -fexceptions -fvisibility=hidden -I../include/libmtdac \
	  -DLIBNAME=\"$(LIBNAME)\" -DGIT_VERSION=${GIT_VERSION} -pipe
LDFLAGS = -shared -Wl,-z,now,-z,defs,-z,relro,--as-needed
LIBS	= -lcurl -ljansson
POSTCOMPILE = @mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d && touch $@

ifeq ($(CC),clang)
        # It is quite comminn to do STRING + N, to skip N bytes of STRING
        CFLAGS += -Wno-string-plus-int
endif

UNAME_S := $(shell uname -s | tr A-Z a-z)
ifeq ($(UNAME_S),freebsd)
        CFLAGS  += -I/usr/local/include
        # Avoid linker error about undefined symbol: environ
        LDFLAGS := $(subst defs,undefs,$(LDFLAGS))
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
%.o: %.c $(DEPDIR)/%.d
	@echo -e "  CC\t$@"
	$(v)$(CC) $(DEPFLAGS) $(CFLAGS) -c -o $@ $<
	$(POSTCOMPILE)

platform/$(UNAME_S)/%.o: platform/$(UNAME_S)/%.c
platform/$(UNAME_S)/%.o: platform/$(UNAME_S)/%.c $(DEPDIR)/%.d
	@echo -e "  CC\t$@"
	$(v)$(CC) $(DEPFLAGS) $(CFLAGS) -c -o $@ $<
	$(POSTCOMPILE)

platform/common/%.o: platform/common/%.c
platform/common/%.o: platform/common/%.c $(DEPDIR)/%.d
	@echo -e "  CC\t$@"
	$(v)$(CC) $(DEPFLAGS) $(CFLAGS) -c -o $@ $<
	$(POSTCOMPILE)

$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d

include $(wildcard $(patsubst %,$(DEPDIR)/%.d,$(basename $(sources))))

clean:
	$(v)rm -f $(objects) $(LIBNAME).so*
	$(v)rm -f $(DEPDIR)/*
	$(v)rmdir $(DEPDIR)
