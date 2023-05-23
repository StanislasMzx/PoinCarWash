CC=clang
CFLAGS=-std=c99 -Wall -Wextra -pedantic -fdiagnostics-color=always -Wno-strict-prototypes
CFLAGS+=-O0 -g3 -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls
LDFLAGS+=-fsanitize=address -lcurl

ALL_EXECUTABLES=coordinates_test nominatim_test vehicles_test stations_test heap_test a_star_test

all: $(ALL_EXECUTABLES)


coordinates.o: ../src/coordinates.c ../src/coordinates.h
nominatim.o: ../src/nominatim.c ../src/nominatim.h ../src/coordinates.o
list.o: ../src/list.c ../src/list.h ../src/coordinates.o
table.o: ../src/table.c ../src/table.h ../src/list.o
stations.o: ../src/stations.c ../src/stations.h ../src/table.o 
heap.o: ../src/heap.c ../src/heap.h ../src/stations.o
a_star.o: ../src/a_star.c ../src/a_star.h ../src/heap.o ../src/vehicles.o


coordinates_test: coordinates_test.o ../src/coordinates.o
coordinates_test.o: coordinates_test.c ../src/coordinates.h snow.h

nominatim_test: nominatim_test.o ../src/nominatim.o
nominatim_test.o: nominatim_test.c ../src/nominatim.h snow.h

vehicles_test: vehicles_test.o ../src/vehicles.o
vehicles_test.o: vehicles_test.c ../src/vehicles.h snow.h

stations_test: stations_test.o ../src/stations.o ../src/table.o ../src/list.o ../src/coordinates.o
stations_test.o: stations_test.c ../src/stations.h snow.h

heap_test : heap_test.o ../src/heap.o ../src/stations.o ../src/table.o ../src/list.o ../src/coordinates.o
heap_test.o : heap_test.c ../src/heap.h snow.h

a_star_test: a_star_test.o ../src/a_star.o ../src/heap.o ../src/stations.o ../src/table.o ../src/list.o ../src/coordinates.o ../src/vehicles.o
a_star_test.o: a_star_test.c ../src/a_star.h snow.h

opti_test: opti_test.o ../src/stations.o ../src/table.o ../src/list.o ../src/coordinates.o
opti_test.o: opti_test.c ../src/stations.h


clean: 
	rm -f *.o ../src/*.o ${ALL_EXECUTABLES} *test


test:
	for test_file in ${ALL_EXECUTABLES}; do ./$$test_file; done
