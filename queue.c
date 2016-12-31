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
    q->debug = false;
    q->tail = 0;
    q->head = 0;
    return q;
}
size_t queue_next_offset(const Queue* const q, const size_t current_offset);
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
    queue_print(q, "push_to_queue begin");

    size_t next_index = queue_next_offset(q, q->head);
    // If this triggers, the queue is full, the head has reached
    // the tail
    if(next_index==q->tail) {
        if(q->dynamic) {
            const size_t additional_memory = q->array->size;
            const size_t original_length = q->array->length;
            array_reserve(q->array, q->array->size*2);
            q->array->length = q->array->size;
            queue_print(q, "push_to_queue resize");
            // If head is overlooped the tail must be moved
            // all the way to the end of the array
            // Ilustration:
            // before:      |---H T------|
            // then:        |---H T------     |
            // this we want:|---H      T------|
            if(q->head < q->tail) {
                array_move(q->array, q->tail, q->tail+additional_memory, original_length-q->tail);
                q->tail = q->tail+additional_memory;
                queue_print(q, "push_to_queue moved");
            }
            next_index = queue_next_offset(q, q->head);
        }
        else {
            log_info("Queue is full with %d items.", queue_length(q));
            return false;
        }

    }

    array_set(q->array, q->head, &data);
    q->head = next_index;

    queue_print(q, "push_to_queue end");
    return true;
}
void queue_free_empty_space(Queue* q)
{
    if(q->array->length<10)
        return;
    // Amount of space to free is so that there's
    // minimum 1/2 of space occupied
    size_t occupied = queue_length(q);
    size_t free_space = q->array->length - occupied;
    size_t total_space = q->array->length;
    if(free_space < occupied)
        return;
    size_t to_free = total_space-2*occupied;
    if(to_free < occupied/2)
        return;
    queue_print(q, "queue_free_empty_space start");
    // Move if necessary
    // If head is overlooped the tail must be moved
    // so that existing elements don't get deleted
    // Ilustration:
    // before:      |---H     T------|
    // then:        |---H T------    |
    // this we want:|---H T------|
    if(q->head < q->tail) {
        array_move(q->array, q->tail, q->tail-to_free, total_space-q->tail);
        q->tail = q->tail-to_free;
        queue_print(q, "queue_free_empty_space moved");
    }
    // before:      |   T------H   |
    // then:        |T------H      |
    // this we want:|T------H|
    else {
        array_move(q->array, q->tail, 0, q->head - q->tail);
        q->head = q->head - q->tail;
        q->tail = 0;
        queue_print(q, "queue_free_empty_space moved");
    }
    array_free_mem(q->array, to_free);
    queue_print(q, "queue_free_empty_space end");
}
void* pop_from_queue(Queue* q)
{
    queue_print(q, "pop_from_queue begin");
    if(q->array->length == 0 || q->head==q->tail)
        return NULL;
    void* const crap = *(void**)array_get(q->array, q->tail);
    q->tail = queue_next_offset(q, q->tail);
    queue_print(q, "pop_from_queue end");
    queue_free_empty_space(q);
    return crap;
}

size_t queue_next_offset(const Queue* const q, const size_t current_offset) {
    size_t next = current_offset+1;
    if(next >= q->array->length) {
        next = 0;
    }
    return next;
}

void* get_from_queue(Queue* q, int idx)
{
    if(q->head == q->tail) {
        log_info("Cannot get from index because queue is empty.", NULL);
        return NULL;
    }

    if(idx<0) {
        log_info("Given index is negative.", NULL);
        return NULL;
    }
    size_t index = idx;

    if(q->head > q->tail && index >= queue_length(q)) {
        log_info("Given index reaches behind tail.", NULL);
        return NULL;
    }
    const size_t from_tail = q->array->length - q->tail;


    if(q->head < q->tail && q->tail+index>=q->array->length) {
        // |--H T--|
        index = (q->tail+index) - q->array->length;
        // Remember, head sits on an empty spot
        if(index >= q->head) {
            log_info("Given index reaches behind tail.", NULL);
            return NULL;
        }
        return *(void**)array_get(q->array, index);
    }
    else
        return *(void**)array_get(q->array, q->tail+index);
}



int get_queue_size(Queue* q)
{
    if(q->head == q->tail)
        return 0;
    else if(q->head>q->tail) {
        return q->head-q->tail;
    }
    else {
        return (q->head) + (q->array->length-q->tail);
    }
}
void queue_print(Queue* q, const char* const prefix) {
    if(!q->debug)
        return;
    if(prefix!=NULL) {
        printf(prefix);
        printf("\n    ");
    }
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
    printf("|\n");
}

bool queue_push(Queue* q, void* data) {
    return push_to_queue(q, data);
}

void* queue_pop(Queue* q) {
    return pop_from_queue(q);
}

void* queue_get(Queue* q, int offset) {
    return get_from_queue(q, offset);
}

int   queue_length(Queue* q) {
    return get_queue_size(q);
}

#endif

