#define _GNU_SOURCE
#define MONITORING_OUT_FILE "../data/monitoring/monitoring.csv"

#include "timeline_station.h"
#include <unistd.h>
#include <stdio.h>

/**
 * @brief Check for monitoring output file
 *
 * @param fileName char* name of the output file
 */
void monitoring_file_init(char *fileName, int lastTick)
{
    // Check if the output file exists and is writable
    if (access(fileName, W_OK) == -1)
    {
        // Create the output file
        FILE *fp = fopen(fileName, "w");
        fprintf(fp, "tick,");
        for (int i = 0; i < lastTick; i++)
        {
            fprintf(fp, "%d,", i);
        }
        fprintf(fp, "%d\n", lastTick);
        fclose(fp);
    }
    else
    {
        printf("\33[2m\u2502\u00a0\u00a0 \u251c\u2500\u2500 Output file already exists, appending to it...\33[0m\n");
    }
}

/**
 * @brief Write the timeline to the output file
 *
 * @param fileName char* name of the output file
 * @param timeline Timeline_all_stations_t* timeline to write
 */
void monitoring_file_write(char *fileName, Timeline_all_stations_t *timeline)
{
    // Open the output file
    FILE *fp = fopen(fileName, "a");

    Timeline_station_t *current_station;
    // For each station
    for (int i = 0; i < timeline->nbStations; i++)
    {
        current_station = timeline->listTimeline[i];
        fprintf(fp, "\"%s\"", current_station->name);
        // For each tick
        while (current_station != NULL)
        {
            fprintf(fp, ",%f", (double)current_station->stateValue->numberVehicle / (double)current_station->stateValue->availablePlugs);
            current_station = current_station->next;
        }
        fprintf(fp, "\n");
    }

    // Close the output file
    fclose(fp);
}

/**
 * @brief Main function
 *
 * @param argc int number of arguments
 * @param argv char** arguments
 * @return int exit code
 */
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
    printf("\33[2m\u251c\u2500\u2500 Starting monitoring...\33[0m\n");

    printf("\33[2m\u251c\u2500\u2500 Initialization \33[0m\n");
    // Load station table
    Table_t *table = load_stations(STATION_TABLE_PATH);
    printf("\33[2m\u2502\u00a0\u00a0 \u251c\u2500\u2500 Table loaded (%d stations).\33[0m\n", table->nbStation);
    // Initialize user timeline
    Timeline_all_users_t *user_timeline = initializeTimelineUser(table, input_file);
    printf("\33[2m\u2502\u00a0\u00a0 \u251c\u2500\u2500 User timeline initialized (%d users).\33[0m\n", user_timeline->userNumber);
    // Initialize station timeline
    Timeline_all_stations_t *station_timeline = initializeTimelineAllStation(user_timeline, table);
    printf("\33[2m\u2502\u00a0\u00a0 \u2514\u2500\u2500 Station timeline initialized (%d stations).\33[0m\n", station_timeline->nbStations);

    // Build timeline
    printf("\33[2m\u251c\u2500\u2500 Build timeline \33[0m\n");
    makeTimelineStation(station_timeline, user_timeline, table);
    printf("\33[2m\u2502\u00a0\u00a0 \u2514\u2500\u2500  Station timeline made (last tick: %d).\33[0m\n", station_timeline->lastTick);

    // Edit output file
    printf("\33[2m\u251c\u2500\u2500 Edit output file \33[0m\n");
    // Check if the output file exists and is writable
    monitoring_file_init(output_file, station_timeline->lastTick);
    printf("\33[2m\u2502\u00a0\u00a0 \u251c\u2500\u2500 Output file opened (%s).\33[0m\n", output_file);
    // Write the timeline to the output file
    monitoring_file_write(output_file, station_timeline);
    printf("\33[2m\u2502\u00a0\u00a0 \u2514\u2500\u2500 Output file written.\33[0m\n");

    // Free memory
    printf("\33[2m\u251c\u2500\u2500 Memory free \33[0m\n");
    timelineUserDestroyAll(&user_timeline);
    printf("\33[2m\u2502\u00a0\u00a0 \u251c\u2500\u2500 User timeline destroyed.\33[0m\n");
    destroyTimelineAllStations(station_timeline);
    printf("\33[2m\u2502\u00a0\u00a0 \u251c\u2500\u2500 Station timeline destroyed.\33[0m\n");
    table_destroy(table);
    printf("\33[2m\u2502\u00a0\u00a0 \u2514\u2500\u2500 Table destroyed.\33[0m\n");

    // End monitoring process
    printf("\33[2m\u2514\u2500\u2500 \33[32mMonitoring successfully ended.\33[0m\n");

    return 0;
}
