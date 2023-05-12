CC=clang
CFLAGS=-std=c99 -Wall -Wextra -pedantic -fdiagnostics-color=always -Wno-strict-prototypes
CFLAGS+=-O0 -g3 -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls
LDFLAGS+=-fsanitize=address

ALL_EXECUTABLES=benchmark

all: $(ALL_EXECUTABLES)

benchmark: benchmark.o #../src/main.o
benchmark.o: benchmark.c #../src/main.h

clean: 
	rm -f *.o ../src/*.o ${ALL_EXECUTABLES}