#ifndef HEAP_H
#define HEAP_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "stations.h"

struct _State_t
{
    char *id_station;
    double weight;
};
/**
 * @typedef State_t
 * @brief State type
 *
 * @var State_t::id_station
 * id of the station of the state
 * @var State_t::weight
 * weight to go to the State::station from the start
 */
typedef struct _State_t State_t;


struct _Heap_t
{
    struct _Heap_t *left;
    struct _Heap_t *right;
    State_t *value;
    unsigned int nbNode;
};
/**
 * @typedef Heap_t
 * @brief Heap type
 *
 * @var Heap_t::left
 * left son of the root
 * @var Heap_t::right
 * right son of the root
 * @var Heap_t::value
 * value of the root
 * @var Heap_t::nbNode
 * number of node in the tree
 * @var Heap_t::empty
 * if Heap is empty or not
 */
typedef struct _Heap_t Heap_t;


State_t *state_create(char *one_station, double one_weight);

Heap_t *heap_empty();

bool heap_is_empty(Heap_t *one_heap);

bool heap_is_leaf(Heap_t *one_heap);

unsigned int heap_height(Heap_t *one_heap);

void rebalance(Heap_t *one_heap);

State_t *heap_pop(Heap_t **one_Heap_t, unsigned int height);

void heap_append(Heap_t **one_heap, State_t *one_value, unsigned int height);

void heap_print_station(Heap_t *one_heap, Table_t *one_table);

void heap_print(Heap_t *one_heap);

void heap_destroy(Heap_t *one_heap);

void state_destroy(State_t *one_state);


#endif /* HEAP_H */
