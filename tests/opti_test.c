#include "../src/stations.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

struct _List_coord
{
    struct _List_coord *next;
    int lat;
    int lon;
    int len;
};
typedef struct _List_coord List_coord;


List_coord *empty(){
    return NULL;
}

List_coord *append(List_coord *one_list, int one_lat, int one_lon){
    List_coord *new = malloc(sizeof(List_coord));
    new->next = one_list;
    new->lat = one_lat;
    new->lon = one_lon;
    if (one_list == NULL){
        new->len = 1;
    }else{
        new->len = one_list->len + 1;
    }
    return new;
}


bool contain(List_coord *one_list, int one_lat, int one_lon){
    List_coord *other = one_list;
    bool contained = false;
    while (!contained && other != NULL){
        if (other->lat == one_lat && other->lon == one_lon){
            contained = true;
        }
        other = other->next;
    }
    return contained;
}



void destroy(List_coord *one_list){
    if (one_list != NULL){
        destroy(one_list->next);
        free(one_list);
    }
}


int main(){
    Table_t *table = load_stations("../data/raw/consolidation-etalab-schema-irve-statique-v-2.2.0-20230415.csv");
    List_coord *coords = empty();
    int count = 0;
    int count2 = 0;
    for (int i = 0; i<table->length; i++){
        List_t *one_list = table->slots[i];
        for (int j = 0; j<one_list->length; j++){
            Element_t *element = &one_list->list[j];
            if (!contain(coords, element->value->coordinates->latitude, element->value->coordinates->longitude)){
                coords = append(coords, element->value->coordinates->latitude, element->value->coordinates->longitude);
            }
            else{
                count += 1;
            }
            count2 += 1;
        }
    }
    printf("gagné : %d, nombre coord : %d, en tout : %d, donc %d == %d", coords->len, count, count2, coords->len+count, count2);
    destroy(coords);
    table_destroy(table);
}
