#ifndef NODE_H
#define NODE_H
#include "arrays.h"
#include "graph_types.h"

#include "edge.h"

typedef struct Node {
    void* value;
    NodeName name;
    Array* edges;
    EdgeCost costToStart;
    NodeName parent;
} Node;

void node_add_edge(Node* node, NodeName target, size_t cost);
Edge* node_find_edge(Node* node, NodeName target);
void node_write_binary(const Node* const node, FILE* stream);
bool node_read_binary(Node* const node, FILE* stream);
#if defined(PREPROCESING_FOR_ODEVSYS) || defined(TESTING_HW)
#define NODE_C_QMAKE_IS_PROBLEMATIC "node.c"
#include NODE_C_QMAKE_IS_PROBLEMATIC
#endif
#endif // NODE_H
