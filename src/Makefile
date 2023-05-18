CC=clang
CFLAGS=-std=c99 -Wall -Wextra -pedantic -fdiagnostics-color=always -Wno-strict-prototypes
CFLAGS+=-O0 -g3 -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls
LDFLAGS+=-fsanitize=address -lcurl

ALL_EXECUTABLES=trip

all: $(ALL_EXECUTABLES)

a_star.o: a_star.c heap.h vehicles.h stations.h table.h list.h coordinates.h a_star.h
heap.o: heap.c stations.h heap.h
vehicles.o: vehicles.c vehicles.h
stations.o: stations.c table.h stations.h
table.o: table.c list.h table.h
list.o: list.c coordinates.h list.h
nominatim.o: nominatim.c nominatim.h
coordinates.o: coordinates.c coordinates.h
compute_journey.o: compute_journey.c a_star.h nominatim.h compute_journey.h

trip.o: trip.c nominatim.h compute_journey.h
trip: trip.o compute_journey.o a_star.o heap.o vehicles.o stations.o table.o list.o nominatim.o coordinates.o


# Varies depending on the OS
download_dependencies:
	apt-get install libcurl4-openssl-dev
link_dependencies:
	ln -s /usr/include/x86_64-linux-gnu/curl /usr/include/curl
fix_dependencies: download_dependencies link_dependencies


clean: 
	rm -f *.o ${ALL_EXECUTABLES} *test

