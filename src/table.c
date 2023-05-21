#include "table.h"

#include <stdlib.h>
#include <stdio.h>

/**
 * @brief Hash function
 *
 * @param some_value value to hash
 * @return int hash value
 */
int hash(char *some_value)
{
    int res = 0;
    int i = 0;

    for (i = 0; some_value[i] != '\0'; i++)
    {
        res = res + some_value[i];
    }
    return res;
}

/**
 * @brief Create a new hash table
 *
 * @param size of the hash table
 * @return Table_t* new hash table
 */
Table_t *table_create(int size)
{
    Table_t *table = malloc(sizeof(Table_t));
    table->slots = malloc(sizeof(List_t) * size);
    table->length = size;
    for (int i = 0; i < size; i++)
    {
        table->slots[i] = list_create();
    }
    return table;
}

/**
 * @brief Destroy a hash table
 *
 * @param one_table to destroy
 */
void table_destroy(Table_t *one_table)
{
    for (int i = 0; i < one_table->length; i++)
    {
        list_destroy(one_table->slots[i]);
    }
    free(one_table->slots);
    free(one_table);
}

/**
 * @brief Get the index of a key in the hash table
 *
 * @param one_table to search
 * @param one_key to search
 * @return int index of the key
 */
int table_indexof(Table_t *one_table, char *one_key)
{
    return (hash(one_key) % one_table->length + one_table->length) % one_table->length;
}

/**
 * @brief Add a key and a value to the hash table
 *
 * @param one_table to add to
 * @param one_key to add
 * @param one_station to add
 */
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
    one_table->nbStation ++;
    List_t *list = one_table->slots[index];
    list_append(list, one_key, one_station);
}

/**
 * @brief Check if a key is in the hash table
 *
 * @param one_table to search
 * @param one_key to search
 * @return bool true if the key is in the hash table, false otherwise
 */
bool table_contains(Table_t *one_table, char *one_key)
{
    int index = table_indexof(one_table, one_key);
    List_t *list = one_table->slots[index];
    return list_contains(list, one_key);
}

/**
 * @brief Get the value of a key in the hash table
 *
 * @param one_table to search
 * @param one_key to search
 * @return Station_t* value of the key
 */
Station_t *table_get(Table_t *one_table, char *one_key)
{
    int index = table_indexof(one_table, one_key);
    List_t *list = one_table->slots[index];
    return list_find(list, one_key);
}


void table_print(Table_t *one_table){
    printf("\n");
    for (int i = 0; i<one_table->length; i++){
        List_t *one_list = one_table->slots[i];
        if (!list_is_empty(one_list)){
            list_print(one_list);
        }
    }
}
