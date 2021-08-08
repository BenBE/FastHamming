LIBSRC = $(wildcard *.c)
LIBOBJ = $(patsubst %.c,%.o,$(LIBSRC))
LIBDEP = $(patsubst %.c,%.d,$(LIBSRC))
LIBDEST = libfasthamming.a

TESTSRC = $(wildcard test/*.c)
TESTDEST = $(patsubst test/%.c,test_%,$(TESTSRC))

CC ?= gcc
LD ?= gcc
AR ?= ar
RANLIB ?= ranlib

.PHONY: all
all: lib test

.PHONY: lib
lib: $(LIBDEST)

%.a: $(LIBOBJ)
	$(AR) rc $@ $^ && $(RANLIB) $@

%.o %.d: %.c
	$(CC) $(CFLAGS) -MMD -o $*.o -c $<

-include $(LIBDEP)

.PHONY: test
test: $(TESTDEST)
	for a in $^; do printf 'Running %s …\n' "$$a"; ./$$a; done

test_%: test/%.c $(LIBDEST)
	$(CC) -I. $(CFLAGS) -o $@ $^ $(LFLAGS)

clean:
	-find . -type f -name '*.d' -print -delete
	-find . -type f -name '*.o' -print -delete
	-find . -type f -name '*.a' -print -delete
	-find . -type f -name 'test_*' \( -perm -u=x -o -perm -g=x -o -perm -o=x \) -print -delete
	-rm -f $(LIBDEP)
	-rm -f $(LIBOBJ)
	-rm -f $(LIBDEST)
	-rm -f $(TESTDEST)
