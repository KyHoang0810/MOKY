#ifndef ROUTECHECK_H
#define ROUTECHECK_H
#include "parameter.h"
using namespace std;
//feasible route
extern vector<Customer> customers;
extern vector<vector<double>> M;
extern int total_node;
bool feasibleroute(vector<int>route){
    for(int i=0;i<total_node;i++){
        if(route[i]>=total_node||route[i]<0)return false;
    }
    return true;
}
//check feasible droneroute
int feasibleDroneroute(vector<int> Droneroute){
    vector<int> copyRoute;
    for(int i=0;i<Droneroute.size();i++){
        int te=Droneroute[i];
        copyRoute.push_back(te);
    }
    copyRoute.insert(copyRoute.begin(),0);
    copyRoute.push_back(0);
    int Dronecus=copyRoute.size();
    double totalweight=0;
    double weight=0;
    double totalenergy=0;
    for(int i=0;i<Droneroute.size();i++){
        if(customers[Droneroute[i]].OnlyByTruck==1)return 1;
    }
    for(int i=0;i<Droneroute.size();i++){
        totalweight+=customers[Droneroute[i]].demand;
        if(totalweight>capacityC)return 2;
    }
    
    for(int i=0;i<Dronecus-1;i++){
        double energypersec=gama+betaB*weight;
        double distance=M[copyRoute[i]][copyRoute[i+1]];
        double time=distance/cruiseSpeed;
        totalenergy+=(takeoffTime+landingTime+time)*energypersec;
        if(totalenergy> batteryPower) return 3;
        weight+=customers[copyRoute[i+1]].demand;
    }
    return 0;
}
bool checkroute(vector<int> temp){
    int total_node=temp.size();
    int posDepot=find(temp.begin(),temp.end(),0)-temp.begin();
    vector<int> droneroute;
    for(int i=posDepot+1;i<total_node;i++){
        if(temp[i]>num_cus){
            if(droneroute.size()>0){
                if(feasibleDroneroute(droneroute)>0)return false;
            }
            droneroute.clear();
        }
        else droneroute.push_back(temp[i]);
    }
    if(droneroute.size()>0){
        if(feasibleDroneroute(droneroute)>0)return false;
    }
    droneroute.clear();
    return true;
    
}
#endif