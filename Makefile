PREFIX=/usr/local
DESTDIR=

CC=cc
AR=ar
LD=ld

#CFLAGS= -Wall -Wextra -Wpedantic -pedantic -Og -g
CFLAGS=-Ofast

HEADER=flphys.h
SOURCES=flphys.c

OBJECTS=$(SOURCES:.c=.o)
SHARED=libflphys.so
STATIC=libflphys.a

TEST_SRC=flphys_test.c
BENCH_SRC=flphys_bench.c

all: $(SHARED) $(STATIC)

$(SHARED): $(OBJECTS)
	$(LD) -shared $^ -o $@ -lm

$(STATIC): $(OBJECTS)
	$(AR) rcs $@ $^

.c.o:
	$(CC) -fPIC -std=c99 -c $(CFLAGS) $< -o $@

install: $(HEADER) $(SHARED) $(STATIC)
	mkdir -p $(DESTDIR)$(PREFIX)/include
	mkdir -p $(DESTDIR)$(PREFIX)/lib
	install $(HEADER) $(DESTDIR)$(PREFIX)/include/$(HEADER)
	install $(SHARED) $(DESTDIR)$(PREFIX)/lib/$(SHARED)
	install $(SHARED) $(DESTDIR)$(PREFIX)/lib/$(STATIC)

uninstall:
	rm $(DESTDIR)$(PREFIX)/include/$(HEADER)
	rm $(DESTDIR)$(PREFIX)/lib/$(SHARED)
	rm $(DESTDIR)$(PREFIX)/lib/$(STATIC)

clean:
	rm -rf $(OBJECTS)
	rm -rf $(SHARED)
	rm -rf $(STATIC)

test: $(STATIC) $(TEST_SRC)
	$(CC) $(TEST_SRC) $(STATIC) -lm -O0 -o flphys_test_tmp && ./flphys_test_tmp
	rm -f flphys_test_tmp

bench: $(STATIC) $(BENCH_SRC)
	$(CC) $(BENCH_SRC) $(STATIC) -lm -O0 -o flphys_bench_tmp && ./flphys_bench_tmp
	rm -f flphys_bench_tmp