#include "CalcGraph.h"
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <iterator>
using std::cout;
using std::endl;


std::set<std::string> vertices;
std::set<std::pair<std::string,std::string> > edges;

Graph::Graph():vertices(),edges(){}

Graph::Graph(std::set<std::string> vertices, std::set<std::pair<std::string,std::string> > edges):
vertices(vertices),edges(edges){}

Graph::~Graph()
{
    vertices.clear();
    edges.clear();
}

Graph& Graph::operator=(const Graph &other_graph)
{
    if (this==&other_graph){ // self =
        return *this;
    }
    vertices.clear();
    edges.clear();
    vertices=other_graph.vertices;
    edges=other_graph.edges;
    return *this;
}

Graph::Graph(const Graph& copy_graph)
{
    vertices=copy_graph.vertices;
    edges=copy_graph.edges;
}

bool Graph::findVertex(std::string vertex) const
{
    return(vertices.find(vertex) != vertices.end());
}

bool Graph::findEdge(std::pair<std::string,std::string> edge) const
{
    return(edges.find(edge) != edges.end());
}

void Graph::addVertex(std::string vertex)
{
    vertices.insert(vertex);
}

void Graph::addEdge(std::pair<std::string,std::string> edge)
{
    if (edge.first==edge.second){
        return;
    }
    if (findVertex(edge.first)&&findVertex(edge.second)){
        edges.insert(edge);
    }
}

unsigned int Graph::sizeVertices() const{
    return vertices.size();
}

unsigned int Graph::sizeEdges() const{
    return edges.size();
}

void Graph::removeVertex(std::string vertex)
{
    vertices.erase(vertex);
    std::set<std::pair<std::string, std::string>> edges_temp(edges);
    for (auto& it : edges_temp){
        if (it.first==vertex || it.second==vertex){
            std::pair<std::string, std::string> temp=it;
            edges.erase(temp);
            //it=edges.first();
        }
    }
    // std::set<std::pair<std::string,std::string> >::iterator check=edges.begin();
    // if (check->first==vertex || check->second==vertex){
    //     edges.erase(*check);
    // }
}

std::set<std::string> Graph::getVertices(){
    return vertices;
}

std::set<std::pair<std::string,std::string> > Graph::getEdges(){
    return edges;
}

std::string Graph::toString() const{
    std::string str="{";
    bool first=true;
    for (auto& it1 : vertices){
        if (first){
            str+=it1;
            first=false;
        }
        else{
            str=str+","+it1;
        }
    }
    str+="|";
    first=true;
    for (auto& it2 : edges){
        if (first){
            str+="<"+it2.first + "," + it2.second+">";
            first=false;
        }
        else{
            str=str+","+"<"+it2.first + "," + it2.second+">";
        }
    }
    
    str+="}";
    return str;
}

Graph operator+(const Graph first, const Graph second)
{
    Graph united(first);
    united.vertices.insert(second.vertices.begin(),second.vertices.end());
    united.edges.insert(second.edges.begin(),second.edges.end());
    return united;
}

Graph operator^(const Graph first, const Graph second)
{
    Graph result;
    for (auto& it : first.vertices){
        if (second.findVertex(it)){
            result.addVertex(it);
        }
    }
    for (auto& it2 : first.edges){
        if (second.findEdge(it2)){
            result.addEdge(it2);
        }
    }
    return result;
}

Graph operator-(const Graph first, const Graph second)
{
    Graph result(first);
    std::set<std::string> temp_vertices(first.vertices);
    for (auto& it :  temp_vertices){
        if (second.findVertex(it))
        {
            result.removeVertex(it);            
        }
    }
    // std::set<std::string>::iterator check=result.vertices.begin();
    // if (second.findVertex(*check)){
    //     result.removeVertex(*check);
    // }
     return result;
}

Graph Graph::operator!() const
{
    Graph result;
    for (auto& copy : vertices){
        result.addVertex(copy);
    }
    for (auto& it1 : vertices){
        for (auto& it2 : vertices){
            if (!findEdge(make_pair(it1,it2))){
                result.addEdge(make_pair(it1,it2));
            }
        }
    }
    return result;
}

// Graph* Graph::operator*() const{

// }

Graph operator*(const Graph first, const Graph second)
{
    Graph result;
    for (auto& it1 : first.vertices){
        for (auto& it2 : second.vertices){
            std::string vertex=std::string("[") + it1 + std::string(";") + it2 + std::string("]");
            result.addVertex(vertex);
        }
    }
    for (auto& it3 : first.edges){
        for (auto& it4 : second.edges){
            std::string left=std::string("[") + it3.first + std::string(";") + it4.first + std::string("]");
            std::string right=std::string("[") + it3.second + std::string(";") + it4.second + std::string("]");
            result.addEdge(make_pair(left,right));
        }
    }
    return result;
}

std::ostream& operator<<(std::ostream& os, const Graph& graph)
    {   
        if (graph.sizeVertices()>0){
            for (auto& it1 : graph.vertices){
                os << it1  << endl;
            }
        }
        os << '$' << endl;
        if (graph.sizeEdges()>0){
            for (auto& it2 : graph.edges){
                os << it2.first << " " << it2.second  << endl;
            }
        }
        return os;
    }

/*
int main(){
    
    Graph test;
    Graph second;
    std::string str="one";
    bool is_in = second.findVertex(str);
    test.addVertex(str);
    test.addVertex("two");
    test.addVertex("one");
    test.addVertex("three");
    test.addEdge(std::make_pair("one","two"));
    test.addEdge(std::make_pair("one","three"));
    test.addEdge(std::make_pair("two","one"));
    test.addEdge(std::make_pair("three","one"));
    test.addEdge(std::make_pair("two","three"));
    test.addEdge(std::make_pair("one",str));
    test.addEdge(std::make_pair("one","one"));
    test.addEdge(std::make_pair("one","thee"));
    is_in = test.findVertex(str);
    test.removeVertex(str);
    second=test;
    second.addVertex("try");
    test.addVertex("nice");
    test.addEdge(std::make_pair("nice","three"));
    Graph uni=test+second;
    Graph duffy=test-second;
    Graph ini=test^second;
    Graph comp=!ini;
    Graph fuli=test*second; 



    return 0;

}*/
