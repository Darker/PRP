#ifndef GRAPH_H
#define GRAPH_H
#include "arrays.h"
#include "node.h"
typedef struct {
    Array* nodes;
} graph_t;
typedef graph_t Graph;
/* Allocate a new graph and return a reference to it. */
Graph* allocate_graph();
/* Free all allocated memory and set reference to the graph to NULL. */
void free_graph(Graph** graph);

/* Load a graph from the text file. */
void load_txt(const char* fname, Graph *g);
/* Load a graph from the binary file. */
void load_bin(const char* fname, Graph *g);

/* Save the graph to the text file. */
void save_txt(const Graph* const g, const char *fname);
/* Save the graph to the binary file. */
void save_bin(const Graph* const g, const char *fname);
// note to self
// do methods to find nodes and shit


#endif // GRAPH_H
