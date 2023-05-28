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

/**
 * @brief Create a new State
 *
 * @param one_station id of the station of the state
 * @param one_weight weight to go to the State::station from the start
 * @return State_t* pointer to the new State
 */
State_t *state_create(char *one_station, double one_weight);

/**
 * @brief Returns a new empty Heap
 * 
 * @return Heap_t* pointer to the new Heap
 */
Heap_t *heap_empty();

/**
 * @brief Check if the Heap is empty
 * 
 * @param one_heap Heap to check
 * @return whether the Heap is empty or not
 */
bool heap_is_empty(Heap_t *one_heap);

/**
 * @brief Check if the Heap is a leaf
 * 
 * @param one_heap Heap to check
 * @return whether the Heap is a leaf or not
 */
bool heap_is_leaf(Heap_t *one_heap);

/**
 * @brief Returns the height of the Heap
 * 
 * @param one_heap Heap to check
 * @return height of the Heap
 */
unsigned int heap_height(Heap_t *one_heap);

/**
 * @brief Balance the Heap
 * 
 * @param one_heap Heap to balance
 */
void rebalance(Heap_t *one_heap);

/**
 * @brief Pop the root of the Heap
 * 
 * @param one_heap Heap to pop
 * @param height height of the Heap
 * @return State_t* pointer to the old root of the Heap, NULL if the Heap is empty
 */
State_t *heap_pop(Heap_t **one_Heap_t, unsigned int height);

/**
 * @brief Append a new State to the Heap
 * 
 * @param one_heap Heap to append to
 * @param one_value State to append
 * @param height height of the Heap
 */
void heap_append(Heap_t **one_heap, State_t *one_value, unsigned int height);

/**
 * @brief Print the Heap with station names
 * 
 * @param one_heap Heap to print
 * @param one_table Table of stations
 */
void heap_print_station(Heap_t *one_heap, Table_t *one_table);

/**
 * @brief Print the Heap
 * 
 * @param one_heap Heap to print
 */
void heap_print(Heap_t *one_heap);

/**
 * @brief Destroy the Heap
 * 
 * @param one_heap Heap to destroy
 */
void heap_destroy(Heap_t *one_heap);

/**
 * @brief Destroy the State
 * 
 * @param one_state State to destroy
 */
void state_destroy(State_t *one_state);


#endif /* HEAP_H */
