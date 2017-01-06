#ifndef NODE_C_ODEVSYS_IS_FUCKING_RETARDED_621
#define NODE_C_ODEVSYS_IS_FUCKING_RETARDED_621
#include "node.h"
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
#endif


