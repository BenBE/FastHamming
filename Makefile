LIBSRC = $(wildcard *.c)
LIBOBJ = $(patsubst %.c,%.o,$(LIBSRC))
LIBDEP = $(patsubst %.c,%.d,$(LIBSRC))
LIBDEST = libfasthamming.a

CC ?= gcc
LD ?= gcc
AR ?= ar
RANLIB ?= ranlib

.PHONY: all
all: lib

.PHONY: lib
lib: $(LIBDEST)

%.a: $(LIBOBJ)
	$(AR) rc $@ $^ && $(RANLIB) $@

%.o %.d: %.c
	$(CC) -MMD -o $*.o -c $<

-include $(LIBDEP)

clean:
	find . -name '*.d' -print -delete
	find . -name '*.o' -print -delete
	find . -name '*.a' -print -delete
