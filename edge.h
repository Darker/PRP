#ifndef EDGE_H
#define EDGE_H
#include "graph_types.h"
typedef struct Edge {
    EdgeCost cost;
    NodeName A;
    NodeName B;
} Edge;

#if defined(PREPROCESING_FOR_ODEVSYS) || defined(TESTING_HW)
#define EDGE_C_QMAKE_IS_PROBLEMATIC "edge.c"
#include EDGE_C_QMAKE_IS_PROBLEMATIC
#endif
#endif // EDGE_H
