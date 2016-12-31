#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <stdbool.h>
#include <stdlib.h>

#if defined(HOME) || defined(QUEUE_C_MODULE) || defined(PREPROCESING_FOR_ODEVSYS)

#include "arrays.h"

/* Queue structure which holds all necessary data */
typedef struct {
   Array* array;
   bool dynamic;
   // this is where we grab bitches
   // after we get closer to tail
   size_t head;
   // This is where we put bitches
   // afterwards, tail grows
   size_t tail;
   bool debug;
} queue_t;
#else

typedef void queue_t;
#endif
typedef queue_t Queue;

/* creates a new queue with a given size */
Queue* create_queue(int capacity);
/* deletes the queue and all allocated memory */
void delete_queue(Queue *queue);

/*
 * PUSHES TO THE FUCKING TAIL
 * returns: true on success; false otherwise
 */
bool push_to_queue(Queue* queue, void* data);

/*
 * RETRIEVES FROM THE FUCKING HEAD
 * returns: the first element on success; NULL otherwise
 */
void* pop_from_queue(Queue* queue);

/*
 * LOOPS OVER QUEUE AS A FUCKING IDIOT!
 * returns: the idx-th element on success; NULL otherwise
 */
void* get_from_queue(Queue* queue, int idx);

/* Size is cached */
int get_queue_size(Queue* q);
void queue_print(Queue* q, const char* const prefix);

bool queue_push(Queue* q, void* data);
void* queue_pop(Queue* q);
void* queue_get(Queue* q, int offset);
int   queue_length(Queue* q);
void  queue_free_empty_space(Queue* q);
#if defined(PREPROCESING_FOR_ODEVSYS) || defined(TESTING_HW)
#define QUEUE_C_QMAKE_IS_RETARDED "queue.c"
#include QUEUE_C_QMAKE_IS_RETARDED
#endif

#endif /* __QUEUE_H__ */
