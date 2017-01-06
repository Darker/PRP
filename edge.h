#ifndef EDGE_H
#define EDGE_H
#include "node.h"
typedef struct Edge Edge;
struct Edge{
    size_t cost;
    Node* A;
    NodeName targetNode;
};
#if defined(PREPROCESING_FOR_ODEVSYS) || defined(TESTING_HW)
#define EDGE_C_QMAKE_IS_RETARDED "edge.c"
#include EDGE_C_QMAKE_IS_RETARDED
#endif
#endif // EDGE_H
