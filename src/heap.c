#include "heap.h"



State_t *state_create(char *one_station, double one_weight){
    /* return an state with the caracteristic in parameter */
    State_t *one_state = malloc(sizeof(State_t));
    one_state->id_station = one_station;
    one_state->weight = one_weight;
    return one_state;
}


Heap_t *heap_empty(){
    /* return an empty heap */
    return NULL;
}


bool heap_is_empty(Heap_t *one_heap){
    /* Return if one_heap is empty */
    return one_heap == NULL;
}


bool heap_is_leaf(Heap_t *one_heap){
    /* Return if one_heap is a leaf */
    return (!heap_is_empty(one_heap) && one_heap->left == NULL); /* no need to look at right, a heap is perfect*/
}


unsigned int heap_height(Heap_t *one_heap){
    /* Return the number of node on the last level */
    unsigned int tmp = 1;
    unsigned int level = 0;
    if (!heap_is_empty(one_heap)){
        level += 1;
        while (one_heap->nbNode >= (tmp<<1)){
            tmp = tmp << 1;
            level += 1;
        }
    }
    return level;
}


void rebalance(Heap_t *one_heap){
    /* équilibre le tas */
    Heap_t *heap = one_heap;
    while (!heap_is_empty(heap) && !heap_is_leaf(heap)){
        Heap_t *left = heap->left;
        Heap_t *right = heap->right;
        if (right != NULL && heap->value->weight > right->value->weight && left->value->weight >= right->value->weight){
            State_t *tmp = heap->value;
            heap->value = right->value;
            right->value = tmp;
            heap = right;
        }
        else if (heap->value->weight > left->value->weight && left->value->weight < right->value->weight){
            State_t *tmp = heap->value;
            heap->value = left->value;
            left->value = tmp;
            heap = left;
        }else{
            heap = NULL; //on sait qu'on a juste a verifier si le nouvel etat pose probleme, en dessous c'est bon
        }
    }
}


State_t *heap_pop(Heap_t **one_heap_ptr, unsigned int height){
    /* Remove the root of one_heap and return his value, return NULL if one_heap is empty */
    Heap_t *one_heap = *one_heap_ptr;
    //heap_print(one_heap);
    //printf("\nFIN\n\n");
    if (!heap_is_empty(one_heap)){
        State_t *tmp = one_heap->value;
        if (height == 1){
            free(one_heap);
            *one_heap_ptr = NULL;
        }
        else if (height == 2){
            if (heap_is_empty(one_heap->right)){
                one_heap->value = one_heap->left->value;
                free(one_heap->left); /* on supprime l'élément le plus bas et le plus à droite */
                one_heap->left = NULL;
            }
            else{
                if (one_heap->left->value->weight >= one_heap->right->value->weight){
                    one_heap->value = one_heap->right->value;
                }
                else{
                    one_heap->value = one_heap->left->value;
                    one_heap->left->value = one_heap->right->value;
                }
                free(one_heap->right); /* on supprime l'élément le plus bas et le plus à droite */
                one_heap->right = NULL;
            }
            one_heap->nbNode -= 1;
        }
        else{
            unsigned int nbLastLeaf = one_heap->nbNode - (1<<(height-1)); /* cherche dans quelle branche on suprrime l'élément */
            if (nbLastLeaf < (1<<(height-2))){
                State_t *new_value = heap_pop(&one_heap->left, height-1);
                if (new_value->weight <= one_heap->right->value->weight){
                    one_heap->value = new_value;
                }else{
                    one_heap->value = one_heap->right->value;
                    one_heap->right->value = new_value;
                    rebalance(one_heap->right);
                }
            }
            else{
                State_t *new_value = heap_pop(&one_heap->right, height-1);
                assert(new_value != NULL);
                if (new_value->weight <= one_heap->left->value->weight){
                    one_heap->value = new_value;
                }else{
                    one_heap->value = one_heap->left->value;
                    one_heap->left->value = new_value;
                    rebalance(one_heap->left);
                }
            }
            one_heap->nbNode -= 1;
        }
        return tmp;
    }
    return NULL;
}


void heap_append(Heap_t **one_heap_ptr, State_t *one_value, unsigned int height){
    Heap_t *one_heap = *one_heap_ptr;
    if (heap_is_empty(one_heap)){
        one_heap = malloc(sizeof(Heap_t));
        one_heap->left = NULL;
        one_heap->right = NULL;
        one_heap->value = one_value;
        one_heap->nbNode = 1;
        *one_heap_ptr = one_heap; // Mettre à jour l'adresse pointée par one_heap_ptr
    }
    else{
        one_heap->nbNode += 1;
        State_t *tmp = one_heap->value;
        unsigned int nbLastLeaf = one_heap->nbNode - (1<<(height-1)); /* cherche dans quelle branche on ajoute l'élément */
        if (nbLastLeaf == (1<<(height-1))){
            height += 1;
            nbLastLeaf = 0;
        }
        if (one_value->weight < tmp->weight){
            one_heap->value = one_value;
        }
        else{
            tmp = one_value;
        }
        if (nbLastLeaf < (1<<(height-2))){
            heap_append(&one_heap->left, tmp, height-1);
        }
        else{
            heap_append(&one_heap->right, tmp, height-1);
        }
    }
}


void heap_print_station(Heap_t *one_heap, Table_t *one_table){
    if (!heap_is_empty(one_heap)){
        Station_t *one_station = table_get(one_table, one_heap->value->id_station);
        station_print(one_station);
        printf("gauche");
        heap_print_station(one_heap->left, one_table);
        printf("droite");
        heap_print_station(one_heap->right, one_table);
    }
    else{
        printf("\nvide\n");
    }
}


void heap_print(Heap_t *one_heap){
    if (!heap_is_empty(one_heap)){
        printf("\nid : %s, poids : %f\n", one_heap->value->id_station, one_heap->value->weight);
        printf("\ngauche");
        heap_print(one_heap->left);
        printf("\ndroite");
        heap_print(one_heap->right);
    }
    else{
        printf("\nvide\n");
    }
}

void heap_destroy(Heap_t *one_heap){
    if (!heap_is_empty(one_heap)){
        heap_destroy(one_heap->left);
        heap_destroy(one_heap->right);
        free(one_heap->value);
        free(one_heap);
    }
}


void state_destroy(State_t *one_state){
    free(one_state);
}
