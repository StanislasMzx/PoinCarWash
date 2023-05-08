#define _GNU_SOURCE
#define TABLE_SIZE 10000

#include "stations.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>


/**
 * @brief Create a hash table of stations
 *
 * @param filename
 * @return Table_t* hash table of stations
 */
Table_t *load_stations(char *filename)
{
    FILE *fp = fopen(filename, "r");
    assert(fp != NULL);

    const unsigned max_line = 1024;
    char line[max_line];
    char *token;
    char *name;
    char *global_id;
    char *local_id;
    int plugs_number;
    int power;
    bool is_free;

    Table_t *table = table_create(TABLE_SIZE);

    // Skip the first line
    fgets(line, sizeof(line), fp);

    while (fgets(line, sizeof(line), fp))
    {
        char *line_str = line;
        assert(line_str != NULL);

        token = strsep(&line_str, ";"); // developer's name
        token = strsep(&line_str, ";"); // siren
        token = strsep(&line_str, ";"); // developer's contact
        token = strsep(&line_str, ";"); // operator's name
        token = strsep(&line_str, ";"); // operator's contact
        token = strsep(&line_str, ";"); // operator's phone
        token = strsep(&line_str, ";"); // brand name
        token = strsep(&line_str, ";"); // global station id
        global_id = token;
        token = strsep(&line_str, ";"); // local station id
        local_id = token;
        token = strsep(&line_str, ";"); // station name
        name = token;
        token = strsep(&line_str, ";"); // station implantation
        token = strsep(&line_str, ";"); // station address
        token = strsep(&line_str, ";"); // station postal code
        token = strsep(&line_str, ";"); // station coordinates (wrong)
        token = strsep(&line_str, ";"); // plugs number
        plugs_number = atoi(token);
        token = strsep(&line_str, ";"); // global station id (wrong)
        token = strsep(&line_str, ";"); // local station id (wrong)
        token = strsep(&line_str, ";"); // power
        power = atoi(token);
        token = strsep(&line_str, ";"); // ef type plug
        token = strsep(&line_str, ";"); // type 2 plug
        token = strsep(&line_str, ";"); // ccs combo plug
        token = strsep(&line_str, ";"); // chademo type plug
        token = strsep(&line_str, ";"); // other plug
        token = strsep(&line_str, ";"); // free
        is_free = (strcasecmp(token, "true") == 0) ? true : false;
        token = strsep(&line_str, ";"); // payment act
        token = strsep(&line_str, ";"); // payment cb
        token = strsep(&line_str, ";"); // other payment
        token = strsep(&line_str, ";"); // rate
        token = strsep(&line_str, ";"); // access conditions
        token = strsep(&line_str, ";"); // reservation
        token = strsep(&line_str, ";"); // opening hours
        token = strsep(&line_str, ";"); // accessibility
        token = strsep(&line_str, ";"); // vehicle shape restriction
        token = strsep(&line_str, ";"); // two wheelers
        token = strsep(&line_str, ";"); // connection type
        token = strsep(&line_str, ";"); // pdl number
        token = strsep(&line_str, ";"); // opening date
        token = strsep(&line_str, ";"); // observations
        token = strsep(&line_str, ";"); // update date
        token = strsep(&line_str, ";"); // t2 cable attachment
        token = strsep(&line_str, ";"); // last modification date
        token = strsep(&line_str, ";"); // datagouv dataset id
        token = strsep(&line_str, ";"); // datagouv resource id
        token = strsep(&line_str, ";"); // datagouv organization
        Coordinates_t *coordinates = malloc(sizeof(Coordinates_t));
        token = strsep(&line_str, ";"); // longitude
        coordinates->longitude = atof(token);
        token = strsep(&line_str, ";"); // latitude
        coordinates->latitude = atof(token);
        Station_t *station = station_create(name, coordinates, plugs_number, power, is_free);
        char *station_id = malloc(strlen(global_id) + strlen(local_id) + 1);
        assert(station_id != NULL);
        strcpy(station_id, global_id);
        strcat(station_id, local_id);

        table_add(table, station_id, station);
    }

    fclose(fp);
    return table;
}
/*
**
 * @brief Get the reachable station neighbors
 *
 * @param one_table hash table of stations
 * @param one_station station to get the neighbors from
 * @param range range in km
 * @return List_t* list of reachable station neighbors
 */
List_t *reachable_station_neighbors(Table_t *one_table, char *one_station_key, unsigned int range)
{
    Station_t *one_station = table_get(one_table, one_station_key);
    assert(one_station != NULL);
    List_t *neighbors = list_create();
    for (int i = 0; i < one_table->length; i++)
    {
        List_t *list = one_table->slots[i];
        for (int j = 0; j < list->length; j++)
        {
            Element_t *element = &list->list[j];
            if (strcmp(element->key, one_station_key) != 0 && distance(element->value->coordinates, one_station->coordinates) <= range)
            {
                list_append(neighbors, element->key, element->value);
            }
        }
    }
    return neighbors;
}

