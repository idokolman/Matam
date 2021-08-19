#include "calculator.h"
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <iterator>
using std::cout;
using std::endl;

std::map<std::string,Graph> Calculator::getMap(){
    return data;
}

int Calculator::count(std::string str){
    return data.count(str);
}

std::map<std::string,Graph>::iterator Calculator::find(std::string str){
    return data.find(str);
}

std::map<std::string,Graph>::iterator Calculator::begin(){
    return data.begin();
}

std::map<std::string,Graph>::iterator Calculator::end(){
    return data.end();
}

void Calculator::clear(){
    data.clear();
}

void Calculator::erase(std::string key){
    data.erase(key);
}

bool Calculator::empty(){
    return data.empty();
}

Graph Calculator::at(std::string key){
    return data.at(key);
}

void Calculator::insert(std::pair<std::string,Graph> pair){
    data.insert(pair);
}

