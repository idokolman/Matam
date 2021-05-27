#ifndef GCALC_AUX_H
#define GCALC_AUX_H
#include <iostream>
#include <string>
#include "CalcGraph.h"
#include "calculator.h"
#include "Exceptions.h"
#include <algorithm>
#include <fstream>
using std::ifstream;
using std::ofstream;
using std::cout;
using std::endl;

std::string cleanSpaces(std::string str);

std::string cleanVertex(std::string str);

std::string cleanVertexForPython(std::string str);

bool checkBrackets(std::string str);

std::pair<std::string,std::string> cleanEdge(std::string str);

std::string cleanName(std::string name);

std::string handleVariable(std::string line);

std::string cleanForFunc(std::string variable);

void printGcalc(std::ostream& to);

#endif //GCALC_AUX_H