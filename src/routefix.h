#ifndef ROUTEFIX_H
#define ROUTEFIX_H
#include "routecheck.h"
#include "fitnessCal.h"
#include <iostream>
using namespace std;
extern int total_node;
vector<int> fixroute(vector<int> droneroute){
    vector<int>temp;
    vector<int>error;
    for(int i=0;i<droneroute.size();i++){
        temp.push_back(droneroute[i]);
        if(feasibleDroneroute(temp)!=0){
            temp.pop_back();
            error.push_back(droneroute[i]);
        }
    }
    return error; 
}
vector<int> repairroute(vector<int> &route){
    vector<int> newroute;
    int total_node=route.size();
    int depotPos=find(route.begin(),route.end(),0)-route.begin();
    for(int i=0;i<=depotPos;i++){
        newroute.push_back(route[i]);
    }
    vector<int> droneroute;
    vector<int> error;
    for(int i=depotPos+1;i<total_node;i++){
        if(route[i]>num_cus){
            droneroute.insert(droneroute.end(),error.begin(),error.end());
            error.clear();
            if(feasibleDroneroute(droneroute)!=0) {
                error= fixroute(droneroute);
            }
            for(int j=0;j<error.size();j++){
                droneroute.erase(find(droneroute.begin(),droneroute.end(),error[j]));
            }
            for(int j=0;j<droneroute.size();j++){
                newroute.push_back(droneroute[j]);
            }
            newroute.push_back(route[i]);
            droneroute.clear();
        }
        else{droneroute.push_back(route[i]);}
    }
    droneroute.insert(droneroute.end(),error.begin(),error.end());
    error.clear();
    if(feasibleDroneroute(droneroute)!=0) {
        error=fixroute(droneroute);
    }
    for(int j=0;j<error.size();j++){
        droneroute.erase(find(droneroute.begin(),droneroute.end(),error[j]));
    }
    for(int j=0;j<droneroute.size();j++){
        newroute.push_back(droneroute[j]);
    }

    for(int i=0;i<error.size();i++){
        int er=error[i];
        int bestposition;
        double best=0;
        vector<int>copyroute;
        for(int j=0;j<depotPos;j++){
            copyroute.push_back(newroute[j]);
        }
        for(int j=0;j<=depotPos;j++){
            vector<int>temp=copyroute;
            vector<int>Trucksroute;
            double fit1=0;
            double fit2=0;
            temp.insert(j+temp.begin(),er);
            for(int i=0;i<temp.size();i++){
                if(temp[i]<=num_cus)Trucksroute.push_back(temp[i]);
                else{
                    if(Trucksroute.size()>0){
                        pair<double,double>result=calculateTrucktime(Trucksroute);
                        fit2+=result.second;
                        if(result.first>fit1)fit1=result.first;
                        Trucksroute.clear();
                    }
                }
            }
            if(Trucksroute.size()>0){
                pair<double,double>re=calculateTrucktime(Trucksroute);
                fit2+=re.second;
                if(re.first>fit1)fit1=re.first;
            }
            if(best==0||fit1+fit2<best){
                bestposition=j;
                best=fit1+fit2;
            }
        }
        //int raninsert;
        //if(depotPos==0) raninsert=0;
        //else raninsert=rand()%depotPos;
        newroute.insert(bestposition+newroute.begin(),er);
        depotPos++;    
    }
    if(newroute.size()<total_node)cout<<"error"<<endl;
    return newroute;
}
vector<int> repairposition(vector<int> route){
    int total_node=route.size();
    int trucksdone=1;
    int depot=0;
    int dronedone=0;
    for(int i=0;i<total_node;i++){
        if(route[i]==0||route[i]>num_cus){
            if(trucksdone==num_trucks){
                if(depot==0){
                    int pos=find(route.begin(),route.end(),0)-route.begin();
                    if(i!=pos)swap(route[i],route[pos]);
                    depot++;
                }
                else{
                    int pos=find(route.begin(),route.end(),num_cus+num_trucks+dronedone)-route.begin();
                    if(i!=pos)swap(route[i],route[pos]);
                    dronedone++;

                }
            }
            if(trucksdone<num_trucks){
                int pos=find(route.begin(),route.end(),num_cus+trucksdone)-route.begin();
                swap(route[i],route[pos]);
                trucksdone++;
            }
            
        }
    }
    return route;
}
#endif