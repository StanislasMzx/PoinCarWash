#include <stdbool.h>
#include "list.h"

#ifndef __TABLE_H__
#define __TABLE_H__

struct _Table_t
{
    List_t **slots;
    int length;
    int nbStation;
};
/**
 * @typedef Table_t
 * @brief Hash table type
 *
 * @var Table_t::slots
 * Array of lists
 * @var Table_t::length
 * Length of the array
 */
typedef struct _Table_t Table_t;

/**
 * @brief Hash function
 *
 * @param some_value value to hash
 * @return int hash value
 */
int hash(char *some_value);

/**
 * @brief Create a new hash table
 *
 * @param size of the hash table
 * @return Table_t* new hash table
 */
Table_t *table_create(int size);

/**
 * @brief Destroy a hash table
 *
 * @param one_table to destroy
 */
void table_destroy(Table_t *one_table);

/**
 * @brief Get the index of a key in the hash table
 *
 * @param one_table to search
 * @param one_key to search
 * @return int index of the key
 */
int table_indexof(Table_t *one_table, char *one_key);

/**
 * @brief Add a key and a value to the hash table
 *
 * @param one_table to add to
 * @param one_key to add
 * @param one_station to add
 */
void table_add(Table_t *one_table, char *one_key, Station_t *one_station);

/**
 * @brief Check if a key is in the hash table
 *
 * @param one_table to search
 * @param one_key to search
 * @return bool true if the key is in the hash table, false otherwise
 */
bool table_contains(Table_t *one_table, char *one_key);

/**
 * @brief Get the value of a key in the hash table
 *
 * @param one_table to search
 * @param one_key to search
 * @return Station_t* value of the key
 */
Station_t *table_get(Table_t *one_table, char *one_key);

/**
 * @brief Print the hash table
 *
 * @param one_table to print
 */
void table_print(Table_t *one_table);

#endif /* __TABLE_H__ */
