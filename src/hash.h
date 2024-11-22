#include "parameter.h"
using namespace std;
extern int total_node;
extern int drone_max_tracks;
typedef size_t Py_uhash_t;
#define _PyHASH_XXPRIME_1 ((Py_uhash_t)11400714785074694791ULL)
#define _PyHASH_XXPRIME_2 ((Py_uhash_t)14029467366897019727ULL)
#define _PyHASH_XXPRIME_5 ((Py_uhash_t)2870177450012600261ULL)
#define _PyHASH_XXROTATE(x) ((x << 31) | (x >> 33))
Py_uhash_t vectorHashing(vector<int> t){
    int len =t.size();
    hash<int> hash_int;
    Py_uhash_t res=_PyHASH_XXPRIME_5 ;
    for(int i=0;i<len;i++){
        Py_uhash_t lane =Py_uhash_t(hash_int(t[i]));
        if (lane == (Py_uhash_t)-1) {
            return -1;
        }
        res+=lane* _PyHASH_XXPRIME_2;
        res=-_PyHASH_XXROTATE(res);
        res*=_PyHASH_XXPRIME_1;
    }
    res+= len^(_PyHASH_XXPRIME_5^3527539UL);
    return res;
}
Py_uhash_t v2Hashing(vector<vector<int>> t){
    int len=t.size();
    for(int i=0;i<len;i++){
        t[i].insert(t[i].begin(),0);
        t[i].push_back(0);
    }
    Py_uhash_t res=_PyHASH_XXPRIME_5 ;
    for(int i=0;i<len;i++){
        Py_uhash_t lane =vectorHashing(t[i]);
        if (lane == (Py_uhash_t)-1) {
            return -1;
        }
        res+=lane* _PyHASH_XXPRIME_2;
        res=-_PyHASH_XXROTATE(res);
        res*=_PyHASH_XXPRIME_1;
    }
    res+= len^(_PyHASH_XXPRIME_5^3527539UL);
    return res;
}
Py_uhash_t v3Hashing(vector<vector<vector<int>>> t){
    int len =t.size();
    Py_uhash_t res=_PyHASH_XXPRIME_5 ;
    for(int i=0;i<len;i++){
        Py_uhash_t lane =v2Hashing(t[i]);
        if (lane == (Py_uhash_t)-1) {
            return -1;
        }
        res+=lane* _PyHASH_XXPRIME_2;
        res=-_PyHASH_XXROTATE(res);
        res*=_PyHASH_XXPRIME_1;
    }
    res+= len^(_PyHASH_XXPRIME_5^3527539UL);
    return res;
}
Py_uhash_t SolHashing(Py_uhash_t truck,Py_uhash_t drone){
    int len =2;
    vector<Py_uhash_t> sol;
    sol.push_back(truck);
    sol.push_back(drone);
    Py_uhash_t res=_PyHASH_XXPRIME_5 ;
    for(int i=0;i<len;i++){
        Py_uhash_t lane =sol[i];
        if (lane == (Py_uhash_t)-1) {
            return -1;
        }
        res+=lane* _PyHASH_XXPRIME_2;
        res=-_PyHASH_XXROTATE(res);
        res*=_PyHASH_XXPRIME_1;
    }
    res+= len^(_PyHASH_XXPRIME_5^3527539UL);
    return res;
}
Py_uhash_t calculateHash(vector<int> route){
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
                Trucksroute.clear();
            }
        }
    }
    if(Trucksroute.size()>0){
        allTruckRoute.push_back(Trucksroute);   
    }
    double droneneed=0;
    vector<vector<int>> singleDroneRoute;
    singleDroneRoute.clear();
    for(int i=posDepot+1;i<total_node;i++){
        if(route[i]<=num_cus)Droneroute.push_back(route[i]);
        else{
            if(Droneroute.size()>0){
                singleDroneRoute.push_back(Droneroute);
            }
            int t=route[i]-num_cus-num_trucks+1;
            if(t>0&&t%(drone_max_tracks)==0){
                allDroneRoute.push_back(singleDroneRoute);
                singleDroneRoute.clear();
            }
            Droneroute.clear();
        }
        if(i==total_node-1){
            if(Droneroute.size()>0){
                singleDroneRoute.push_back(Droneroute);
                allDroneRoute.push_back(singleDroneRoute);
            }
        }

    }
    Py_uhash_t hashTruck=v2Hashing(allTruckRoute);
    Py_uhash_t hashDrone=v3Hashing(allDroneRoute);
    return SolHashing(hashTruck,hashDrone);

}