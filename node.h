#ifndef NODE_H
#define NODE_H
#include "arrays.h"
#include "edge.h"
// Name of a node in the graph
typedef int NodeName;
typedef struct Node Node;
struct Node {
    void* value;
    int name;
    Array* edges;
};

void node_add_edge(Node* node, Node* target, size_t cost);
Edge* node_find_edge(Node* node, Node* target);
#if defined(PREPROCESING_FOR_ODEVSYS) || defined(TESTING_HW)
#define NODE_C_QMAKE_IS_RETARDED "node.c"
#include NODE_C_QMAKE_IS_RETARDED
#endif
#endif // NODE_H
