#define EARTH_RADIUS 6371
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#define BENCHMARK_OUT_DIR "out"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include "../src/nominatim.h"
#include "../src/compute_journey.h"

/**
 * @brief Check for output directory
 */
void benchmark_dir_init(void)
{
    // Check if the output directory exists and is writable
    if (access(BENCHMARK_OUT_DIR, W_OK) == -1)
    {
        // Create the output directory
        mkdir(BENCHMARK_OUT_DIR, 0755);
    }
}

/**
 * @brief Get a timestamped benchmark file name
 *
 * @return char* timestamped file name
 */
char *benchmark_fileName(void)
{
    // Create a timestamped file name
    char *fileName = malloc(sizeof(char) * 25);
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(fileName, "benchmark_%02d-%02d_%02d-%02d-%02d", tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

    return fileName;
}

/**
 * @brief Check for benchmark output file
 *
 * @param fileName char* name of the output file
 * @return char* path to the output file
 */
char *benchmark_file_init(char *fileName)
{
    char *outFile = malloc(sizeof(char) * (strlen(BENCHMARK_OUT_DIR) + strlen(fileName) + 6));
    sprintf(outFile, "%s/%s.csv", BENCHMARK_OUT_DIR, fileName);

    // Check if the output file exists and is writable
    if (access(outFile, W_OK) == -1)
    {
        // Create the output file
        FILE *fp = fopen(outFile, "w");
        fprintf(fp, "id,maxDist,randomDist,startLat,startLon,time,nbRuns,timePerRun\n");
        fclose(fp);
    }
    else
    {
        printf("\x1b[33m[!] Output file already exists, appending to it...\x1b[0m\n");
    }

    return outFile;
}

/**
 * @brief Append a line to a benchmark file
 *
 * @param file char* path to the file
 * @param id int id of the benchmark
 * @param maxDist int maximum distance to search for a station
 * @param randomDist bool if the distance should be random or always use maxDist
 * @param startLat double latitude of the starting point
 * @param startLon double longitude of the starting point
 * @param time double time taken to run the benchmark
 * @param nbRuns int number of runs
 * @param timePerRun double time per run
 */
void benchmark_file_append(char *file, int id, int maxDist, bool randomDist, double startLat, double startLon, double time, int nbRuns, double timePerRun)
{
    FILE *fp = fopen(file, "a");
    fprintf(fp, "%d,%d,%d,%f,%f,%f,%d,%f\n", id, maxDist, randomDist, startLat, startLon, time, nbRuns, timePerRun);
    fclose(fp);
}

/**
 * @brief Call the function to benchmark
 *
 * @param startLat double latitude of the starting point
 * @param startLon double longitude of the starting point
 * @param endLat double latitude of the ending point
 * @param endLon double longitude of the ending point
 * @param vehicleName char* name of the vehicle
 */
void benchmark_call(double startLat, double startLon, double endLat, double endLon, char *vehicleName)
{
    Table_t *table = load_stations(STATION_TABLE_PATH);
    Nominatim_t *startNomin = nominatim_create("start", startLat, startLon);
    Nominatim_t *endNomin = nominatim_create("end", endLat, endLon);

    // Main call
    Journey_output_t journeyOutput = compute_journey(startNomin, endNomin, vehicleName);

    // Free memory
    nominatim_destroy(startNomin);
    nominatim_destroy(endNomin);
    table_destroy(table);
    table_destroy(journeyOutput.table);
    list_destroy(journeyOutput.journey);
}

/**
 * @brief Run the benchmark
 *
 * @param perRun bool if the benchmark should be run per run
 * @param outFile char* path to the output benchmark file
 * @param maxDist int maximum distance to search for a station
 * @param randomDist bool if the distance should be random or always use maxDist
 * @param startLat double latitude of the starting point
 * @param startLon double longitude of the starting point
 * @param nbRuns int number of runs
 */
void benchmark_run(char *outFile, bool perRun, int maxDist, bool randomDist, double startLat, double startLon, int nbRuns)
{
    clock_t mainStart = clock();
    for (int i = 0; i < nbRuns; i++)
    {
        // Start the run
        clock_t runStart = clock();

        // Get the distance
        int dist = randomDist ? rand() % maxDist + 1 : maxDist;

        // Random end point
        double distRad = (double)dist / EARTH_RADIUS;
        double startLatRad = startLat * (M_PI / 180.0);
        double startLonRad = startLon * (M_PI / 180.0);
        double angle = 2 * M_PI * ((double)rand() / RAND_MAX);

        double endLatRad = asin(sin(startLatRad) * cos(distRad) + cos(startLatRad) * sin(distRad) * cos(angle));
        double endLonRad = startLonRad + atan2(sin(angle) * sin(distRad) * cos(startLatRad), cos(distRad) - sin(startLatRad) * sin(endLatRad));
        double endLat = endLatRad * (180.0 / M_PI);
        double endLon = endLonRad * (180.0 / M_PI);

        // Run main function
        benchmark_call(startLat, startLon, endLat, endLon, "Tesla Model 3");

        // Summarize the run
        if (perRun)
        {
            clock_t runEnd = clock();
            double time = (double)(runEnd - runStart) / CLOCKS_PER_SEC;
            benchmark_file_append(outFile, i + 1, dist, randomDist, startLat, startLon, time, 1, time);
        }
    }
    clock_t mainEnd = clock();

    // Summarize the benchmark
    if (!perRun)
    {
        double time = (double)(mainEnd - mainStart) / CLOCKS_PER_SEC;
        double timePerRun = time / nbRuns;
        benchmark_file_append(outFile, 0, maxDist, randomDist, startLat, startLon, time, nbRuns, timePerRun);
    }
}

/**
 * @brief Main function
 *
 * @param argc int number of arguments
 * @param argv char** arguments
 * @return int exit code
 */
int main(int argc, char **argv)
{
    printf("\x1b[2m[~] Starting benchmark...\x1b[0m\n");
    char *cwd = getcwd(NULL, 0);

    // Check if the output directory exists and is writable
    benchmark_dir_init();
    printf("\x1b[32m[+] Output directory: \x1b[1m\x1b]8;;file://%s/%s\x07%s\x1b]8;;\x07\x1b[0m\n", cwd, BENCHMARK_OUT_DIR, BENCHMARK_OUT_DIR);

    // Get the output file name
    char *fileName = argc > 1 ? argv[1] : benchmark_fileName();
    char *outFile = benchmark_file_init(fileName);
    printf("\x1b[32m[+] Output file: \x1b[1m\x1b]8;;file://%s/%s\x07%s\x1b]8;;\x07\x1b[0m\n", cwd, outFile, outFile);

    // Prompt benchmark parameters
    int perRun = false;
    int maxDist = 10;
    int randomDist = false;
    double startLat = 45;
    double startLon = 0;
    int nbRuns = 0;
    printf("[?] Enter if the benchmark should be run per run (0 = false, 1 = true): ");
    scanf("%d", &perRun);
    printf("[?] Enter the maximum distance to search for a station (in km): ");
    scanf("%d", &maxDist);
    printf("[?] Enter if the distance should be random (0 = false, 1 = true): ");
    scanf("%d", &randomDist);
    printf("[?] Enter the latitude of the starting point: ");
    scanf("%lf", &startLat);
    printf("[?] Enter the longitude of the starting point: ");
    scanf("%lf", &startLon);
    printf("[?] Enter the number of runs: ");
    scanf("%d", &nbRuns);

    // Run the benchmark
    printf("\x1b[2m[~] Running benchmark...\x1b[0m\n");
    benchmark_run(outFile, perRun, maxDist, randomDist, startLat, startLon, nbRuns);
    printf("\x1b[32m[+] Benchmark finished!\x1b[0m\n");
    printf("\x1b[34m[*] You can find the results there: \x1b[1m\x1b]8;;file://%s/%s\x07%s\x1b]8;;\x07\x1b[0m\x1b[0m\n", cwd, outFile, outFile);

    // Free memory
    free(cwd);
    free(outFile);
    if (argc <= 1)
        free(fileName);

    return 0;
}
