default: fake_tgt
	make -f Makefile 2>&1 $(MAKE_FLAGS) | tee log/make.log

%: fake_tgt
	make  -f Makefile $@ 2>&1 $(MAKE_FLAGS) | tee log/make.log
	
CF_DEFS:=$(wildcard etc/*.def)
CF_FILE:=$(patsubst %.def,%,$(CF_DEFS))
CF_PRES:=$(wildcard $(CF_FILE))
CF_MISS:=$(filter-out $(CF_PRES), $(CF_FILE))


Makefile GNUmakefile:;

MAKE_FLAGS:= -Rr --warn-undefined-variable


fake_tgt: $(CF_MISS)
	@echo missing files added

$(CF_MISS): %: %.def
	cp $< $@

.PHONY: 
