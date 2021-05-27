%module graph
#include "graph_wrap_py.h"
%{
#include "graph_wrap_py.h"
%}

Graph* create();
void destroy(Graph* graph);
Graph* addVertex(Graph* graph, char* v);
Graph* addEdge(Graph* graph, char* v1, char* v2);
void disp(Graph* graph);
Graph* graphUnion(Graph* graph_in1, Graph* graph_in2, Graph* graph_out);
Graph* graphIntersection(Graph* graph_in1, Graph* graph_in2, Graph* graph_out); 
Graph* graphDifference(Graph* graph_in1, Graph* graph_in2, Graph* graph_out); 
Graph* graphProduct(Graph* graph_in1, Graph* graph_in2, Graph* graph_out); 
Graph* graphComplement(Graph* graph_in, Graph* graph_out); 
