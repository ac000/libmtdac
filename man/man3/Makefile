sources = $(wildcard *.3)
objects = $(sources:.3=.3.gz)

v = @
ifeq ($V,1)
        v =
endif

.PHONY: man
man: $(objects)

%.3.gz: %.3
	@echo "  GZ   $@"
	$(v)gzip -c $< > $@

.PHONY: clean
clean:
	$(v)rm -f *.gz
