CC=clang
CFLAGS=-std=c99 -Wall -Wextra -pedantic -fdiagnostics-color=always -Wno-strict-prototypes
CFLAGS+=-O0 -g3 -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls
LDFLAGS+=-fsanitize=address

ALL_EXECUTABLES=coordinates_test vehicles_test stations_test

all: $(ALL_EXECUTABLES)

coordinates_test: coordinates_test.o ../src/coordinates.o
coordinates_test.o: coordinates_test.c ../src/coordinates.h snow.h

vehicles_test: vehicles_test.o ../src/vehicles.o
vehicles_test.o: vehicles_test.c ../src/vehicles.h snow.h

stations_test: stations_test.o ../src/stations.o ../src/table.o ../src/list.o ../src/coordinates.o
stations_test.o: stations_test.c ../src/stations.h snow.h
stations.o: ../src/stations.c ../src/stations.h ../src/table.o 
table.o: ../src/table.c ../src/table.h ../src/list.o
list.o: ../src/list.c ../src/list.h ../src/coordinates.o
coordinates.o: ../src/coordinates.c ../src/coordinates.h

clean: 
	rm -f *.o ../src/*.o ${ALL_EXECUTABLES}