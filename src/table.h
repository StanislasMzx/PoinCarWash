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

int hash(char *some_value);

Table_t *table_create(int size);

void table_destroy(Table_t *one_table);

int table_indexof(Table_t *one_table, char *one_key);

void table_add(Table_t *one_table, char *one_key, Station_t *one_station);

bool table_contains(Table_t *one_table, char *one_key);

Station_t *table_get(Table_t *one_table, char *one_key);

void table_print(Table_t *one_table);

#endif /* __TABLE_H__ */
