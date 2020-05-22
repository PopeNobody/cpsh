MAKEFLAGS:=-R -r 
all:
include Makefile.avoid
include etc/make_default_target.mk
include etc/make_jobs.mk

all:

default: $(MAKECMDGOALS)


ifneq (,$(MAKECMDGOALS))
$(MAKECMDGOALS):
	${MAKE} -f Makefile $(MAKECMDGOALS)
endif


all:
	${MAKE} -f Makefile all


