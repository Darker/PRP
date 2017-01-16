#ifndef NODE_C_ODEVSYS_IS_PROBLEMATIC_624
#define NODE_C_ODEVSYS_IS_PROBLEMATIC_624
#include "node.h"
#include <stdbool.h>
#include "edge.h"

void node_add_edge(Node* node, NodeName target, size_t cost) {
    Edge e;
    e.A = node->name;
    e.B = target;
    e.cost = cost;
    if(node->edges == NULL) {
        node->edges = array_create(sizeof(Edge), 2);
    }
    array_push(node->edges, &e);
}
Edge* node_find_edge(Node* node, NodeName target) {
    for(size_t i=0, l=node->edges->length; i<l; ++i) {
        Edge* e = array_get(node->edges, i);
        if(e->B == target) {
            return e;
        }
    }
    return NULL;
}
void node_write_binary(const Node* const node, FILE* stream)
{
    const NodeName name = node->name;
    const size_t len = node->edges->length;

    // node name
    fwrite(&name, sizeof(NodeName), 1, stream);
    //fprintf(stream, "no_edges:");
    fwrite(&len, sizeof(len), 1, stream);
    Edge* e;
    AR_FOREACH_PTR(e, node->edges, Edge) {
        fwrite(&(e->B), sizeof(NodeName), 1, stream);
        fwrite(&(e->cost), sizeof(e->cost), 1, stream);
    }
}
bool node_read_binary(Node* const n, FILE* stream) {
    if(fread(&(n->name), sizeof(NodeName), 1, stream) != 1)
        return false;

    //char test = '\0';
    //if(fscanf(stream, "no_edges%c", &test)!=1)
    //    return false;
    //if(test!=':')
    //    return false;
    // Loop over edges
    size_t no_edges = 0;
    if(fread(&no_edges, sizeof(no_edges), 1, stream) != 1)
        return false;
    if(no_edges > 0) {
        n->edges = array_create(sizeof(Edge), no_edges);
        for (int i = 0, l = no_edges; i < l; ++i) {
            // B's name and then cost
            NodeName target;
            EdgeCost cost;
            bool success = true;
            if(fread(&target, sizeof(NodeName), 1, stream) != 1) {
                success = false;
            }
            else if(fread(&cost, sizeof(EdgeCost), 1, stream) != 1) {
                success = false;
            }
            if(!success) {
                if(feof(stream)) {
                    log_info("File over error at node #%d", (int)n->name);
                }
                else {
                    log_warn("File error #%d at node #%d", ferror(stream), (int)n->name);
                }
                array_destroy(n->edges);
                n->edges = NULL;
                return false;
            }
            Edge e;
            e.A = n->name;
            e.B = target;
            e.cost = cost;
            array_push(n->edges, &e);
        }
    }
    return true;
}

#endif



