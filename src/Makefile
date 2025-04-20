LIBNAME = libmtdac

GIT_VERSION  = \"$(shell git describe --long --dirty 2> /dev/null || cat ../.version)\"

SOVER	= 0
VERSION = $(SOVER).90.0

OBJ_DIR := .objs
$(shell mkdir -p $(OBJ_DIR) >/dev/null)

DEPDIR  := .d
$(shell mkdir -p $(DEPDIR) >/dev/null)
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$(@F).Td

CC	= gcc
CFLAGS += -Wall -Wextra -Wdeclaration-after-statement -Wvla \
	  -Wmissing-prototypes -Wstrict-prototypes -Wold-style-definition \
	  -std=gnu11 -g -O2 -Wp,-D_FORTIFY_SOURCE=2 --param=ssp-buffer-size=4 \
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
        # undefined. In the CI we wrap the build in
        # .ci/freebsd-clang-build.sh which will decide if the build fails
        # or not.
        LDFLAGS := $(LDFLAGS),--warn-unresolved-symbols
        CFLAGS  += -I/usr/local/include
        LIBS    := -L/usr/local/lib $(LIBS) -luuid
endif

sources := $(wildcard platform/$(UNAME_S)/*.c platform/common/*.c *.c)
# We just want the sources without any directory prefix to
# make $(DEPDIR)/%.o.d work otherwise % expands out to a
# full path. vpath is used to allow these source files to
# be found.
sources := $(notdir $(sources))
objects := $(patsubst %.c,$(OBJ_DIR)/%.o,$(sources))

v = @
ifeq ($V,1)
	v =
endif

all: $(LIBNAME)

$(LIBNAME): $(objects)
	@echo "  LNK  $@.so"
	$(v)$(CC) $(LDFLAGS),-soname,$(LIBNAME).so.$(SOVER) -o $(LIBNAME).so.$(VERSION) $(objects) $(LIBS)
	$(v)ln -sf $(LIBNAME).so.$(VERSION) $(LIBNAME).so.$(SOVER)
	$(v)ln -sf $(LIBNAME).so.$(VERSION) $(LIBNAME).so

# Sources are stored without directory prefix
vpath %.c platform/$(UNAME_S):platform/common

%.o: %.c
$(OBJ_DIR)/%.o: %.c $(DEPDIR)/%.o.d
	@echo "  CC   $< => $@"
	$(v)$(CC) $(DEPFLAGS) $(CFLAGS) -c -o $@ $<
	$(POSTCOMPILE)

# Build headers to ensure they have all the required includes
headers := $(wildcard *.h ../include/libmtdac/*.h)
headers := $(notdir $(headers))
hdrobjs := $(patsubst %.h,$(OBJ_DIR)/%.gch,$(headers))

vpath %.h ../include/libmtdac

hdrchk: $(hdrobjs)
%.gch: %.h
$(OBJ_DIR)/%.gch: %.h $(DEPDIR)/%.gch.d
	@echo "  CC   $< => $@"
	$(v)$(CC) $(DEPFLAGS) $(CFLAGS) -c -o $@ $<
	$(POSTCOMPILE)

$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d

include $(wildcard $(patsubst %,$(DEPDIR)/%.o.d,$(basename $(sources))))
include $(wildcard $(patsubst %,$(DEPDIR)/%.gch.d,$(basename $(headers))))

clean:
	$(v)rm -f $(LIBNAME).so*
	$(v)rm -f $(OBJ_DIR)/*
	$(v)rmdir $(OBJ_DIR)
	$(v)rm -f $(DEPDIR)/*
	$(v)rmdir $(DEPDIR)
