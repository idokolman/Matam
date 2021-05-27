#ifndef CALCULATOR_H
#define CALCULATOR_H
#include "CalcGraph.h"
#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <iterator>
#include <set>
class Calculator{

    private:

    std::map<std::string,Graph> data;

    public:

    std::map<std::string,Graph> getMap();
    int count(std::string);
    std::map<std::string,Graph>::iterator find(std::string str);
    std::map<std::string,Graph>::iterator begin();
    std::map<std::string,Graph>::iterator end();
    void clear();
    void erase(std::string key);
    void insert(std::pair<std::string,Graph> pair);
    bool empty();
    Graph at(std::string key);
};
#endif //CALCULATOR_H