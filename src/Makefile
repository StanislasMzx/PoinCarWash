CC=clang
CFLAGS=-std=c99 -Wall -Wextra -pedantic -fdiagnostics-color=always -Wno-strict-prototypes
CFLAGS+=-O0 -g3 -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls
LDFLAGS+=-fsanitize=address

ALL_EXECUTABLES=compute_journey

all: $(ALL_EXECUTABLES)

a_star.o: a_star.c heap.h vehicles.h stations.h table.h list.h coordinates.h a_star.h
heap.o: heap.c stations.h heap.h
vehicles.o: vehicles.c vehicles.h
stations.o: stations.c table.h stations.h
table.o: table.c list.h table.h
list.o: list.c coordinates.h list.h
coordinates.o: coordinates.c coordinates.h


compute_journey: compute_journey.o a_star.o heap.o vehicles.o stations.o table.o list.o coordinates.o
compute_journey.o: compute_journey.c a_star.h a_star.h


clean: 
	rm -f *.o ${ALL_EXECUTABLES} *test

