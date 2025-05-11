VERSION	    = 0.90.0
DATE	    = $(shell date +"%b %e, %Y")

MANS3	    = man/man3
MANS3type   = man/man3type

PANDOC3     = pandoc --standalone --to man $< --metadata title="$$(basename $@ .3)(3) Version $(VERSION) | libmtdac"
PANDOC3type = pandoc --standalone --to man $< --metadata title="$$(basename $@ .3type)(3type) Version $(VERSION) | libmtdac"

sources  = $(wildcard *.3*.rst)
objects  = $(patsubst %.3.rst,$(MANS3)/%.3,$(sources))
objects += $(patsubst %.3type.rst,$(MANS3type)/%.3type,$(sources))

v = @
ifeq ($V,1)
        v =
endif

.PHONY: man
man: $(objects)

%.3: %.3.rst
$(MANS3)/%.3: %.3.rst
	@echo "  MAN   $@"
	$(v)$(PANDOC3) --metadata date="$(DATE)" -o $@

%.3type: %.3type.rst
$(MANS3type)/%.3type: %.3type.rst
	@echo "  MAN   $@"
	$(v)$(PANDOC3type) --metadata date="$(DATE)" -o $@
