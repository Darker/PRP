#ifndef GRAPH_C_ODEVSYS_IS_PROBLEMATIC_621
#define GRAPH_C_ODEVSYS_IS_PROBLEMATIC_621
#include "graph.h"
#include <string.h>
//#include "node.h"
//#include "edge.h"
//#include "arrays.h"
FILE* file_from_string(const char* str, const char* mode) {
    FILE* file = NULL;
    if(strcmp(str, ":stdin") == 0) {
        if(mode[0]=='w')
            chyba("Cannot write in stdin!%s", 12,"");
        return stdin;
    }
    else if(strcmp(str, ":stdout") == 0) {
        if(mode[0]=='r')
            chyba("Cannot read from stdout!%s", 12,"");
        return stdout;
    }
    else {
        file = fopen(str, mode);
    }
    if(file == NULL) {
        chyba("File \"%s\" cannot be open.", 6+6+6, str);
    }
    else {
        return file;
    }
}

Graph* allocate_graph()
{
    Graph* g = NEW(Graph);
    g->nodes = array_create(sizeof(Node), 10);
    g->sorted = true;
    g->forceSorting = true;
    return g;
}
void free_graph(Graph** graph)
{
    Graph* g = *graph;
    graph_clear(g);
    array_destroy(g->nodes);
    g->nodes = NULL;
    free(g);
    *graph = NULL;
}

void graph_clear(Graph* const g)
{
    Node* currentNode;
    AR_FOREACH_PTR(currentNode, g->nodes, Node) {
        if(currentNode->edges != NULL) {
            array_destroy(currentNode->edges);
        }
    }
    array_clear(g->nodes);
    //return NULL;
}

void load_txt(const char* fname, Graph* g)
{
    FILE* stream = file_from_string(fname, "r");
    Node* node = NULL;
    graph_clear(g);
    NodeName max_node_name = 0;
    while(!feof(stream)) {
       int name, target, cost;
       if(fscanf(stream, "%d %d %d", &name, &target, &cost)!=3)
           break;
       if(target > max_node_name) {
           max_node_name = target;
       }
       if(name > max_node_name) {
           max_node_name = name;
       }
       // add new node
       if(node==NULL || node->name != name) {
           // Expand array as much as possible to predict
           array_reserve(g->nodes, max_node_name-1);
           // grab new node
           node = graph_add_node(g, name, NULL);
       }
       node_add_edge(node, target, cost);
    }
    fclose(stream);
}


void save_txt(const Graph* const g, const char* fname)
{
    FILE* stream = file_from_string(fname, "w");
    Node* currentNode;
    AR_FOREACH_PTR(currentNode, g->nodes, Node) {
        if(currentNode->edges != NULL) {
            Array* edges = currentNode->edges;
            const NodeName name = currentNode->name;
            for (size_t i = 0, l = edges->length; i < l; ++i) {
                Edge* e = array_get_fast(edges, i);
                fprintf(stream, "%u %u %u\n", name, e->B, e->cost);
            }
        }
    }
    fclose(stream);
}
const unsigned char header[] = {0xDE, 0xAD};

void save_bin(const Graph* const g, const char* fname)
{
    FILE* stream = file_from_string(fname, "wb");
    fwrite(header, sizeof(*header), 2, stream);
    Node* n;
    AR_FOREACH_PTR(n, g->nodes, Node) {
        node_write_binary(n, stream);
    }
    fclose(stream);
}
void load_bin(const char* fname, Graph* g)
{

    FILE* stream = file_from_string(fname, "rb");
    graph_clear(g);
    unsigned char read_header[2];
    size_t read = fread(read_header, sizeof(*header), 2, stream);
    if(read<2)
        chyba("Corrupted binary file %s!", 22, fname);
    if(read_header[0]!=header[0] || read_header[1]!=header[1])
        chyba("Invalid header in file %s!", 23, fname);

    Node n;
    n.edges = NULL;
    while(node_read_binary(&n, stream)) {
        array_push(g->nodes, &n);
        n.edges = NULL;
    }
    // If last element failed to initialize fully
    if(n.edges != NULL)
        array_destroy(n.edges);
    fclose(stream);
}
Node* graph_add_node(Graph * const g, const NodeName name, void *value) {
    Node* node = NULL;
    if((node=graph_find_node(g, name))!=NULL) {
        node->value = value;
        return node;
    }
    return graph_add_node_unsafe(g, name, value);
}


Node* graph_find_node(const Graph* const g, const NodeName name)
{
    if(g->sorted) {
        if(name < (int)g->nodes->length && name>=0) {
            return array_get_fast(g->nodes, name);
        }
    }
    else {
        Node* currentNode;
        AR_FOREACH_PTR(currentNode, g->nodes, Node) {
            if(currentNode->name == name) {
                return currentNode;
            }
        }
    }
    return NULL;
}
Node* graph_add_node_unsafe(Graph* const g, const NodeName name, void* value)
{
    Node node;
    node.costToStart = -1;
    node.parent = -1;
    node.edges = NULL;
    node.name = name;
    node.value = value;
    // Automatically mark graph as unsorted
    // this makes search slower
    if(g->sorted) {
        if(g->nodes->length > 0) {
            Node* const prev_node = array_get_fast(g->nodes, g->nodes->length-1);
            if(prev_node->name != name-1) {
                // sorting can only be forced when appending after the end
                if(g->forceSorting && name>prev_node->name) {
                    graph_fill_before(g, name);
                }
                else {
                    log_info("Graph is no longer ordered, nodes %d -> %d!", prev_node->name, name);
                    g->sorted = false;
                    g->forceSorting = false;
                }
            }
        }
        else if(name!=0) {
            if(g->forceSorting) {
                graph_fill_before(g, name);
            }
            else {
                log_info("Graph is no longer ordered, first node: %d!", name);
                g->sorted = false;
            }
        }
    }
    array_push(g->nodes, &node);
    return array_get_fast(g->nodes, g->nodes->length-1);
}
void graph_fill_before(Graph* const g, const NodeName addedNode)
{
    const Node* const lastNode = array_get_fast(g->nodes, g->nodes->length-1);
    NodeName nextName = lastNode->name;

    if(nextName+1>=addedNode) {
        return;
    }
    // reserve space in array for new nodes
    // that's rather optimal thing to do
    array_reserve(g->nodes, g->nodes->length+(size_t)(addedNode - nextName));
    log_info("Reserving space for %d new nodes starting from %d to %d",
             (int)(addedNode - nextName), (int)lastNode->name, (int)addedNode);
    while((++nextName) < addedNode) {
        graph_add_node_unsafe(g, nextName, NULL);
    }
}

void graph_destroy(Graph** g){free_graph(g);}
Graph* graph_create() {return allocate_graph();}
#endif








