#define _GNU_SOURCE
#define MONITORING_OUT_FILE "../data/monitoring/monitoring.csv"

#include "timeline_station.h"
#include <getopt.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    // Parse command line arguments
    int opt;
    char *output_file = MONITORING_OUT_FILE;
    while ((opt = getopt(argc, argv, ":hf:")) != -1)
    {
        switch (opt)
        {
        case 'h':
            printf("USAGE: %s [options] file\n", argv[0]);
            printf("\n");
            printf("OPTIONS:\n");
            printf("  -h         Show this help message\n");
            printf("  -f <file>  Set the output file name\n");
            return 0;
        case 'f':
            output_file = optarg;
            break;
        case '?':
            fprintf(stderr, "\u2514\u2500\u2500 \33[1;91mError\33[0m\n");
            fprintf(stderr, "    \u2514\u2500\u2500 Unknown option: %c\n", optopt);
            return 1;
            break;
        }
    }
    if (optind >= argc)
    {
        fprintf(stderr, "\u2514\u2500\u2500 \33[1;91mError\33[0m\n");
        fprintf(stderr, "    \u2514\u2500\u2500 No input file specified.\n");
        return (EXIT_FAILURE);
    }
    char *input_file = argv[optind];

    // Start monitoring process
    printf("\33[2m[~] Starting monitoring...\33[0m\n");
    // Load station table
    Table_t *table = load_stations(STATION_TABLE_PATH);
    printf("\33[2m[~] Table loaded (%d stations).\33[0m\n", table->nbStation);
    // Initialize user timeline
    Timeline_all_users_t *user_timeline = initializeTimelineUser(table, input_file);
    printf("\33[2m[~] User timeline initialized (%d users).\33[0m\n", user_timeline->userNumber);
    // Initialize station timeline
    Timeline_all_stations_t *station_timeline = initializeTimelineAllStation(user_timeline, table);
    printf("\33[2m[~] Station timeline initialized (%d stations).\33[0m\n", station_timeline->nbStations);

    return 0;
}
