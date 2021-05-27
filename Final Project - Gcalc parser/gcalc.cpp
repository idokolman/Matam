#include <iostream>
#include <string>
#include "CalcGraph.h"
#include "GcalcAux.h"
#include "calculator.h"
#include "Exceptions.h"
#include <algorithm>
#include <fstream>
using std::ifstream;
using std::ofstream;
using std::cout;
using std::endl;

Graph loadFunc(Calculator calc, std::string file_name);
Graph handleGraph(Calculator calc, std::string str);
Graph doCompGraphCalc(Calculator calc, Graph graph1, char op, std::string var2);

std::set<std::pair<std::string,std::string>> makeEdgeSet(std::set<std::string> vertex_set ,std::string edges){
    std::set<std::pair<std::string,std::string>> edge_set;
    edges=cleanSpaces(edges);
    int closers=std::count(edges.begin(),edges.end(),'>');
    int tempeners=std::count(edges.begin(),edges.end(),'<');
    int comas=std::count(edges.begin(),edges.end(),',');
    if (tempeners!=closers){
        throw InvalidEdge();
    }
    if (closers==0){
        if (edges!=""){
            throw InvalidEdge();
        }
    }
    for (int i=0;i<closers;i++){
        edges=cleanSpaces(edges);
        if (comas%2==0){
            throw InvalidEdge();
        }
        int position=edges.find('>');
        std::pair<std::string,std::string> edge=cleanEdge(edges.substr(1,position-1));
        if ((vertex_set.count(edge.first)!=0)&&(vertex_set.count(edge.second)!=0)&&edge.first!=edge.second)
        {
            if (edge_set.count(edge)==1){
                throw InvalidEdge();
            }
            edge_set.insert(edge);
        }
        else{
            throw InvalidEdge();
        }
        edges=edges.substr(position+1,edges.length());
        edges=cleanSpaces(edges);
        if (edges.front()==','){
            edges=edges.substr(1,edges.length());
        }
    }
    return edge_set;
}

std::set<std::string> makeVertexSet(std::string vertices){
    std::set<std::string> vertex_set;
    vertices=cleanSpaces(vertices);
    int comas=std::count(vertices.begin(),vertices.end(),',');
    if (comas==0){
        vertices=cleanVertex(vertices);
        if (vertices!=""){
            vertex_set.insert(vertices);
        }
    }
    else {
        for (int i=0;i<comas+1;i++){
            int position=vertices.find(',');
            std::string vertex=cleanVertex(vertices.substr(0,position));
            if (vertex_set.count(vertex)==1||vertex==""){
                throw InvalidVertex();
            }
            vertex_set.insert(vertex);
            vertices=vertices.substr(position+1,vertices.length());
        }
    }
    return vertex_set;
}

Graph makeNewGraph(Calculator calc, std::string str){
    int ender=str.find('}');
    int middle=str.find('|');
    std::string vertices;
    std::set<std::string> vertex_set;
    std::set<std::pair<std::string,std::string>> edge_set;
    if (middle==-1||middle>ender){
        vertices=str.substr(1,ender-1);
        vertex_set=makeVertexSet(vertices);
    }
    else if (middle<ender){
        vertices=str.substr(1,middle-1);
        vertex_set=makeVertexSet(vertices);
        std::string edges=str.substr(middle+1,ender-middle-1);
        edge_set=makeEdgeSet(vertex_set, edges);
    }
    Graph result(vertex_set,edge_set);
    for (unsigned int i=ender+1;i<str.length();i++){
        if (str[i]=='+'||str[i]=='^'||str[i]=='*'||str[i]=='-'){
            //result=doCompGraphCalc(calc,result,str[i],str.substr(i+1,str.length()));
            result=doCompGraphCalc(calc,result,str[i],str.substr(i+1,str.length()));
            break;
        }
        else if (str[i]==' '||iscntrl(str[i])){
            continue;}
        else {
            throw InvalidGraph();
        }
    }
    return result;
}

Graph doCompGraphCalc(Calculator calc, Graph graph1, char op, std::string var2)
{
    Graph result,graph2;
    bool allalnum=true;
    std::string new_var="",temp="",temp2;
    char new_op;
    var2=cleanSpaces(var2);
    for (unsigned int i=0;i<var2.length();i++){
        if (var2[i]=='+'||var2[i]=='^'||var2[i]=='*'||var2[i]=='-'){
                    temp2=var2.substr(i+1,var2.length());
                    new_op=var2[i];
                    var2=var2.substr(0,i);
                    graph1=doCompGraphCalc(calc,graph1,op,var2);
                    result=doCompGraphCalc(calc,graph1,new_op,temp2);
                    return result;
                }
    }
    if (var2.front()=='{'||var2.front()=='!'||var2.substr(0,4)=="load"||var2.front()=='('){
        graph2=handleGraph(calc,var2);
    }
    else
    {   
        for (unsigned int i=0;i<var2.length();i++){
            if (!isalnum(var2[i])){
                new_var=var2.substr(0,i);
                temp=var2.substr(i,var2.length());
                temp=cleanSpaces(temp);
                allalnum=false;
                break;
            }
        }
        if (!allalnum){
            var2=new_var;
        }
        if(calc.empty()||(!calc.count(var2))){
            throw InvalidInput();
        }
        graph2=calc.at(var2);
    }
    switch (op){
        case '+':
        result=graph1+graph2;
        break;
        case '*':
        result=graph1*graph2;
        break;
        case '-':
        result=graph1-graph2;
        break;
        case '^':
        result=graph1^graph2;
        break;
        }
    if (!allalnum){
        char oper=temp.front();
        temp=temp.substr(1,temp.length());
        result=doCompGraphCalc(calc,result,oper,temp);
    }
    return result;
}

Graph handleGraph(Calculator calc, std::string str){
    Graph result;
    str=cleanSpaces(str);
    if (!checkBrackets(str)){
        throw IllegalName();
    }
    int first=str.find('('),last=str.find(')');
    if (str.substr(0,4)==("load")&&first>3&&last>0)
    {   
        std::string start=str.substr(4,first-4);
        std::string file_name=str.substr(first,last-first+1);
        std::string rest=str.substr(last+1);
        file_name=cleanSpaces(file_name);
        Graph temp;
        temp=loadFunc(calc, file_name);
        str=start+temp.toString()+rest;
    }
    //int load_counter=std::count(str.begin(),str.end(),"load");
    int str_length=str.length();
    for(int i=0;i<str_length;i++){
        if(str.substr(i,4)=="load"){
            for(int j=i+4;j<str_length;j++){
                if(str[j]==' '){
                    continue;
                }
                if(str[j]=='('){
                    std::string start=str.substr(0,i);
                    std::string load_part=str.substr(i,str.length());
                    std::string end=load_part.substr(load_part.find(')')+1);
                    load_part=load_part.substr(0,load_part.find(')')+1);
                    load_part=cleanSpaces(load_part);
                    Graph temp;
                    temp=handleGraph(calc, load_part);
                    load_part=temp.toString();
                    str=start+load_part+end;
                    str_length=str.length();
                    i=0;
                    break;
                }
                if(str[j]!=('(')){
                    break;
                }
            }
        }
    }
    int loader=str.find("load");
    bool load_name=false;
    while (loader>0){

        std::string start=str.substr(0,loader);
        std::string load_part=str.substr(loader,str.length());
        std::string end=load_part.substr(load_part.find(')')+1);
        load_part=load_part.substr(0,load_part.find(')')+1);
        load_part=cleanSpaces(load_part);
        bool double_break=false;
        first=load_part.find('(');
        last=load_part.find(')');
        if (first>0&&last>0){
            for (int j=4;j<first;j++)
                if(load_part[j]!=' '){
                    double_break=true;
                    break;
                }
        }
        else{
            load_name=true;
            break;
        }
        if (double_break){
            load_name=true;
            break;
        }
        Graph temp;
        temp=handleGraph(calc, load_part);
        load_part=temp.toString();
        str=start+load_part+end;
        loader=str.find("load");
    }
    int opener=str.find('(');
    int openers=0,closers=0;
    if (opener>-1&&(opener<loader||loader==-1||load_name)){
        for (unsigned int i=0;i<str.length();i++){
            if(str[i]=='('){
                openers++;
            }
            if(str[i]==')'){
                closers++;
            }
            if(openers==closers&&openers>0){
                std::string var1=str.substr(0,str.find('('));
                std::string brackets=str.substr(str.find('(')+1,i-str.find('(')-1);
                std::string var2=str.substr(i+1,str.length());
                Graph temp=handleGraph(calc, brackets);
                std::string middle=temp.toString();
                str=var1+middle+var2;
                temp=handleGraph(calc,str);
                str=temp.toString();
                openers=0;
                closers=0;
                i=0;
            }
        }
    }
    str=cleanSpaces(str);
    if (str.front()=='{'){
        result=makeNewGraph(calc, str);
        return result;
    }
    else if (str.front()=='!'){
        bool allaslnum=true;
        str=str.substr(1,str.length());
        str=cleanSpaces(str);
        if (str.front()=='{'){
            std::string cons_graph=str.substr(0,str.find('}')+1);
            Graph temp=!handleGraph(calc,cons_graph);
            str=temp.toString()+str.substr(str.find('}')+1,str.length());
            result=handleGraph(calc,str);
            return result;
        }
        if (str.front()=='!'){
            int mark=1,where=0;
            for (unsigned int i=0;i<str.length();i++){
                if (str[i]=='+'||str[i]=='^'||str[i]=='*'||str[i]=='-'){
                    std::string start=str.substr(0,i);
                    std::string end=str.substr(i);
                    Graph temp;
                    if (mark%2==1){
                        start=start.substr(where+1);
                        temp=!handleGraph(calc,start);
                        str=temp.toString()+end;
                    }
                    else{
                        str=str.substr(where+1);
                    }
                    //str=temp.toString()+end;
                    result=handleGraph(calc,str);
                    return result;
                }
                if (str[i]=='!'){
                    where=i;
                    mark++;
                }
            }
            str=str.substr(where+1);
            if (mark%2==1){
                result=!handleGraph(calc,str);
                return result;
            }
            else{
                result=handleGraph(calc,str);
                return result;
            }
            // result=!handleGraph(calc,str);
            // return result;
        }
        for (unsigned int i=1;i<str.length();i++){
            if (!isalnum(str[i])){
                allaslnum=false;
                std::string name=str.substr(0,i);
                name=cleanName(name);
                if (calc.count(name)==0){
                    throw InvalidGraph();
                }
                result=!calc.at(name);
                if (str[i]=='+'||str[i]=='^'||str[i]=='*'||str[i]=='-'){
                    result=doCompGraphCalc(calc,result,str[i],str.substr(i+1,str.length()));
                    break;
                }
                else if (str[i]==' '||iscntrl(str[i])){
                    continue;}
                else {
                    throw InvalidGraph();
                }
            }
        }
        if (allaslnum){
            if (calc.count(str)==0){
                throw InvalidGraph();
            }
            result=!calc.at(str);
        }
        return result;
    }
    else if(isalpha(str.front())){
        std::string var1;
        std::string temp;
        bool allalnum=true;
        for (unsigned int i=0;i<str.length();i++)
        {
            if (!isalnum(str[i]))
            {
                var1=str.substr(0,i);
                temp=str.substr(i,str.length());
                temp=cleanSpaces(temp);
                allalnum=false;
                break;
            }
        }
        if (allalnum){
            if (calc.count(str)==0){
                throw InvalidGraph();
            }
            return calc.at(str);
        }
        if (calc.count(var1)==0){
            throw InvalidGraph();
        }
        char oper=temp.front();
        std::string var2=temp.substr(1,temp.length());
        var2=cleanSpaces(var2);
        if (oper=='+'||oper=='-'||oper=='*'||oper=='^'){
            result=doCompGraphCalc(calc,calc.at(var1),oper,var2);
            return result;
        }
        else
        {
            throw InvalidInput();
        }
        
    }
    else {
        throw InvalidInput();}
    return result;
}

void printGraph(std::ostream& to, Calculator calc, std::string variable)
{
    variable=cleanForFunc(variable);
    if (calc.count(variable)==1){
        to<< calc.find(variable)->second;
    }
    else{
        Graph result=handleGraph(calc,variable);
        to<<result;
    }
    
}

Calculator deleteGraph(Calculator calc, std::string variable)
{
    variable=cleanForFunc(variable);
    variable=cleanName(variable);
    if (calc.find(variable)!=calc.end()){
        calc.erase(variable);
    }
    else{
        throw InvalidGraph();
    }
    return calc;
}

Graph loadFunc(Calculator calc, std::string file_name)
{
    file_name=cleanForFunc(file_name);
    Graph result;
    file_name=cleanSpaces(file_name);
    ifstream from(file_name);
        if (!from) {
            throw NoFile();
        }
    unsigned int length[2];
    unsigned int vertex_length;
    char vertex_char;
    std::string vertices="";
    std::string edges="";
    for(unsigned int i = 0; i < 2; i++){
        if (from.peek()==EOF){
            throw LoadProblem();}
        from.read((char*)&length[i],sizeof(unsigned int));
    }
    for(unsigned int i=0;i<length[0];i++){
        if (from.peek()==EOF){
            throw LoadProblem();}
        from.read((char*)&vertex_length,sizeof(unsigned int));
        for (unsigned int j=0;j<vertex_length;j++){
            if (from.peek()==EOF){
                throw LoadProblem();}
            from.read((char*)&vertex_char,sizeof(char));
            vertices+=vertex_char;
        }
        vertices+=",";
    }
    vertices=vertices.substr(0,vertices.length()-1);

    for(unsigned int i=0;i<length[1]*2;i++){
        if (i%2==0){
            edges+="<";
        }
        if (from.peek()==EOF){
            throw LoadProblem();}
        from.read((char*)&vertex_length,sizeof(unsigned int));
        for (unsigned int j=0;j<vertex_length;j++){
            if (from.peek()==EOF){
                throw LoadProblem();}
            from.read((char*)&vertex_char,sizeof(char));
            edges+=vertex_char;
        }
        if (i%2==0){
        edges+=",";}
        else{
            edges+=">,";
        }
    }
    if (from.peek()!=EOF){
        throw LoadProblem();}
    edges=edges.substr(0,edges.length()-1);
    std::string full_graph = "{"+vertices+"|"+edges+"}";
    result=handleGraph(calc,full_graph);
    return result;
}

void printBinary(ofstream& to, Graph graph)
{
    unsigned int num_of_vertices=graph.sizeVertices();
    unsigned int num_of_edges=graph.sizeEdges();
    unsigned int vertex_length;
    std::string temp;
    to.write((char*)&num_of_vertices,sizeof(unsigned int));
    to.write((char*)&num_of_edges,sizeof(unsigned int));
    std::set<std::string> vertices=graph.getVertices();
    for (auto& it1 : vertices){
        temp=it1;
        vertex_length=temp.length();
        to.write((char*)&vertex_length,sizeof(unsigned int));
        for (unsigned int i=0;i<vertex_length;i++){
            to.write((char*)&temp[i],sizeof(char));
        }
    }
    std::set<std::pair<std::string,std::string>> edges=graph.getEdges();
    for (auto& it2 : edges){
        temp=it2.first;
        vertex_length=temp.length();
        to.write((char*)&vertex_length,sizeof(unsigned int));
        for (unsigned int i=0;i<vertex_length;i++)
            to.write((char*)&temp[i],sizeof(char));
        temp=it2.second;
        vertex_length=temp.length();
        to.write((char*)&vertex_length,sizeof(unsigned int));
        for (unsigned int i=0;i<vertex_length;i++)
            to.write((char*)&temp[i],sizeof(char));
    }
}

void saveFunc(Calculator calc, std::string str){
    str=cleanForFunc(str);
    int comma=str.find_last_of(',');
    if (comma==-1){
        throw SaveProblem();
    }
    std::string variable = str.substr(0,comma);
    std::string file_name = str.substr(comma+1,str.length());
    file_name=cleanSpaces(file_name);
    int opener=file_name.find('('), closer=file_name.find(')');
    if (file_name==""||opener!=-1||closer!=-1){
        throw IllegalName();
    }
    Graph result=handleGraph(calc,variable);
    ofstream outfile (file_name, std::ios::out |std::ios_base::binary);
    if (!outfile){
        throw NoFile();
    }
    if (calc.count(variable)==1){
        printBinary(outfile,calc.at(variable));
    }
    else{
        printBinary(outfile,result);
    }
}

void run(std::istream& from, std::ostream& to){
    try{
        std::string line;
        Calculator calc;
        printGcalc(to);
        while (std::getline(from, line))
        {
            line=cleanSpaces(line);
            if (line=="quit"){
                break;
            }
            std::string name="";
            Graph graphi;
            int equal=line.find('=');
            bool should_save=false,should_print=false;
            if(line.substr(0,4)==("save"))
            {
                int line_length=line.length();
                for(int j=4;j<line_length;j++)
                {
                    if(line[j]==' '){
                        continue;
                    }
                    if(line[j]=='('){
                        should_save=true;
                        break;
                    }
                    if(line[j]!=('(')){
                        should_save=false;
                        break;
                    } 
                }
                if (should_save){
                    std::string str=line.substr(4,line.length());
                    try
                    {
                        saveFunc(calc, str);
                    }
                    catch(const Exception& error){
                        to << error.what() << endl;
                        printGcalc(to);
                        continue;
                    }
                }
            }
            if(line.substr(0,5)==("print"))
            {
                int line_length=line.length();
                for(int j=5;j<line_length;j++)
                {
                    if(line[j]==' '){
                        continue;
                    }
                    if(line[j]=='('){
                        should_print=true;
                        break;
                    }
                    if(line[j]!=('(')){
                        should_print=false;
                        break;
                    } 
                }
                if (should_print){
                    std::string variable=line.substr(5,line.length());
                    try
                    {
                        printGraph(to, calc, variable);
                    }
                    catch(const Exception& error){
                        to << error.what() << endl;
                        printGcalc(to);
                        continue;
                    }
                }
            }
            if (should_save||should_print){
                printGcalc(to);
                continue;
            }
            if (equal!=-1){
                try{
                name=handleVariable(line);
                }
                catch(const Exception& error){
                    to << error.what() << endl;
                    printGcalc(to);
                    continue;
                }
                std::string rest = line.substr(equal+1,line.length());
                try{
                graphi=handleGraph(calc, rest);
                }
                catch (const Exception& error){
                    to<< error.what()<<endl;
                    printGcalc(to);
                    continue;
                }
                if (calc.count(name)==1){
                    calc.erase(name);
                }
                calc.insert(make_pair(name, graphi));
            }
            else if(line==""){
                printGcalc(to);
                continue;
            }
            else if(line.substr(0,6)==("delete"))
            {
                std::string variable=line.substr(6,line.length());
                try
                {
                    calc=deleteGraph(calc, variable);
                }
                catch(const Exception& error){
                    to << error.what() << endl;
                    printGcalc(to);
                    continue;
                }
            }
            else if(line==("reset")){
                calc.clear();
            }
            else if(line==("who")){
                for (auto graph : calc){
                    to << graph.first << endl;
                }
            }
            // else if(line.substr(0,4)==("load"))
            // {
            //     std::string str=line.substr(4,line.length());
            //     try
            //     {
            //         loadFunc(calc, str);
            //     }
            //     catch(const Exception& error){
            //         to << error.what() << endl;
            //         printGcalc(to);
            //         continue;
            //     }
            // }
            else if(!should_print&&!should_save){
                to << InvalidInput().what() << endl;
            }
            printGcalc(to);
        }
        return;
    }
    catch(const std::bad_alloc& error){
        std::cerr << error.what() << endl;
        return;
    }
}

int main(int argc, char** argv){
    if (argc==3)
    {    
        ifstream from(argv[1]);
        if (!from) {
            std::cerr << NoFile().what() << argv[1] << endl;
            return 0;	}
        ofstream to(argv[2]);
        if (!to) {
            std::cerr << NoFile().what() << argv[2] << endl;
            return 0;	
        }
        run(from,to);
        return 0;
    }
    else if (argc==1){
        run(std::cin, std::cout);
        return 0;
    }
    else{
        std::cerr<<IllegalArgument().what()<<endl;
    }
    return 0;
}