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
            printf("  -h\t\tShow this help message\n");
            printf("  -f <file>\tSet the output file name\n");
            return 0;
        case 'f':
            output_file = optarg;
            break;
        case '?':
            fprintf(stderr, "\33[31m>> Error:\33[0m Unknown option: %c\n", optopt);
            return 1;
            break;
        }
    }
    if (optind >= argc)
    {
        fprintf(stderr, "\33[31m>> Error:\33[0m No input file specified\n");
        return (EXIT_FAILURE);
    }
    char *input_file = argv[optind];

    // Start monitoring process
    printf("\33[2m[~] Starting monitoring...\33[0m\n");

    return 0;
}
