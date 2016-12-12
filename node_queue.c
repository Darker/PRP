#include "node_queue.h"
#include "cpp_tools.h"
void node_append_next(Node*const target, Node*const new_node);
void node_append_prev(Node*const target, Node*const new_node);

queue_t* create_queue(int capacity)
{
    Queue* queue = NEW(Queue);
    queue->head = NULL;
    queue->tail = NULL;
    queue->size = 0;
    queue->limit = capacity;
    return queue;
}

void delete_queue(Queue* queue)
{
    QUEUE_FOREACH_TAIL(item, queue) {
        free(item->value);
        item->value = NULL;
        if(item->prev != NULL)
            free(item->prev);
        if(item->next == NULL)
            free(item);
    }
    free(queue);
}

bool push_to_queue(Queue* queue, void* data)
{
    if(queue->size == queue->limit)
        return false;
    Node* node = NEW(Node);
    node->next = NULL;
    node->prev = NULL;
    node->value = data;

    if(queue->size == 0) {
        queue->head = node;
        queue->tail = node;
        queue->size = 1;
    }
    else {
        node_append_prev(queue->tail, node);
        queue->tail = node;
        queue->size++;
    }
}

void* pop_from_queue(Queue* queue)
{
    if(queue->size == 0 || queue->head == NULL)
        return NULL;
    Node* node = queue->head;

    if(queue->size == 0) {
        queue->head = node;
        queue->tail = node;
        queue->size = 1;
    }
    else {
        node_append_prev(queue->tail, node);
        queue->tail = node;
        queue->size++;
    }
}
/**
 * Starting state:
 *   HEAD - target - TAIL
 * Ending state:
 *   HEAD - new_node - target - TAIL
 */
void node_append_next(Node*const target, Node*const new_node) {
    Node* HEAD = target->next;
    if( HEAD != NULL) {
        HEAD->prev = new_node;
        new_node->next = HEAD;
    }
    new_node->prev = target;
    target->next = new_node;
}
/**
 * Starting state:
 *   HEAD - target - TAIL
 * Ending state:
 *   HEAD - target - new_node - TAIL
 */
void node_append_prev(Node*const target, Node*const new_node) {
    Node* TAIL = target->prev;
    if(TAIL != NULL) {
        TAIL->next = new_node;
        new_node->prev = TAIL;
    }
    new_node->next = target;
    target->prev = new_node;
}

/**
 * Starting state:
 *   HEAD - target - TAIL
 * Ending state:
 *   HEAD - TAIL
 */
void node_remove(Node*const target) {
    Node* TAIL = target->prev;
    Node* HEAD = target->next;
    free(node);
}

