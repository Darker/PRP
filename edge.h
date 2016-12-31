#ifndef EDGE_H
#define EDGE_H
typedef struct Edge Edge;
struct Edge{
    void* value;
    Node* prev;
    Node* next;
};
#endif // EDGE_H
