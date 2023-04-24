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
// #include "../src/main.h"


/**
 * @brief Check for output directory
 */
void benchmark_dir_init(void) {
    // Check if the output directory exists and is writable
    if (access(BENCHMARK_OUT_DIR, W_OK) == -1) {
        // Create the output directory
        mkdir(BENCHMARK_OUT_DIR, 0755);
    }
}

/**
 * @brief Get a timestamped benchmark file name
 * 
 * @return char* timestamped file name
 */
char *benchmark_file_name(void) {
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
char *benchmark_file_init(char *fileName) {
    char *out_file = malloc(sizeof(char) * (strlen(BENCHMARK_OUT_DIR) + strlen(fileName) + 6));
    sprintf(out_file, "%s/%s.csv", BENCHMARK_OUT_DIR, fileName);

    // Check if the output file exists and is writable
    if (access(out_file, W_OK) == -1) {
        // Create the output file
        FILE *fp = fopen(out_file, "w");
        fprintf(fp, "id,maxDist,randomDist,startLat,startLon,time,nbRuns,timePerRun\n");
        fclose(fp);
    } else {
        printf("\x1b[33m[!] Output file already exists, appending to it...\x1b[0m\n");
    }

    return out_file;
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
void benchmark_file_append(char *file, int id, int maxDist, bool randomDist, double startLat, double startLon, double time, int nbRuns, double timePerRun) {
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
 */
void benchmark_call(double startLat, double startLon, double endLat, double endLon) {
    // TODO
    int ceci = rand() % 900000000 + 100000000;
    int est = rand() % 900000000 + 100000000;
    int du = rand() % 900000000 + 100000000;
    int code = rand() % 900000000 + 100000000;
    int pour = rand() % 900000000 + 100000000;
    int remplacer = rand() % 900000000 + 100000000;
    int l_appel = rand() % 900000000 + 100000000;
    int a_la = ceci * est * du * code;
    int fonction = pour * remplacer * l_appel;
    int finale = a_la + fonction;
    ceci += est + du + code + pour + remplacer + l_appel + a_la + fonction + finale + startLat + startLon + endLat + endLon;
}

/**
 * @brief Run the benchmark
 * 
 * @param perRun bool if the benchmark should be run per run
 * @param out_file char* path to the output benchmark file
 * @param maxDist int maximum distance to search for a station
 * @param randomDist bool if the distance should be random or always use maxDist
 * @param startLat double latitude of the starting point
 * @param startLon double longitude of the starting point
 * @param nbRuns int number of runs
 */
void benchmark_run(char *out_file, bool perRun, int maxDist, bool randomDist, double startLat, double startLon, int nbRuns) {
    clock_t main_start = clock();
    for (int i = 0; i < nbRuns; i++) {
        // Start the run
        clock_t run_start = clock();

        // Get the end point
        int dist = randomDist ? rand() % maxDist + 1 : maxDist;
        double endLat = startLat + dist * cos(2 * M_PI * rand() / RAND_MAX);
        double endLon = startLon + dist * sin(2 * M_PI * rand() / RAND_MAX);

        // Run main function
        benchmark_call(startLat, startLon, endLat, endLon);

        // Summarize the run
        if (perRun) {
            clock_t run_end = clock();
            double time = (double)(run_end - run_start) / CLOCKS_PER_SEC;
            benchmark_file_append(out_file, i+1, dist, randomDist, startLat, startLon, time, 1, time);
        }
    }
    clock_t main_end = clock();

    // Summarize the benchmark
    if (!perRun) {
        double time = (double)(main_end - main_start) / CLOCKS_PER_SEC;
        double timePerRun = time / nbRuns;
        benchmark_file_append(out_file, 0, maxDist, randomDist, startLat, startLon, time, nbRuns, timePerRun);
    }
}

/**
 * @brief Main function
 * 
 * @param argc int number of arguments
 * @param argv char** arguments
 * @return int exit code
 */
int main(int argc, char **argv) {
    printf("\x1b[2m[~] Starting benchmark...\x1b[0m\n");
    char *cwd = getcwd(NULL, 0);

    // Check if the output directory exists and is writable
    benchmark_dir_init();
    printf("\x1b[32m[+] Output directory: \x1b[1m\x1b]8;;file://%s/%s\x07%s\x1b]8;;\x07\x1b[0m\n", cwd, BENCHMARK_OUT_DIR, BENCHMARK_OUT_DIR);

    // Get the output file name
    char *file_name = argc > 1 ? argv[1] : benchmark_file_name();
    char *out_file = benchmark_file_init(file_name);
    printf("\x1b[32m[+] Output file: \x1b[1m\x1b]8;;file://%s/%s\x07%s\x1b]8;;\x07\x1b[0m\n", cwd, out_file, out_file);

    // Prompt benchmark parameters
    int perRun = false;
    int maxDist = 10;
    int randomDist = false;
    double startLat = 45;
    double startLon = 0;
    int nbRuns = 0;
    printf("[?] Enter if the benchmark should be run per run (0 = false, 1 = true): ");
    scanf("%d", &perRun);
    printf("[?] Enter the maximum distance to search for a station (in meters): ");
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
    benchmark_run(out_file, perRun, maxDist, randomDist, startLat, startLon, nbRuns);
    printf("\x1b[32m[+] Benchmark finished!\x1b[0m\n");
    printf("\x1b[34m[*] You can find the results there: \x1b[1m\x1b]8;;file://%s/%s\x07%s\x1b]8;;\x07\x1b[0m\x1b[0m\n", cwd, out_file, out_file);

    // Free memory
    free(cwd);
    free(out_file);
    if (argc <= 1) free(file_name);

    return 0;
}
