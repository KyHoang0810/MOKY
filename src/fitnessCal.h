#ifndef FITNESSCAL_H
#define FITNESSCAL_H
#include "parameter.h"
using namespace std;
extern int total_node;
extern int drone_max_tracks;
extern vector<vector<double>> M;
vector<double> TruckHour{0.7, 0.4, 0.6, 0.7, 0.8, 0.9, 1.0, 0.7, 0.6, 0.5, 0.7, 0.8};
pair<double,double> calculateTrucktime(vector<int> &Truckroute){
    vector<int> copyRoute=Truckroute;
    copyRoute.insert(copyRoute.begin(),0);
    copyRoute.push_back(0);
    int Truckcus=copyRoute.size();
    double timeneed=0;
    double totaltimewait=0;
    vector<double> timewait;
    timewait.reserve(Truckcus);
    
    for(int i=0;i<Truckcus-1;i++){
        double distance =M[copyRoute[i]][copyRoute[i+1]];
        double hour=(int)(timeneed/3600);
        while(distance-((hour+1)*3600-timeneed)*VmaxTruck*TruckHour[((int)hour%12)]>0){
            distance-=((hour+1)*3600-timeneed)*VmaxTruck*TruckHour[(int)hour%12];
            timeneed=(hour+1)*3600;
            hour=(int)timeneed/3600;
        }
        timeneed+=distance/(VmaxTruck*TruckHour[int(hour)%12])+truckservetime;
        timewait.push_back(timeneed);
    }
    timeneed-=truckservetime;
    for(int i=0;i<Truckroute.size();i++){
        totaltimewait+=(timeneed-timewait[i]);
    }
    return make_pair(timeneed,totaltimewait);
}
pair<double,double> calculateDronetime(vector<int>Droneroute){
    vector<int> copyRoute=Droneroute;
    copyRoute.insert(copyRoute.begin(),0);
    copyRoute.push_back(0);
    int Dronecus=copyRoute.size();
    double timeneed=0;
    vector<double> timewait;
    double totaltimewait;
    for(int i=0;i<Dronecus-1;i++){
        double distance=M[copyRoute[i]][copyRoute[i+1]];
        double time=distance/cruiseSpeed;
        timeneed+=(takeoffTime+landingTime+time)+droneservetime;
        timewait.push_back(timeneed);
    }
    timeneed-=droneservetime;
    for(int i=0;i<Droneroute.size();i++){
        totaltimewait+=(timeneed-timewait[i]);
    }
    return make_pair(timeneed,totaltimewait);

}
pair<double,double>calculatefitness(vector<int>route){
    double timeneed=0;
    double totaltimewait=0;
    int posDepot=find(route.begin(),route.end(),0)-route.begin();
    vector<int> Trucksroute;
    vector<vector<int>>allTruckRoute;
    vector<int>Droneroute;
    vector<vector<vector<int>>> allDroneRoute;
    vector<int>dronetimeneed(num_drones);
    for(int i=0;i<posDepot;i++){
        if(route[i]<=num_cus)Trucksroute.push_back(route[i]);
        else{
            if(Trucksroute.size()>0){
                allTruckRoute.push_back(Trucksroute);
                pair<double,double>result=calculateTrucktime(Trucksroute);
                totaltimewait+=result.second;
                if(result.first>timeneed)timeneed=result.first;
                Trucksroute.clear();
            }
        }
    }
    if(Trucksroute.size()>0){
        pair<double,double>re=calculateTrucktime(Trucksroute);
        allTruckRoute.push_back(Trucksroute);
        totaltimewait+=re.second;
        if(re.first>timeneed)timeneed=re.first;
    }
    double droneneed=0;
    vector<vector<int>> singleDroneRoute;
    singleDroneRoute.clear();
    for(int i=posDepot+1;i<total_node;i++){
        if(route[i]<=num_cus)Droneroute.push_back(route[i]);
        else{
            if(Droneroute.size()>0){
                pair<double,double>result=calculateDronetime(Droneroute);
                singleDroneRoute.push_back(Droneroute);
                droneneed+=result.first;
                totaltimewait+=result.second;
            }
            int t=route[i]-num_cus-num_trucks+1;
            if(t>0&&t%(drone_max_tracks)==0){
                if(droneneed>timeneed)timeneed=droneneed;
                droneneed=0;
                allDroneRoute.push_back(singleDroneRoute);
                singleDroneRoute.clear();
            }
            Droneroute.clear();
        }
        if(i==total_node-1){
            if(Droneroute.size()>0){
                pair<double,double>result=calculateDronetime(Droneroute);
                droneneed+=result.first;
                totaltimewait+=result.second;
                singleDroneRoute.push_back(Droneroute);
                allDroneRoute.push_back(singleDroneRoute);
            }
            if(droneneed>timeneed)timeneed=droneneed;
        }

    }
    
    return make_pair(timeneed,totaltimewait);
}
#endif