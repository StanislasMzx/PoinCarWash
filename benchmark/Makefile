CC=clang
CFLAGS=-std=c99 -Wall -Wextra -pedantic -fdiagnostics-color=always -Wno-strict-prototypes
CFLAGS+=-O0 -g3 -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls
LDFLAGS+=-fsanitize=address -lcurl

ALL_EXECUTABLES=benchmark

all: $(ALL_EXECUTABLES)


coordinates.o: ../src/coordinates.c ../src/coordinates.h
nominatim.o: ../src/nominatim.c ../src/nominatim.h ../src/coordinates.o
list.o: ../src/list.c ../src/list.h ../src/coordinates.o
table.o: ../src/table.c ../src/table.h ../src/list.o
stations.o: ../src/stations.c ../src/stations.h ../src/table.o 
heap.o: ../src/heap.c ../src/heap.h ../src/stations.o
a_star.o: ../src/a_star.c ../src/a_star.h ../src/heap.o ../src/vehicles.o
compute_trip.o: ../src/compute_trip.c ../src/a_star.h ../src/nominatim.h ../src/coordinates.o ../src/nominatim.o ../src/vehicles.o ../src/stations.o ../src/table.o ../src/list.o ../src/heap.o

benchmark: benchmark.o ../src/compute_trip.o ../src/a_star.o ../src/nominatim.o ../src/coordinates.o ../src/nominatim.o ../src/vehicles.o ../src/stations.o ../src/table.o ../src/list.o ../src/heap.o
benchmark.o: benchmark.c ../src/compute_trip.h ../src/nominatim.h

clean: 
	rm -f *.o ../src/*.o ${ALL_EXECUTABLES}
