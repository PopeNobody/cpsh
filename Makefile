include etc/make_default_target.mk
MAKEFLAGS:= -Rr --warn-undefined-variable
include etc/make_jobs.mk

all: 

CXX:=/usr/stow/llvm-10a/bin/clang++ 
AR:= ar

CXXFLAGS = @etc/cxxflags
CPPFLAGS = @etc/cppflags
LDFLAGS = @etc/ld_flags
LDLIBS = @etc/ld_libs
ARFLAGS = $(shell cat etc/ar_flags)

ALL_DEPS = $(patsubst %,%.dd,$(ALL_SRC))
ALL_SRC:=
LCOIN_SRC:=$(wildcard lib/*.cc)
ALL_SRC+= $(LCOIN_SRC)
LCOIN_OBJ:=$(patsubst %.cc,%.cc.oo,$(LCOIN_SRC))

TESTS_SRC:=$(wildcard tst/*.cc)
ALL_SRC+= $(TESTS_SRC)
TESTS_OBJ:=$(patsubst %.cc,%.oo,$(TESTS_SRC))
TESTS:=$(patsubst tst/%.cc,bin/test-%,$(TESTS_SRC))

PROGS_SRC:=$(wildcard src/*.cc)
ALL_SRC+= $(PROGS_SRC)
PROGS_OBJ:=$(patsubst %.cc,%.oo,$(PROGS_SRC))
PROGS:=$(patsubst src/%.cc,bin/%,$(PROGS_SRC))
SH_SCRS:=$(patsubst scr/%.sh,bin/%,$(wildcard src/*.sh))
PL_SCRS:=$(patsubst scr/%.pl,bin/%,$(wildcard scr/*.pl))
ALL_SCRS:=$(SH_SCRS) $(PL_SCRS)
ALL_SCRS+=

ALL_CPP:=$(patsubst %.cc,%.cc.ii,$(ALL_SRC))
all_cpp: $(ALL_CPP)

test_%: bin/%
	report ./$<

test_%: tst/%
	report ./$<



LCOIN_OBJ: $(LCOIN_OBJ)

lib/libcoin.a: $(LCOIN_OBJ)
	flock $@.lock $(AR) $(ARFLAGS) $@ $^

%.cc.ii: %.cc    etc/cppflags etc/cxxflags
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -E $< -o $<.ii -MD -MT $@ -MF $<.dd  

%.cc.oo: %.cc.ii etc/cppflags etc/cxxflags
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(PL_SCRS): bin/%: scr/%.pl
	ln -f $< $@
	chmod +x $@

$(SH_SCRS): bin/%: scr/%.sh
	ln -f $< $@
	chmod +x $@

scripts: $(ALL_SCRS)

$(TESTS): bin/test-%: tst/%.cc.oo lib/libcoin.a etc/ld_flags etc/ld_libs
	${MAKE} -C readline all
	@echo making $< into $@
	$(CXX) $(LDFLAGS) $< -o $@ $(LDLIBS)

$(PROGS): bin/%: src/%.cc.oo lib/libcoin.a etc/ld_flags etc/ld_libs
	${MAKE} -C readline all
	@echo making $< into $@
	mkdir -p bin
	$(CXX) $(LDFLAGS) $< -o $@ $(LDLIBS)

check: tests $(patsubst bin/%, test_%, $(TESTS))

tests: $(TESTS)

progs: $(PROGS)

all: progs scripts $(TESTS)

tests:	$(TESTS)

.PHONY: all clean tags lists
	
db: bittrex.sql
	pg_dump --section pre-data | tee sql/schema.sql >/dev/null
	pg_dump --section data | tee sql/data.sql >/dev/null
	pg_dump --section post-data | tee sql/post.sql >/dev/null
	git add sql/{schema,data,post}.sql

#echo git add sql/$@
#echo git commit sql/$@ -m "dump"

CTAGS_FLAGS:= --extra=fq --fields=afikKlmnsSzt --language-force=c++

clean:
	rm -f lib/libcoin.a{,.lock} $(TESTS) $(ALL_SCRS) $(PROGS) tags
	find -name '*.oo' -o -name '*.ii' -o -name '*.dd' | xargs rm -f

tags:	deps.all
	ctags $(CTAGS_FLAGS) -L $^

#    

PRECIOUS: $(PROGS_SRC:.cc=.cc.ii)
PRECIOUS: $(PROGS_SRC:.cc=.cc.ii)
PRECIOUS: $(LCOIN_SRC:.cc=.cc.ii)
PRECIOUS: $(TESTS_SRC:.cc=.cc.ii)
PRECIOUS: $(LCOIN_SRC:.cc=.cc.dd)
PRECIOUS: $(TESTS_SRC:.cc=.cc.dd)


deps.all: bin/all_deps $(ALL_DEPS)
	vi_perl bin/all_deps  $@ $(wildcard */*.dd)

include $(wildcard /dev/null */*.dd)

lists: info/files.all info/files.git

info/files.git: scripts
	./bin/files.all

info/files.all: scripts
	./bin/files.all

all_cpp: $(ALL_CPP)
	echo done with $(ALL_CPP)

.PHONY: info/files.git info/files.all
