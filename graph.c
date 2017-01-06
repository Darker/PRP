#ifndef GRAPH_C_ODEVSYS_IS_FUCKING_RETARDED_621
#define GRAPH_C_ODEVSYS_IS_FUCKING_RETARDED_621
#include "edge.h"
#include "node.h"
#include "graph.h"
#include "arrays.h"

Graph* allocate_graph()
{
    Graph* g = NEW(Graph);
    g->nodes = array_create(sizeof(Node), 10);
}
void free_graph(Graph** graph)
{
    Graph* g = *graph;
    array_destroy(g->nodes);
    free(g);
    *graph = NULL;
}
void load_txt(const char* fname, Graph* g)
{
    FILE* input = NULL;
    if((input = fopen(fname, "r")) == NULL) {
        chyba("File \"%s\" cannot be open.", 6+6+6, file);
    }
    array_clear(g->nodes);
    while(feof(input)) {
       int name, target, cost;
       fscanf(input, "%d %d %d", name, target, cost);

    }
}

void load_bin(const char* fname, Graph* g)
{

}
void save_txt(const Graph* const g, const char* fname)
{

}

void save_bin(const Graph* const g, const char* fname)
{

}
#endif



