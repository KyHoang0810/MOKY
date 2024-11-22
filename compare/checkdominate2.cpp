#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>
#include <ctime>
#include<sstream>
#include <string>
#include <unistd.h>
using namespace std;
string inputfile="checkdom.txt";
struct Individual{
    double fitness1;
    double fitness2;
};

vector<Individual> pareto;
int checkdomination(Individual dominater,Individual dominated){
    bool ch = false;
    if(dominater.fitness1-dominated.fitness1>1e-9) 
        return false;
    else if(dominated.fitness1-dominater.fitness1>1e-3) ch=true;
    if(dominater.fitness2-dominated.fitness2>1e-3) return false;
    else if(dominated.fitness2-dominater.fitness2>1e-3) ch=true;
    return ch;
}
bool dominatedinpareto(Individual indi){
    if(pareto.size()==0)return false;
    for(int i=0;i<pareto.size();i++){
        if(checkdomination(pareto[i],indi))return true;
    }
    return false;
}
bool comparefit1(Individual  &indi1,Individual &indi2){
    return indi1.fitness1<indi2.fitness1;
}
void updatepareto(Individual indi){
    vector<int> dominated;
    if(!dominatedinpareto(indi)){
        for(int i=0;i<pareto.size();i++){
            if(checkdomination(indi,pareto[i]))dominated.push_back(i);
        }
        sort(dominated.begin(),dominated.end());
        for(int i=0;i<dominated.size();i++){
            pareto.erase(pareto.begin()+dominated[i]-i);

        }
        pareto.push_back(indi);
    }
    
}
bool inpareto(Individual indi){
    for(int i=0;i<pareto.size();i++){
        if(abs(indi.fitness1-pareto[i].fitness1)<1e-4&&abs(indi.fitness2-pareto[i].fitness2)<1e-4) return true;

    }
    return false;
}
std::vector<std::string> splitString(const std::string& str, char delimiter) {
    std::vector<std::string> parts;
    std::stringstream ss(str);
    std::string part;
    
    while (std::getline(ss, part, delimiter)) {
        parts.push_back(part);
    }
    
    return parts;
}
int main(){
    ifstream inputFile(inputfile);
    string line;
    int dominatedcount=0;
    int dominatecount=0;
    int neutral=0;
    getline(inputFile,line);
    vector<string> sols=splitString(line,';');
    for(int i=0;i<sols.size();i++){
        Individual indi;
        
        
        vector<string>objectives=splitString(sols[i],',');
        if(i==0) indi.fitness1=stod(objectives[0].substr(1));
        else indi.fitness1=stod(objectives[0].substr(2));
        istringstream iss(objectives[1]);
        float value;
        while (iss >> value){
            indi.fitness2=value;
        }
        
        pareto.push_back(indi);
    }
    getline(inputFile,line);
    vector<string> solss=splitString(line,';');
    for(int i=0;i<solss.size();i++){
        Individual indi;
        vector<string>objectives=splitString(solss[i],',');
        if(i==0) indi.fitness1=stod(objectives[0].substr(1));
        else indi.fitness1=stod(objectives[0].substr(2));
        istringstream iss(objectives[1]);
        float value;
        while (iss >> value){
            indi.fitness2=value;
        }
        if(dominatedinpareto(indi))dominatedcount++;
        else{
            int cou=0;
            for(int j=0;j<pareto.size();j++){
                if(checkdomination(indi,pareto[j]))cou++;
            }
            if(cou>0)dominatecount++;
            else neutral++;
        }
        
    }
    cout<<dominatecount<<" "<<dominatedcount<<" "<<neutral<<endl;;


}