#include "table.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

int hash(char *some_value)
{
    int res = 0;
    int i = 0;

    assert(some_value != NULL);

    for (i = 0; some_value[i] != '\0'; i++)
    {
        res = res + some_value[i];
    }
    return res+2; // 0 is the start and 1 is the end
}

Table_t *table_create(int size)
{
    Table_t *table = malloc(sizeof(Table_t));
    table->slots = malloc(sizeof(List_t) * (size+2));
    table->length = size+2;
    table->nbStation = 0;
    for (int i = 0; i < size+2; i++)
    {
        table->slots[i] = list_create();
    }
    return table;
}

void table_destroy(Table_t *one_table)
{
    for (int i = 0; i < one_table->length; i++)
    {
        list_destroy(one_table->slots[i]);
    }
    free(one_table->slots);
    free(one_table);
}

int table_indexof(Table_t *one_table, char *one_key)
{
    return (hash(one_key) % one_table->length + one_table->length) % one_table->length;
}

void table_add(Table_t *one_table, char *one_key, Station_t *one_station)
{
    int index = table_indexof(one_table, one_key);
    if (list_contains(one_table->slots[index], one_key))
    {
        free(one_key);
        free(one_station->name);
        free(one_station->coordinates);
        free(one_station->last_station);
        free(one_station);
        return;
    }
    one_table->nbStation++;
    List_t *list = one_table->slots[index];
    list_append(list, one_key, one_station);
}

bool table_contains(Table_t *one_table, char *one_key)
{
    int index = table_indexof(one_table, one_key);
    List_t *list = one_table->slots[index];
    return list_contains(list, one_key);
}

Station_t *table_get(Table_t *one_table, char *one_key)
{
    int index = table_indexof(one_table, one_key);
    List_t *list = one_table->slots[index];
    return list_find(list, one_key);
}

void table_print(Table_t *one_table)
{
    printf("\n");
    for (int i = 0; i < one_table->length; i++)
    {
        List_t *one_list = one_table->slots[i];
        if (!list_is_empty(one_list))
        {
            list_print(one_list);
        }
    }
}
