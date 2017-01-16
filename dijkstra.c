#include "dijkstra.h"
#include "arrays.h"
#include "graph.h"
#include "node.h"
#include "pq_heap.h"

Dijk* dijkstra_init()
{
    Dijk* d = NEW(Dijk);
    d->graph = graph_create();
    return d;
}

bool dijkstra_load_graph(const char* filename, Dijk* dijkstra)
{
    load_txt(filename, dijkstra->graph);
    return true;
}

bool dijkstra_set_graph(int e, int edges[][3], Dijk* dijkstra)
{
    Graph* const g = dijkstra->graph;
    NodeName last_node_name = 0;
    Node* node = NULL;
    for (int i = 0, l = e; i < l; ++i) {
        int* edge = edges[i];
        if(last_node_name!=edge[EDGES_NAME]) {
            node = NULL;
        }
        if(node == NULL) {
            node = graph_add_node(g, edge[EDGES_NAME], NULL);
            last_node_name = node->name;
        }
        node_add_edge(node, edge[EDGES_TARGET], edge[EDGES_COST]);
        // add edge to prevent overly common realloc
        if(edge[EDGES_TARGET] > node->name) {
            graph_add_node(g, edge[EDGES_TARGET], NULL);
            // array may have changed, pointer invalid
            node = NULL;
        }
    }
    return true;
}
bool dijkstra_solve(Dijk* dijkstra, NodeName label)
{
    if (!dijkstra || label < 0 || label >= (int)dijkstra->graph->nodes->length) {
       return false;
    }
    dijkstra->start_node = label;

    void* pq = pq_alloc(dijkstra->graph->nodes->length);

    Node* start = graph_find_node(dijkstra->graph, label);
    start->costToStart = 0;
    pq_push(pq, label, 0);

    NodeName cur_label;
    while ( !pq_is_empty(pq) && pq_pop(pq, &cur_label) ) {
       Node* cur = graph_find_node(dijkstra->graph, cur_label);
       Edge e;
       // if node has no edges, we just ignore it
       if(cur->edges==NULL || cur->edges->length==0)
           continue;
       AR_FOREACH(e, cur->edges, Edge) {
           Node* to = graph_find_node(dijkstra->graph, e.B);
           if( to==NULL ) {
               // add target node in graph
               to = graph_add_node(dijkstra->graph, e.B, NULL);
               // fix possibly lost pointer due to realloc
               cur = graph_find_node(dijkstra->graph, cur_label);
           }
           const int cost = cur->costToStart + e.cost;
           if (to->costToStart == -1) { // the node to has not been visited yet
              to->costToStart = cost;
              to->parent = cur_label;
              pq_push(pq, e.B, cost);
           } else if (cost < to->costToStart) { // already relaxed check if we can make a shortcut to child node via the current node
              to->costToStart = cost;
              to->parent = cur_label;
              pq_update(pq, e.B, cost);
           }
       }
    } // end pq_is_empty

    pq_free(pq); // release the memory

    return true;
}
bool dijkstra_get_solution(const Dijk* dijkstra, int n, int solution[][3])
{
    Node* cur_node;
    size_t iterator = 0;
    if(n<(int)dijkstra->graph->nodes->length) {
        log_warn("Too small result array given - only %d fields for %d nodes.", (int)n, (int)dijkstra->graph->nodes->length);
        return false;
    }
    AR_FOREACH_PTR(cur_node, dijkstra->graph->nodes, Node) {
        solution[iterator][SOLUTON_NODE_NAME] = cur_node->name;
        solution[iterator][SOLUTON_PARENT] = cur_node->parent;
        solution[iterator][SOLUTION_COST] = cur_node->costToStart;
        ++iterator;
    }
    return true;
}

bool dijkstra_save_path(const Dijk* dijkstra, const char* filename)
{
    bool ret = false;
    if (dijkstra) {
       FILE *f = fopen(filename, "w");
       if (f) {
           Node* cur_node;
           AR_FOREACH_PTR(cur_node, dijkstra->graph->nodes, Node) {
               fprintf(f, "%i %i %i\n", cur_node->name, cur_node->costToStart, cur_node->parent);
           }
           ret = fclose(f) == 0;
       }
    }
    return ret;
}

void dijkstra_free(Dijk* dijkstra)
{
    graph_destroy(&(dijkstra->graph));
    free(dijkstra);
}
