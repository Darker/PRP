#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Node Node;
struct Node{
    void* value;
    Node* prev;
    Node* next;
};

typedef struct {
   Node* head;
   Node* tail;
   size_t size;
   size_t limit;
} queue_t;

typedef queue_t Queue;

#define QUEUE_FOREACH_TAIL(item, queue)  \
    for(Node* item = queue->tail; item!=NULL; item = item->next)

/*
 * #define QUEUE_FOREACH_TAIL(item, queue)  \
    for(Node* item = queue->tail, *queue_foreach_next=queue->tail!=NULL?queue->tail->next:NULL; item!=NULL; item = queue_foreach_next)
*/
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

#endif /* __QUEUE_H__ */
