#ifndef DIJKSTRA_H
#define DIJKSTRA_H
#include "graph.h"
#include <stdbool.h>

typedef struct Dijk {
    Graph* graph;
    NodeName start_node;
} Dijk;
/*
 * Initialize structure for storing graph, solution, and eventual
 * variables for dijkstra algorithm
 *
 * return: point to the allocated structure; NULL on an error
 */
Dijk* dijkstra_init(void);


/*
 * Load input graph (in text format) from the given file and store it
 * to the dijkstra structure previously created by dijkstra_init()
 *
 * return: true on success; false otherwise
 */
bool dijkstra_load_graph(const char *filename, Dijk* dijkstra);


/*
 * Set the graph to the dijkstra structure instead of direct
 * loading the file as in dijkstra_load_graph() function.
 * The given array edges should not be directly used for the
 * internal representation of the graph in the dijkstra algorithm
 *
 * return: true on success; false on an error, e.g., memory allocation
 */
bool dijkstra_set_graph(int e, int edges[][3], Dijk* dijkstra);

/*
 * Solve the dijsktra algorithm on the graph previously
 * loaded by the dijkstra_load_graph() set by dijkstra_set_graph()
 * The solution is stored in some internal structure of the dijkstra
 * type passed to the function
 *
 * return: true on success; false otherwise
 */
bool dijkstra_solve(Dijk* dijkstra, NodeName label);


/*
 * Retrived the solution found by the function dijkstra_solve()
 * It is assumed the passed argument solution[][3] is properly allocated,
 * and thus the internal solution of the dijkstra can used to fill the
 * solution[][3].
 *
 * return: true on success; false otherwise
 */
bool dijkstra_get_solution(const Dijk* dijkstra, int n, int solution[][3]);


/*
 * Directly solve the solution found by the dijkstra_solve() in to the
 * file (in the text format) with the given filename.
 *
 * return: true on success; false otherwise
 */
bool dijkstra_save_path(const Dijk* dijkstra, const char *filename);


/*
 * Release on allocated memory of the passed structure for
 * the dijkstra algorithm, e.g., graph as an array of pointers to struct edge
 * and solution as an array of pointers to struct node.
 * All previosly allocated memory related to solution of the shortest
 * paths is freed
 */
void dijkstra_free(Dijk* dijkstra);
#endif // DIJKSTRA_H
