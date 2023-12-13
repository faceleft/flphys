PREFIX=/usr

CC=cc

CFLAGS= -Wall -Wextra -Ofast -march=native

HEADERS=flphys.h
SOURCES=flphys.c

OBJECTS=libflphys.o
SHARED=libflphys.so

TEST_SRC=flphys_test.c
BENCH_SRC=flphys_bench.c

all: $(SHARED)

$(SHARED): $(OBJECTS)
	$(CC) -shared $^ -o $@

$(OBJECTS): $(SOURCES) $(HEADERS)
	$(CC) -c -fPIC $(CFLAGS) -std=c99 $(SOURCES) -o $@

install: $(HEADERS) $(SHARED)
	mkdir -p $(PREFIX)/include
	mkdir -p $(PREFIX)/lib
	install $(HEADERS) $(PREFIX)/include/$(HEADERS)
	install $(SHARED) $(PREFIX)/lib/$(SHARED)

uninstall:
	rm $(PREFIX)/include/$(HEADERS)
	rm $(PREFIX)/lib/$(SHARED)

clean:
	rm -rf $(OBJECTS)
	rm -rf $(SHARED)

test: $(OBJECTS) $(TEST_SRC)
	$(CC) $(TEST_SRC) $(OBJECTS) -lm -o flphys_test.tmp && ./flphys_test.tmp
	rm -f flphys_test.tmp

bench: $(OBJECTS) $(BENCH_SRC)
	$(CC) $(BENCH_SRC) $(OBJECTS) -lm -O0 -o flphys_bench.tmp && ./flphys_bench.tmp
	rm -f flphys_bench.tmp
