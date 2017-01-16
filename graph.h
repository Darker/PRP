#ifndef GRAPH_H
#define GRAPH_H
//#ifdef GRAPH_C_ODEVSYS_IS_PROBLEMATIC_621
#include "arrays.h"
#include "node.h"
#include "edge.h"
//#else
//typedef struct Array Array;
//#ifndef NODE_H
//typedef struct Node Node;
//#endif
//#ifndef EDGE_H
//typedef struct Edge Edge;
//#endif
//#ifndef GRAPH_TYPES_H
//typedef int NodeName;
//#endif
//#endif
#include <stdbool.h>
typedef struct {
    Array* nodes;
    // If the graph is currently sorted (eg. label = index in node array)
    bool sorted;
    // if true, blank spaces between labels will be filled so that they are indexed by their label ID
    // this still doesn't 100% guarantee sorting
    bool forceSorting;
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

Graph* graph_create();
void graph_destroy(Graph**);

void graph_clear(Graph* const g);
Node* graph_find_node(const Graph* const g, const NodeName name);
Node* graph_add_node(Graph* const g, const NodeName name, void* value);
// doesn't check for node existence first
Node* graph_add_node_unsafe(Graph* const g, const NodeName name, void* value);
// Adds more nodes to graph to ensure it's fully sorted
// this only works if currently added node is larger than largest node
void graph_fill_before(Graph* const g, const NodeName addedNode);
#endif // GRAPH_H
