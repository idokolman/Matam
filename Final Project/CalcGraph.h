#ifndef CALCGRAPH_H
#define CALCGRAPH_H
#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <iterator>
#include <set>
class Graph{
    private:

    std::set<std::string> vertices;
    std::set<std::pair<std::string,std::string>> edges;

    public:

    Graph();
    Graph(std::set<std::string> vertices,std::set<std::pair<std::string,std::string>> edges);
    ~Graph();
    Graph& operator=(const Graph& other_graph);
    Graph(const Graph& copy_graph);
    void addVertex(std::string vertex);
    std::set<std::string> getVertices();
    std::set<std::pair<std::string,std::string>> getEdges();
    bool findVertex(std::string vertex) const;
    bool findEdge(std::pair<std::string,std::string> edge) const;
    void addEdge(std::pair<std::string,std::string> edge);
    void removeVertex(std::string vertex);
    std::string toString() const;
    unsigned int sizeVertices() const;
    unsigned int sizeEdges() const;
    Graph operator!() const;
    // Graph operator*() const;
    friend Graph operator+(const Graph first, const Graph second);
    friend Graph operator-(const Graph first, const Graph second);
    friend Graph operator^(const Graph first, const Graph second);
    friend Graph operator*(const Graph first, const Graph second);
    friend std::ostream& operator<<(std::ostream& os, const Graph& graph);

};
Graph operator+(const Graph first, const Graph second);
Graph operator-(const Graph first, const Graph second);
Graph operator^(const Graph first, const Graph second);
Graph operator*(const Graph first, const Graph second);
std::ostream& operator<<(std::ostream& os, const Graph& graph);


#endif //CALCGRAPH_H