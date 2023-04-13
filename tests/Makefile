CC=clang
CFLAGS=-std=c99 -Wall -Wextra -pedantic -fdiagnostics-color=always -Wno-strict-prototypes
CFLAGS+=-O0 -g3 -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls
LDFLAGS+=-fsanitize=address

ALL_EXECUTABLES=coordinates_test vehicles_test

all: $(ALL_EXECUTABLES)

coordinates_test: coordinates_test.o ../src/coordinates.o
coordinates_test.o: coordinates_test.c ../src/coordinates.h snow.h

vehicles_test: vehicles_test.o ../src/vehicles.o
vehicles_test.o: vehicles_test.c ../src/vehicles.h snow.h

clean: 
	rm -f *.o ../src/*.o ${ALL_EXECUTABLES}