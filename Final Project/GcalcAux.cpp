#include <iostream>
#include <string>
#include "Exceptions.h"
#include "GcalcAux.h"
#include <algorithm>
#include <fstream>
using std::ifstream;
using std::ofstream;
using std::cout;
using std::endl;

std::string cleanSpaces(std::string str){
    if (iscntrl(str.back())){
        str=str.substr(0,str.length()-1);
    }
    while (isblank(str.front())){
        str=str.substr(1,str.length());
    }
    while (isblank(str.back())){
        str=str.substr(0,str.length()-1);
    }
    return str;
}

std::string cleanVertex(std::string str)
{
    str=cleanSpaces(str);
    int openers=0,closers=0;
    for (unsigned int i=0;i<str.length();i++){
        if (str[i]=='['){
            openers++;
        }
        if (str[i]==']'){
            closers++;
        }
        if (closers>openers){
            throw InvalidVertex();
        }
        if (str[i]==';'&&(openers==closers)){
            throw InvalidVertex();
        }
        if (!isalnum(str[i])&&str[i]!='['&&str[i]!=']'&&str[i]!=';'){
            throw InvalidVertex();
        }
    }
    if (openers!=closers){
        throw InvalidVertex();
    }
    return str;
}


std::string cleanVertexForPython(std::string str)
{
    int openers=0,closers=0;
    for (unsigned int i=0;i<str.length();i++){
        if (str[i]=='['){
            openers++;
        }
        if (str[i]==']'){
            closers++;
        }
        if (closers>openers){
            throw InvalidVertex();
        }
        if (str[i]==';'&&(openers==closers)){
            throw InvalidVertex();
        }
        if (!isalnum(str[i])&&str[i]!='['&&str[i]!=']'&&str[i]!=';'){
            throw InvalidVertex();
        }
    }
    if (openers!=closers){
        throw InvalidVertex();
    }
    return str;
}

bool checkBrackets(std::string str)
{
    int openers=0,closers=0;
    for (unsigned int i=0;i<str.length();i++){
        if (str[i]=='('){
            openers++;
        }
        if (str[i]==')'){
            closers++;
        }
        if (closers>openers){
            return false;
        }
    }
    if (openers!=closers){
        return false;
    }
    return true;
}

std::pair<std::string,std::string> cleanEdge(std::string str)
{
    str=cleanSpaces(str);
    std::pair<std::string,std::string> edge;
    if(std::count(str.begin(),str.end(),',')!=1){
        throw InvalidEdge();
    }
    int coma=str.find(',');
    std::string vertex1 = str.substr(0,coma);
    vertex1=cleanVertex(vertex1);
    std::string vertex2 = str.substr(coma+1,str.length());
    vertex2=cleanVertex(vertex2);
    edge.first=vertex1;
    edge.second=vertex2;
    return edge;
}

std::string cleanName(std::string name)
{
    name=cleanSpaces(name);
    int space=name.find(' ');
    if (space!=-1)
    {
        for (unsigned int i=space;i<name.length();i++)
        {
            if (!isblank(name[i])||!iscntrl(name[i]))
            {
                throw IllegalName();
            }
        }
        name=name.substr(0,space);
    }
    if (!isalpha(name.front())){
        throw IllegalName();
    }
    for (unsigned int i=0;i<name.length();i++){
        if (!isalnum(name[i])){
            throw IllegalName();
        }
    }
    return name;
}

std::string handleVariable(std::string line)
{
    int equal=line.find('=');
    std::string name = line.substr(0, equal);
    name=cleanName(name);
    if (name=="print"||name=="who"||name=="reset"||name=="print"||name=="delete"||name=="save"||name=="load"){
        throw IllegalName();
    }
    return name;
}

std::string cleanForFunc(std::string variable)
{
    variable=cleanSpaces(variable);
    if (variable.front()!='('||variable.back()!=')'){
        throw IllegalName();
    }
    variable=variable.substr(1,variable.length()-2);
    return variable;
}

void printGcalc(std::ostream& to){
    if (&to==&std::cout){
        cout << "Gcalc> ";
    }
}
