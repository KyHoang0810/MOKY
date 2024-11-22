#include "compare.h"
#include "parameter.h"
#include "dominationCheck.h"
#include <algorithm>
using namespace std;
extern double maxobj1;
extern double maxobj2;
extern double minobj1;
extern double minobj2;
extern double obj1norm;
extern double obj2norm;
//non dominated sort
vector<vector<int>> fast_non_dominated_sort(vector<Individual> &population){
    int popsize=population.size();
    vector<vector<int>> front(popsize);
    vector<int> dominationcount(popsize);
    vector<vector<int>> dominatescount(popsize);
    dominatescount.clear();
    vector<int> check;
    check.assign(popsize,-1);
    for(int i=0;i<popsize;i++){
        for(int j=i+1;j<popsize;j++){
            if(checkdomination(population[i],population[j])) {
                dominatescount[i].push_back(j);
                dominationcount[j]++;
            }
            if(checkdomination(population[j],population[i])) {
                dominatescount[j].push_back(i);
                dominationcount[i]++;
            }
        }
    }
    for(int i=0;i<popsize;i++){
        front[i].clear();
    }
    
    for(int i=0;i<popsize;i++){
        for(int j=0;j<popsize;j++){
            if(dominationcount[j]==0){
                if(check[j]==-1){
                    front[i].push_back(j);
                    check[j]=0;
                }
            }
        }
        for(int j=0;j<front[i].size();j++){
            int t=front[i][j];
            for(int k=0;k<dominatescount[t].size();k++){
                int dom=dominatescount[t][k];
                dominationcount[dom]=dominationcount[dom]-1;
            }
        }
        
    }
    
    return front;

}
vector<vector<int>> longestTrips(vector<int> &route){
    vector<int> temp;
    int longest=0;
    vector<vector<int>> trips;
    for(int i=0;i<total_node;i++){
        if(route[i]<=num_cus&&route[i]>0)temp.push_back(route[i]);
        else{
            if(temp.size()>longest){
                trips.clear();
                longest=temp.size();
                trips.push_back(temp);
            }
            else if( temp.size()==longest){
                trips.push_back(temp);
            }
            temp.clear();
        }
    }
    if(temp.size()>longest){
        trips.clear();
        longest=temp.size();
        trips.push_back(temp);
    }
    else if( temp.size()==longest){
        trips.push_back(temp);
    }
    return trips;
}
int difTrips(vector<int> &t1,vector<int> &t2){
    for(int i=0;i<t1.size();i++){
        if(t1[i]!=t2[i])return 0;
    }
    return 1;
}
bool checkSol(vector<int> &route1, vector<int> &route2){
    vector<vector<int>>t1=longestTrips(route1);
    vector<vector<int>>t2=longestTrips(route2);
    if(t1.size()!=t2.size())return false;
    int t=0;
    for(int i=0;i<t1.size();i++){
        for(int j=0;j<t2.size();j++){
            if(t1[i].size()==t2[j].size()){
                t+=difTrips(t1[i],t2[j]);
            };
        }
    }
    if(t==t1.size())return true;
    return false;
}
vector<Individual> getremain(vector<Individual>population,vector<int> front){
    vector<Individual> remain;
    vector<Individual>temp;
    int frontsize=front.size();
    for(int i=0;i<frontsize;i++){
        temp.push_back(population[front[i]]);
    }
    sort(temp.begin(),temp.end(),comparefit1);
    vector<int> duplicate;
    duplicate.clear();
    vector<vector<int>> model;
    model.push_back(temp[0].route);
    double curF1=temp[0].fitness1;
    for(int i=1;i<frontsize;i++){
        if(abs(curF1-temp[i].fitness1)<1e-6){
            bool dup=false;
            for(int j=0;j<model.size();j++){
                if(checkSol(model[j],temp[i].route)){
                    dup=true;
                }
            }
            if(dup)duplicate.push_back(i);
        }
        else{
            model.clear();
            model.push_back(temp[i].route);
            curF1=temp[i].fitness1;
        }
    }
    for(int i=duplicate.size()-1;i>=0;i--){
        temp.erase(temp.begin()+duplicate[i]);
    }
    frontsize=temp.size();
    temp[0].crowdingdistance=mvalue;
    temp[frontsize-1].crowdingdistance=mvalue;
    
    for(int i=1;i<temp.size()-1;i++){
        
        temp[i].crowdingdistance+=(temp[i+1].fitness1-temp[i-1].fitness1)/(temp[frontsize-1].fitness1-temp[0].fitness1);
    }
    sort(temp.begin(),temp.end(),comparefit2);
    temp[0].crowdingdistance=mvalue;
    temp[frontsize-1].crowdingdistance=mvalue;

    for(int i=1;i<temp.size()-1;i++){
        
        if(temp[i].crowdingdistance!=mvalue) temp[i].crowdingdistance+=(temp[i+1].fitness2-temp[i-1].fitness2)/(temp[frontsize-1].fitness2-temp[0].fitness2);
    }
    sort(temp.begin(),temp.end(),compareDistance);
    for(int i=0;i<temp.size();i++){
        remain.push_back(temp[i]);
    }
    return remain;
}
vector<Individual> selectNewPopulation(vector<Individual> population,int popusize){
    vector<Individual> newpopulation;
    int popsize=population.size();
    for(int j=0;j<popsize;j++){
        population[j].crowdingdistance=0;
    }
    vector<vector<int>> front=fast_non_dominated_sort(population);
    int tot=0;
    int popcount=0;
    for(int i=0;i<popsize;i++){
        if(front[i].size()==0) break;
        vector<Individual> remain=getremain(population,front[i]);
        for(int j=0;j<remain.size();j++){
            newpopulation.push_back(remain[j]);
            popcount++;
            if(popcount==popusize) break;
        }
        if(popcount==popusize) break;
    }
    
    minobj1=newpopulation[0].fitness1;
    maxobj1=newpopulation[0].fitness1;
    minobj2=newpopulation[0].fitness2;
    maxobj2=newpopulation[0].fitness2;
    for(int j=0;j<newpopulation.size();j++){
        if(newpopulation[j].fitness1<minobj1) minobj1=newpopulation[j].fitness1;
        if(newpopulation[j].fitness2<minobj2) minobj2=newpopulation[j].fitness2;
        if(newpopulation[j].fitness1>maxobj1) maxobj1=newpopulation[j].fitness1;
        if(newpopulation[j].fitness2>maxobj1) maxobj1=newpopulation[j].fitness2;
    }
    obj1norm=maxobj1-minobj1;
    obj2norm=maxobj2-minobj2;
    
    return newpopulation;
}