#define SNOW_ENABLED

#include "snow.h"
#include "../src/heap.h"

describe(test_heap_add)
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

        char *word4 = "entoutcasjycrois";
        State_t *more_state = state_create(word4, 78);
        heap_append(&one_heap, more_state, heap_height(one_heap));
        // test
        assert(!heap_is_empty(one_heap));
        assert(!heap_is_leaf(one_heap));
        asserteq_uint(heap_height(one_heap), 3);

        char *word5 = "5";
        State_t *state5 = state_create(word5, 4);
        heap_append(&one_heap, state5, heap_height(one_heap));
        // test
        assert(!heap_is_empty(one_heap));
        assert(!heap_is_leaf(one_heap));
        asserteq_uint(heap_height(one_heap), 3);

        char *word6 = "6";
        State_t *state6 = state_create(word6, 4);
        heap_append(&one_heap, state6, heap_height(one_heap));
        // test
        assert(!heap_is_empty(one_heap));
        assert(!heap_is_leaf(one_heap));
        asserteq_uint(heap_height(one_heap), 3);

        char *word7 = "7";
        State_t *state7 = state_create(word7, 7);
        heap_append(&one_heap, state7, heap_height(one_heap));
        // test
        assert(!heap_is_empty(one_heap));
        assert(!heap_is_leaf(one_heap));
        asserteq_uint(heap_height(one_heap), 3);

        char *word8 = "8";
        State_t *state8 = state_create(word8, 8);
        heap_append(&one_heap, state8, heap_height(one_heap));
        // test
        assert(!heap_is_empty(one_heap));
        assert(!heap_is_leaf(one_heap));
        asserteq_uint(heap_height(one_heap), 4);

        char *word9 = "9";
        State_t *state9 = state_create(word9, 9);
        heap_append(&one_heap, state9, heap_height(one_heap));
        // test
        assert(!heap_is_empty(one_heap));
        assert(!heap_is_leaf(one_heap));
        asserteq_uint(heap_height(one_heap), 4);


        defer(heap_destroy(one_heap));
    }
}


describe(test_heap_pop)
{
    it("Structure test")
    {
        Heap_t *one_heap = heap_empty();

        // We add an element in one_heap
        char *word1 = "jesuisentraindetester";
        State_t *one_state = state_create(word1, 142);
        heap_append(&one_heap, one_state, heap_height(one_heap));

        // We add an element in one_heap
        char *word2 = "etjesperequecamarche";
        State_t *other_state = state_create(word2, 54);
        heap_append(&one_heap, other_state, heap_height(one_heap));

        // We add an element in one_heap
        char *word3 = "parcelopposeneminteressepas";
        State_t *again_state = state_create(word3, 4);
        heap_append(&one_heap, again_state, heap_height(one_heap));

        char *word4 = "entoutcasjycrois";
        State_t *more_state = state_create(word4, 78);
        heap_append(&one_heap, more_state, heap_height(one_heap));

        char *word5 = "5";
        State_t *state5 = state_create(word5, 4);
        heap_append(&one_heap, state5, heap_height(one_heap));

        char *word6 = "6";
        State_t *state6 = state_create(word6, 4);
        heap_append(&one_heap, state6, heap_height(one_heap));

        char *word7 = "7";
        State_t *state7 = state_create(word7, 7);
        heap_append(&one_heap, state7, heap_height(one_heap));

        char *word8 = "8";
        State_t *state8 = state_create(word8, 8);
        heap_append(&one_heap, state8, heap_height(one_heap));

        char *word9 = "9";
        State_t *state9 = state_create(word9, 9);
        heap_append(&one_heap, state9, heap_height(one_heap));


        // pop a element
        State_t *s1 = heap_pop(&one_heap, heap_height(one_heap));
        // test
        assert(!heap_is_empty(one_heap));
        assert(!heap_is_leaf(one_heap));
        asserteq_str(s1->id_station, word3);
        asserteq_dbl(s1->weight, 4);
        asserteq_uint(heap_height(one_heap), 4);

        // pop a element
        State_t *s2 = heap_pop(&one_heap, heap_height(one_heap));
        // test
        assert(!heap_is_empty(one_heap));
        assert(!heap_is_leaf(one_heap));
        asserteq_str(s2->id_station, word5);
        asserteq_dbl(s2->weight, 4);
        asserteq_uint(heap_height(one_heap), 3);

        // pop a element
        State_t *s3 = heap_pop(&one_heap, heap_height(one_heap));
        // test
        assert(!heap_is_empty(one_heap));
        assert(!heap_is_leaf(one_heap));
        asserteq_str(s3->id_station, word6);
        asserteq_dbl(s3->weight, 4);
        asserteq_uint(heap_height(one_heap), 3);

        // pop a element
        State_t *s4 = heap_pop(&one_heap, heap_height(one_heap));
        // test
        assert(!heap_is_empty(one_heap));
        assert(!heap_is_leaf(one_heap));
        asserteq_str(s4->id_station, word7);
        asserteq_dbl(s4->weight, 7);
        asserteq_uint(heap_height(one_heap), 3);

        // pop a element
        State_t *s5 = heap_pop(&one_heap, heap_height(one_heap));
        // test
        assert(!heap_is_empty(one_heap));
        assert(!heap_is_leaf(one_heap));
        asserteq_str(s5->id_station, word8);
        asserteq_dbl(s5->weight, 8);
        asserteq_uint(heap_height(one_heap), 3);

        // pop a element
        State_t *s6 = heap_pop(&one_heap, heap_height(one_heap));
        // test
        assert(!heap_is_empty(one_heap));
        assert(!heap_is_leaf(one_heap));
        asserteq_str(s6->id_station, word9);
        asserteq_dbl(s6->weight, 9);
        asserteq_uint(heap_height(one_heap), 2);

        // pop a element
        State_t *s7 = heap_pop(&one_heap, heap_height(one_heap));
        // test
        assert(!heap_is_empty(one_heap));
        assert(!heap_is_leaf(one_heap));
        asserteq_str(s7->id_station, word2);
        asserteq_dbl(s7->weight, 54);
        asserteq_uint(heap_height(one_heap), 2);

        // pop a element
        State_t *s8 = heap_pop(&one_heap, heap_height(one_heap));
        // test
        assert(!heap_is_empty(one_heap));
        assert(heap_is_leaf(one_heap));
        asserteq_str(s8->id_station, word4);
        asserteq_dbl(s8->weight, 78);
        asserteq_uint(heap_height(one_heap), 1);

        // pop a element
        State_t *s9 = heap_pop(&one_heap, heap_height(one_heap));
        // test
        assert(heap_is_empty(one_heap));
        assert(!heap_is_leaf(one_heap));
        asserteq_str(s9->id_station, word1);
        asserteq_dbl(s9->weight, 142);
        asserteq_uint(heap_height(one_heap), 0);

        defer(state_destroy(one_state));
        defer(state_destroy(more_state));
        defer(state_destroy(again_state));
        defer(state_destroy(other_state));
        defer(state_destroy(state5));
        defer(state_destroy(state6));
        defer(state_destroy(state7));
        defer(state_destroy(state8));
        defer(state_destroy(state9));

        defer(heap_destroy(one_heap));
    }
}


snow_main();
