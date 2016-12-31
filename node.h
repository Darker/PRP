#ifndef NODE_H
#define NODE_H
#include "arrays.h"
#include "edge.h"
typedef struct Node Node;
struct Node {
    void* value;
    int name;
    Array* edges;
};

void node_add_edge(Node* node, Node* target, size_t cost) {
    Edge e;
    e.A = node;
    e.B = target;
    e.cost = cost;
    array_push(node->edges, &e);
}
Edge* node_find_edge(Node* node, Node* target) {
    for(size_t i=0, l=node->edges->length; i<l; ++i) {
        Edge* e = array_get(node->edges, i);
        if(e->B == target) {
            return e;
        }
    }
    return NULL;
}
#if defined(PREPROCESING_FOR_ODEVSYS) || defined(TESTING_HW)
#define NODE_C_QMAKE_IS_RETARDED "node.c"
#include NODE_C_QMAKE_IS_RETARDED
#endif
#endif // NODE_H
