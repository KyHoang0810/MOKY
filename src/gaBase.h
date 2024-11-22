#ifndef GABASE_H
#define GABASE_H
#include "routefix.h"
#include <random>
#include <set>
#include "compare.h"
using namespace std;
extern vector<Customer> customers;
extern int drone_max_tracks;
extern int total_node;
extern double minobj1;
extern double minobj2;
extern double obj1norm;
extern double obj2norm;
double computeLinear1(double obj1,double obj2){
    return (obj1-minobj1)/obj1norm+(obj2-minobj2)/obj2norm;
}
double calTruck(int cur,int des,double timeUse){
    double distance =M[cur][des];
    int hour=(int)(timeUse/3600);
    while(distance-((hour+1)*3600-timeUse)*VmaxTruck*TruckHour[((int)hour%12)]>0){
        distance-=((hour+1)*3600-timeUse)*VmaxTruck*TruckHour[(int)hour%12];
        timeUse=(hour+1)*3600;
        hour=(int)(timeUse/3600);
    }
    timeUse+=distance/(VmaxTruck*TruckHour[int(hour)%12])+truckservetime;
    return timeUse;
}
double calDrone(int cur, int des){
    double distance=M[cur][des];
    double time=distance/cruiseSpeed;
    return (takeoffTime+landingTime+time)+droneservetime;
}
double findRank(int cur,int target, vector<int> &temp){
    vector<NodeDistance> nds;
    for(int i=0;i<temp.size();i++){
        if(i!=cur){
            NodeDistance nd;
            nd.node=temp[i];
            nd.distance=M[temp[i]][temp[cur]];
            nds.push_back(nd);
        }
    }
    sort(nds.begin(),nds.end(),compareNodeDistance);
    int found=0;
    for(int i=0;i<nds.size();i++){
        if(nds[i].node==temp[target])found= i;
    }
 
    return double(found);

}
vector<int> distanceRanking(int cur, vector<int> remaining){
    vector<DistanceRanking> drs;
    vector<int> temp=remaining;
    temp.push_back(cur);
    for(int i=0;i<remaining.size();i++){
        DistanceRanking a;
        a.node=remaining[i];
        a.ranking=(findRank(i,temp.size()-1,temp)+findRank(temp.size()-1,i,temp))/2.0;
        //cout<<a.ranking<<endl;
        drs.push_back(a);
    }
    sort(drs.begin(),drs.end(),compareDistanceRanking);
    vector<int> result;
    result.clear();
    for(int i=0;i<drs.size();i++){
        result.push_back(drs[i].node);    
    }
    return result;
}
int checkDroneable(vector<int> group){
    int cnt=0;
    for(int i=0;i<group.size();i++){
        if(customers[group[i]].OnlyByTruck==0)cnt++;
    }
    return cnt;
}
vector<int> generateSol2(){
    vector<int>  group[num_trucks];
    bool check=false;
    while(!check){
        for(int i=0;i<num_trucks;i++){
            group[i].clear();
        }
        check=true;
        for(int i=1;i<=num_cus;i++){
            int sel=rand()%num_trucks;
            group[sel].push_back(i);
        };
        for(int i=0;i<num_trucks;i++){
            if(group[i].size()<2) check =false;
        }
    }
    
    

    vector<vector<int>> trucks(num_trucks);
    vector<vector<vector<int>>>drones(num_drones);
    for(int i=0;i<num_trucks;i++){
        int droneable=checkDroneable(group[i]);
        vector<int> truckRoute;
        truckRoute.clear();
        vector<vector<int>>droneRoutes;
        droneRoutes.clear();
        vector<int> droneRoute;
        droneRoute.clear();
        int truckRand=rand()%(group[i].size());
        truckRoute.push_back(group[i][truckRand]);
        if(customers[truckRoute.back()].OnlyByTruck==0)droneable-=1;
        group[i].erase(group[i].begin()+truckRand);
        int droneRand;
        double droneTime=0.0;
        if(droneable!=0){
            do{
                droneRand=rand()%(group[i].size());
            }while(customers[group[i][droneRand]].OnlyByTruck!=0);
            droneRoute.push_back(group[i][droneRand]);
            group[i].erase(group[i].begin()+droneRand);
            droneTime=calDrone(0,droneRoute.back());
            droneable-=1;
        }
        else droneTime=mvalue;
        double truckTime=calTruck(0,truckRoute[0],0.0);
        while(group[i].size()!=0){
            if(droneTime<truckTime){
                int cur;
                if(droneRoute.size()!=0)cur=droneRoute.back();
                else cur=0;
                vector<int> sel=distanceRanking(cur,group[i]);
                bool fin=false;
                int cnt=0;
                for(int cnt=0;cnt<sel.size();cnt++){
                    if(customers[sel[cnt]].OnlyByTruck==0){
                        droneRoute.push_back(sel[cnt]);
                        if(feasibleDroneroute(droneRoute)==0){fin=true;break;}
                        else droneRoute.pop_back();
                    } 
                }
                if(fin){
                    int po=find(group[i].begin(),group[i].end(),droneRoute.back())-group[i].begin();
                    group[i].erase(group[i].begin()+po);
                    droneTime+=calDrone(cur,droneRoute.back());
                    droneable-=1;
                }
                else{
                    droneRoutes.push_back(droneRoute);
                    if(cur!=0){
                        droneTime+=calDrone(cur,0)-droneservetime;
                    }
                    droneRoute.clear();
                }
                if(droneable==0||droneRoutes.size()==drone_max_tracks) {droneTime=mvalue;}
            }
            else if(truckTime<=droneTime){
                int trucksel=distanceRanking(truckRoute.back(),group[i])[0];
                truckTime=calTruck(truckRoute.back(),trucksel,truckTime);
                truckRoute.push_back(trucksel);
                if(customers[truckRoute.back()].OnlyByTruck==0)droneable-=1;
                int po=find(group[i].begin(),group[i].end(),trucksel)-group[i].begin();
                group[i].erase(group[i].begin()+po);
                if(droneable==0) droneTime=mvalue;
            }
        }
        if(droneRoute.size()>0)droneRoutes.push_back(droneRoute);
        trucks[i]=truckRoute;
        truckTime=calTruck(truckRoute.back(),0,truckTime)-truckservetime;
        if(truckTime!=calculateTrucktime(trucks[i]).first)cout<<truckTime<<" "<<calculateTrucktime(trucks[i]).first<<endl;
        drones[i]=droneRoutes;
    }


    vector<int> tour;
    for(int i=0;i<num_trucks;i++){
        for(int j=0;j<trucks[i].size();j++){
            tour.push_back(trucks[i][j]);   
        }
        if(i<num_trucks-1)tour.push_back(num_cus+i+1);
    }
    tour.push_back(0);
    for(int i=0;i<num_drones;i++){
        for(int j=0;j<drones[i].size();j++){
            for(int k=0;k<drones[i][j].size();k++){
                tour.push_back(drones[i][j][k]);
            }
            tour.push_back(num_cus+num_trucks+j+i*drone_max_tracks);
        }
        for(int j=drones[i].size();j<drone_max_tracks;j++){
            tour.push_back(num_cus+num_trucks+j+i*drone_max_tracks);
        }
    }
    tour.pop_back();

    return tour;
}
vector<int> generatesol(){
    vector<int>sol;
    vector<int> drone;
    vector<int> truck;
    vector<vector<int>> trucksroute(num_trucks);
    vector<vector<int>> droneroute(num_drones);
    for(int i=1;i<=num_cus;i++){
        int random=rand() %10;
        if(customers[i].OnlyByTruck==0&&random<7) drone.push_back(i);
        else truck.push_back(i);
    } 
    for(int i=0; i<truck.size();i++){
        int randtr=rand()%(num_trucks);
        trucksroute[randtr].push_back(truck[i]);
    }
    for(int i=0;i<num_trucks;i++){
        random_device rd;
        mt19937 gen(rd());
        shuffle(trucksroute[i].begin(), trucksroute[i].end(), gen);  
    }

    for(int i=0;i<num_trucks;i++){
        for(int j=0;j<trucksroute[i].size();j++){
            sol.push_back(trucksroute[i][j]);
        }
        if(i==num_trucks-1) {sol.push_back(0);}
        else{
            sol.push_back((num_cus+i+1));
        }
    }
    for(int i=0;i<drone.size();i++){
        int randdr=rand()%num_drones;
        droneroute[randdr].push_back(drone[i]);
    }
    for(int i=0;i<num_drones;i++){
        random_device rd;
        mt19937 gen(rd());
        shuffle(droneroute[i].begin(), droneroute[i].end(), gen);  
    }
    for(int i=0;i<num_drones;i++){
        int dronetrfin=0;
        for(int j=0;j<droneroute[i].size();j++){
            sol.push_back(droneroute[i][j]);
            if(dronetrfin<drone_max_tracks-1){
                int randendtrack=rand()%2;
                if(randendtrack==1) {sol.push_back(num_cus+num_trucks+dronetrfin+i*drone_max_tracks);dronetrfin++;}

            }
        }
        while(dronetrfin<drone_max_tracks-1){
            sol.push_back(num_cus+num_trucks+dronetrfin+i*drone_max_tracks);
            dronetrfin++;
        }
        if(i<num_drones-1){
            sol.push_back(num_cus+num_trucks+drone_max_tracks*(i+1)-1);
        }
    }
    sol=repairroute(sol);
    return sol;
}
vector<Individual> selectPopulation(int size) {
    vector<Individual> population;
    for (int i = 0; i < size; i++) {
        Individual indi;
        vector<int>route;
        //route=generateSol2();
        //if(i>=size/2&&type==1)route=generatesol();
        route=generateSol2();
        if(route.size()!=total_node) cout<<"err"<<endl;
        
        indi.route=route;
        pair<double,double> fit=calculatefitness(route);
        indi.fitness1=fit.first;
        indi.fitness2=fit.second;
        indi.crowdingdistance=0;
        indi.tabusearch=0;
        indi.localsearch=0;
        population.push_back(indi);
    }
    return population;
}
Individual tournamentSelection(vector<Individual>& population){
    set<int> random_numbers;
    // Generate 4 unique random numbers
    while (random_numbers.size() < 4) {
        int number = rand()%200;
        random_numbers.insert(number); // set automatically handles uniqueness
    } 
    double best=0;
    int bestSol=0;
    for(const int& num : random_numbers){
        if(computeLinear1(population[num].fitness1,population[num].fitness2)<best||best==0){
            best=computeLinear1(population[num].fitness1,population[num].fitness2);
            bestSol=num;
        }
    }
    return population[bestSol];
}
bool checkdifsol(vector<int> route1,vector<int> route2){
    for(int i=1;i<=num_cus;i++){
        int po1=find(route1.begin(),route1.end(),i)-route1.begin();
        int po2=find(route2.begin(),route2.end(),i)-route2.begin();
        if(po1<total_node-1&&po2<total_node-1){
            if(route1[po1+1]!=route2[po2+1]&&(route1[po1+1]<=num_cus||route2[po2+1]<=num_cus))return true;
        }
    }
    return false;
}
#endif