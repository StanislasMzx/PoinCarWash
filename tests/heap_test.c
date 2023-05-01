#define SNOW_ENABLED

#include "snow.h"
#include "../src/heap.h"

describe(test_heap)
{
    it("Structure test")
    {
        Heap_t *one_heap = heap_empty();
        assert(heap_is_empty(one_heap));
        assert(!heap_is_leaf(one_heap));
        asserteq_uint(heap_height(one_heap), 0);

        // We add an element in one_heap
        char *word1 = "jesuisentraindetester";
        State_t *one_state = state_create(word1, 142);
        heap_append(&one_heap, one_state, heap_height(one_heap));
        // test
        assert(!heap_is_empty(one_heap));
        assert(heap_is_leaf(one_heap));
        asserteq_uint(heap_height(one_heap), 1);

        // We add an element in one_heap
        char *word2 = "etjesperequecamarche";
        State_t *other_state = state_create(word2, 54);
        heap_append(&one_heap, other_state, heap_height(one_heap));
        // test
        assert(!heap_is_empty(one_heap));
        assert(!heap_is_leaf(one_heap));
        asserteq_uint(heap_height(one_heap), 2);

        // We add an element in one_heap
        char *word3 = "parcelopposeneminteressepas";
        State_t *again_state = state_create(word3, 4);
        heap_append(&one_heap, again_state, heap_height(one_heap));
        // test
        assert(!heap_is_empty(one_heap));
        assert(!heap_is_leaf(one_heap));
        asserteq_uint(heap_height(one_heap), 2);

        // We add an element in one_heap
        char *word4 = "entoutcasjycrois";
        State_t *more_state = state_create(word4, 78);
        heap_append(&one_heap, more_state, heap_height(one_heap));
        // test
        assert(!heap_is_empty(one_heap));
        assert(!heap_is_leaf(one_heap));
        asserteq_uint(heap_height(one_heap), 3);

        // pop a element
        State_t *s1 = heap_pop(&one_heap, heap_height(one_heap));
        // test
        assert(!heap_is_empty(one_heap));
        assert(!heap_is_leaf(one_heap));
        asserteq_str(s1->id_station, word3);
        asserteq_dbl(s1->weight, 4);
        asserteq_uint(heap_height(one_heap), 2);
        state_destroy(s1);

        // pop a element
        State_t *s2 = heap_pop(&one_heap, heap_height(one_heap));
        // test
        assert(!heap_is_empty(one_heap));
        assert(!heap_is_leaf(one_heap));
        asserteq_str(s2->id_station, word2);
        asserteq_dbl(s2->weight, 54);
        asserteq_uint(heap_height(one_heap), 2);
        state_destroy(s2);

        // pop a element
        State_t *s3 = heap_pop(&one_heap, heap_height(one_heap));
        // test
        assert(!heap_is_empty(one_heap));
        assert(heap_is_leaf(one_heap));
        asserteq_str(s3->id_station, word4);
        asserteq_dbl(s3->weight, 78);
        asserteq_uint(heap_height(one_heap), 1);
        state_destroy(s3);

        // pop a element
        State_t *s4 = heap_pop(&one_heap, heap_height(one_heap));
        // test
        assert(heap_is_empty(one_heap));
        assert(!heap_is_leaf(one_heap));
        asserteq_str(s4->id_station, word1);
        asserteq_dbl(s4->weight, 142);
        asserteq_uint(heap_height(one_heap), 0);
        state_destroy(s4);

        defer(heap_destroy(one_heap));
    }
}

snow_main();
