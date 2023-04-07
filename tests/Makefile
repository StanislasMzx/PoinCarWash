CC=clang
CFLAGS=-std=c99 -Wall -Wextra -pedantic -fdiagnostics-color=always -Wno-strict-prototypes
CFLAGS+=-O0 -g3 -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls
LDFLAGS+=-fsanitize=address

ALL_EXECUTABLES=coordinates_test

all: $(ALL_EXECUTABLES)

coordinates_test: coordinates_test.o ../src/coordinates.o
coordinates_test.o: coordinates_test.c ../src/coordinates.h snow.h

clean: 
	rm -f *.o ../src/*.o ${ALL_EXECUTABLES}