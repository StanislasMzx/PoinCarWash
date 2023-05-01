#include "../src/heap.h"
#include <assert.h>


int main(){
    Heap_t *one_heap = heap_empty();
    assert(heap_is_empty(one_heap));
    assert(!heap_is_leaf(one_heap));
    assert((heap_height(one_heap) == 0));

    // We add an element in one_heap
    char *word1 = "jesuisentraindetester";
    State_t *one_state = state_create(word1, 142);
    heap_append(&one_heap, one_state, heap_height(one_heap));
    // test
    assert(!heap_is_empty(one_heap));
    assert(heap_is_leaf(one_heap));
    printf("\nheight : %u, nbNode : %u\n", heap_height(one_heap), one_heap->nbNode);


    // We add an element in one_heap
    char *word2 = "encore";
    State_t *other_state = state_create(word2, 42);
    heap_append(&one_heap, other_state, heap_height(one_heap));
    // test
    assert(!heap_is_empty(one_heap));
    assert(!heap_is_leaf(one_heap));
    printf("\nheight : %u, nbNode : %u\n", heap_height(one_heap), one_heap->nbNode);

    heap_destroy(one_heap);

}
