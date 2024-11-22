#ifndef PARETOUPDATE_H
#define PARETOUPDATE_H
#include "dominationCheck.h"
#include <algorithm>
using namespace std;
bool dominatedinpareto(Individual indi,vector<Individual> pareto){
    if(pareto.size()==0)return false;
    for(int i=0;i<pareto.size();i++){
        if(checkdomination(pareto[i],indi))return true;
    }
    return false;
}
bool dominatedinTabupareto(Individual indi,vector<Individual>Tabupareto){
    if(Tabupareto.size()==0)return false;
    for(int i=0;i<Tabupareto.size();i++){
        if(checkdomination(Tabupareto[i],indi))return true;
    }
    return false;
}
void updatepareto(Individual indi,vector<Individual> &pareto){
    vector<int> dominated;
    if(!dominatedinpareto(indi,pareto)){
        for(int i=0;i<pareto.size();i++){
            if(checkdomination(indi,pareto[i]))dominated.push_back(i);
        }
        sort(dominated.begin(),dominated.end());
        for(int i=0;i<dominated.size();i++){
            pareto.erase(pareto.begin()+dominated[i]-i);

        }
        pareto.push_back(indi);
    }
    /*
    paretofit1.clear();
    for(int i=0;i<pareto.size();i++){
        paretofit1.push_back(pareto[i].fitness1);
    }
    paretofit2.clear();
    for(int i=0;i<pareto.size();i++){
        paretofit2.push_back(pareto[i].fitness2);
    }
    */
    
    
}
void updateTabupareto(Individual indi,vector<Individual> Tabupareto){
    vector<int> dominated;
    if(!dominatedinTabupareto(indi,Tabupareto)){
        for(int i=0;i<Tabupareto.size();i++){
            if(checkdomination(indi,Tabupareto[i]))dominated.push_back(i);
        }
        sort(dominated.begin(),dominated.end());
        for(int i=0;i<dominated.size();i++){
            Tabupareto.erase(Tabupareto.begin()+dominated[i]-i);

        }
        Tabupareto.push_back(indi);
    }
    
    
}
bool inpareto(Individual indi,vector<Individual> pareto){
    for(int i=0;i<pareto.size();i++){
        if(abs(indi.fitness1-pareto[i].fitness1)<1e-4&&abs(indi.fitness2-pareto[i].fitness2)<1e-4) return true;

    }
    return false;
}
bool inTabupareto(Individual indi,vector<Individual> Tabupareto){
    for(int i=0;i<Tabupareto.size();i++){
        if(abs(indi.fitness1-Tabupareto[i].fitness1)<1e-4&&abs(indi.fitness2-Tabupareto[i].fitness2)<1e-4) return true;

    }
    return false;
}
#endif