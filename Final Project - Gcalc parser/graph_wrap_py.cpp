#include "CalcGraph.h"
#include "GcalcAux.h"
#include "graph_wrap_py.h"
#include "Exceptions.h"
#include <string>
#include <iostream>
#include <string.h>

/*
static char* prepVertex(char* vertex){
    if (strlen(vertex)==0){
        throw InvalidInput();
    }
    while (vertex[0]==' '){
        memmove(vertex, vertex+1,strlen(vertex));
    }
    if (strlen(vertex)==0){
        throw InvalidInput();
    }
    while (vertex[strlen(vertex)-1]==' '){
        vertex[strlen(vertex)-1]='\0';
    }
    int openers=0,closers=0;
    for (unsigned int i=0;i<strlen(vertex);i++){
        if (vertex[i]=='['){
            openers++;
        }
        if (vertex[i]==']'){
            closers++;
        }
        if (closers>openers){
            throw InvalidInput();
        }
        if (vertex[i]==';'&&(openers==closers)){
            throw InvalidInput();
        }
        if (!isalnum(vertex[i])&&vertex[i]!='['&&vertex[i]!=']'&&vertex[i]!=';'){
            throw InvalidInput();
        }
    }
    if (openers!=closers){
        throw InvalidInput();
    }
    if (!strcmp(vertex,"print")||!strcmp(vertex,"who")||!strcmp(vertex,"reset")||!strcmp(vertex,"print")||!strcmp(vertex,"delete")||!strcmp(vertex,"save")||!strcmp(vertex,"load")){
        throw InvalidInput();
    } 

    return vertex;
}
*/

Graph* create(){
    try{
        return new Graph();
    }
    catch (const std::bad_alloc& error){
        std::cerr << error.what() << endl;
        return nullptr;
    }
}

void destroy(Graph* graph){
    if (graph==nullptr){
        std::cout << "Error: graph entered is nullptr" << endl;
        return;
    }
    delete graph;
}

Graph* addVertex(Graph* graph, char* v){
    if (graph==nullptr){
        std::cout << "Error: graph entered is nullptr" << endl;
        return nullptr;
    }
    std::string str_v=v;
    try{
        str_v=cleanVertexForPython(str_v);
        if(graph->findVertex(str_v)){
            throw AlreadyExist();
        }
        graph->addVertex(str_v);
    }
    catch(const Exception& error){
        std::cout << error.what() << endl;
        return nullptr;
    }
    return graph;
}

/*
Graph* addEdge(Graph* graph, char* v1, char* v2){
    // std::string v1=char_v1;
    // std::string v2=char_v2;
    try{
        v1=prepVertex(v1);
        v2=prepVertex(v2);
        if(graph->findVertex(v1)&&graph->findVertex(v2)){
            if(graph->findEdge(std::pair<char*,char*>(v1,v2))){
                throw AlreadyExist();
            }
        }
        else{
            throw InvalidInput();
        }
        graph->addEdge(std::pair<char*,char*>(v1,v2));
    }
    catch(const Exception& error){
        std::cout << error.what() << endl;
        return nullptr;
    }
    return graph;
}
*/

Graph* addEdge(Graph* graph, char* v1, char* v2){
    if (graph==nullptr){
        std::cout << "Error: graph entered is nullptr" << endl;
        return nullptr;
    }
    std::string str_v1=v1;
    std::string str_v2=v2;
    try{
        str_v1=cleanVertexForPython(str_v1);
        str_v2=cleanVertexForPython(str_v2);
        if(graph->findVertex(str_v1)&&graph->findVertex(str_v2)){
            if(graph->findEdge(std::pair<std::string,std::string>(str_v1,str_v2))){
                throw AlreadyExist();
            }
        }
        else{
            throw InvalidEdge();
        }
        if (str_v1==str_v2){
            throw InvalidEdge();
        }
        graph->addEdge(std::pair<std::string,std::string>(str_v1,str_v2));
    }
    catch(const Exception& error){
        std::cout << error.what() << endl;
        return nullptr;
    }
    return graph;
}

void disp(Graph* graph){
    if (graph==nullptr){
        std::cout << "Error: graph entered is nullptr" << endl;
        return;
    }
    Graph temp=*graph;  
    std::cout<< temp;
}

Graph* graphUnion(Graph* graph_in1, Graph* graph_in2, Graph* graph_out){
    if (graph_in1==nullptr||graph_in2==nullptr||graph_out==nullptr){
        std::cout << "Error: graph entered is nullptr" << endl;
        return nullptr;
    }
    Graph temp(*graph_in1+*graph_in2);
    *graph_out=(temp);
    return graph_out;
}

Graph* graphIntersection(Graph* graph_in1, Graph* graph_in2, Graph* graph_out){
    if (graph_in1==nullptr||graph_in2==nullptr||graph_out==nullptr){
        std::cout << "Error: graph entered is nullptr" << endl;
        return nullptr;
    }
    if(graph_in1==graph_in2){
        *graph_out=*graph_in1;
        return graph_out;
    }
    Graph temp=*graph_in1^*graph_in2;
    *graph_out=(temp);
    return graph_out;
}

Graph* graphDifference(Graph* graph_in1, Graph* graph_in2, Graph* graph_out){
    if (graph_in1==nullptr||graph_in2==nullptr||graph_out==nullptr){
        std::cout << "Error: graph entered is nullptr" << endl;
        return nullptr;
    }
    if(graph_in1==graph_in2){
        Graph empty;
        *graph_out=empty;
        return graph_out;
    }
    Graph temp1=*graph_in1;
    Graph temp2=*graph_in2;
    Graph temp=temp1-temp2;
    *graph_out=(temp);
    return graph_out;
}

Graph* graphProduct(Graph* graph_in1, Graph* graph_in2, Graph* graph_out){
    if (graph_in1==nullptr||graph_in2==nullptr||graph_out==nullptr){
        std::cout << "Error: graph entered is nullptr" << endl;
        return nullptr;
    }
    Graph temp((*graph_in1)*(*graph_in2));
    *graph_out=(temp);
    return graph_out;
}
Graph* graphComplement(Graph* graph_in, Graph* graph_out){
    if (graph_in==nullptr){
        std::cout << "Error: graph entered is nullptr" << endl;
        return nullptr;
    }
    Graph temp=*graph_in;
    temp=!temp;
    *graph_out=temp;
    return graph_out;
}
/*
int main(){
    Graph* test=create(),*temp2=create();
    char* str1=(char*)("wow");
    char* str2=(char*)"nice";
    test=addVertex(test,str1);
    temp2=addVertex(temp2,str2);
    test=graphUnion(test,temp2,test);
    disp(test);
    test=graphUnion(test,test,test);
    disp(test);
    temp2=graphUnion(temp2,temp2,temp2);
    disp(test);

    
}*/
