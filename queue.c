//#ifndef PREPROCESING_FOR_ODEVSYS
#ifndef QUEUE_C_MODULE
#define QUEUE_C_MODULE

#include "queue.h"
//#else
//#include <queue.h>
//#endif
queue_t* create_queue(int capacity)
{
    Queue* q = NEW(Queue);
    q->array = array_create(sizeof(void*), capacity);
    q->array->length = capacity;
    q->dynamic = !false;
    q->empty = true;
    q->tail = 0;
    q->head = 0;
    return q;
}
size_t queue_next_offset(const Queue* const q);
void delete_queue(Queue* q)
{
    array_destroy(q->array);
    free(q);
}
// at the beginning:
//              |T--H             |
//              | T-H             |
// this we want:|---H  T----------|
bool push_to_queue(Queue* q, void* data)
{
    size_t next_index = queue_next_offset(q);
    // If this triggers, the queue is full, the head has reached
    // the tail
    if(next_index==q->tail) {
        if(q->dynamic) {
            const size_t additional_memory = q->array->size;
            const size_t original_length = q->array->length;
            array_reserve(q->array, q->array->size*2);
            // If head is overlooped the tail must be moved
            // all the way to the end of the array
            // Ilustration:
            // before:      |---H T------|
            // then:        |---H T------     |
            // this we want:|---H      T------|
            if(q->head < q->tail) {
                q->array->length = q->array->size;
                array_move(q->array, q->tail, q->tail+additional_memory, original_length-q->tail);
            }
        }
        else
            return false;
    }
    if(q->array->length == q->array->length && !q->dynamic)
        return false;
    array_push(q->array, &data);
    return true;
}

void* pop_from_queue(Queue* q)
{
    if(q->array->length == 0)
        return NULL;
    void* crap;
    array_shift(q->array, &crap);
    return crap;
}

size_t queue_next_offset(const Queue* const q) {
    size_t next = q->head-1;
    if(next >= q->array->length) {
        next = 0;
    }
    return next;
}

void* get_from_queue(Queue* q, int idx)
{
    /*if(idx>-1 && idx<q->array->length)
        return *(void**)array_get(q->array, idx);
    else
        return NULL;*/

}


int get_queue_size(Queue* q)
{
    return q->array->length;
}
void print_queue(Queue* q) {
    printf("|", NULL);
    for(size_t i=0; i<q->array->length; ++i) {
        if(i == q->head) {
            printf(q->head!=q->tail ? "H":"X");
        }
        else if(i == q->tail) {
            printf("T");
        }
        else if(q->head>q->tail) {
            printf(i<q->head && i>q->tail ? "-":" ");
        }
        else if(q->head<q->tail) {
            printf(i<q->head || i>q->tail ? "-":" ");
        }
        else {
            printf(" ");
        }
    }
    printf("\n");
}

#endif
