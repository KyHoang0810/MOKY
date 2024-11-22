#ifndef LOCALUTI_H
#define LOCALUTI_H
#include "fitnessCal.h"
#include "routecheck.h"
using namespace std;
extern int total_node;
extern int drone_max_tracks;
extern double minobj1;
extern double minobj2;
extern double obj1norm;
extern double obj2norm;
pair<vector<vector<int>>,vector<int>> splitTracks(vector<int>copytour){
    vector<vector<int>> Tracks;
    Tracks.clear();
    vector<int>Track;
    Track.clear();
    vector<int> endpoint;
    endpoint.push_back(-1);
    for(int j=0;j<total_node;j++){
        if(copytour[j]==0||copytour[j]>num_cus){
            if(copytour[j]==0||copytour[j]<num_cus+num_trucks){
                endpoint.push_back(j);
                Tracks.push_back(Track);
                Track.clear();
            }
            else{
                if((copytour[j]-num_cus-num_trucks+1)%drone_max_tracks==0){
                    endpoint.push_back(j);
                    Tracks.push_back(Track);
                    Track.clear();  
                }
                else Track.push_back(copytour[j]);
            }
        }
        else Track.push_back(copytour[j]);
    }
    Tracks.push_back(Track);
    endpoint.push_back(total_node);
    return make_pair(Tracks,endpoint);
}
pair<double,double> TrackResult(vector<int> track,int Tracknum){
    double trackobj1=0;
    double trackobj2=0;
    if(Tracknum<num_trucks){
        return calculateTrucktime(track);
    }
    else{
        vector<int>droneroute;
        droneroute.clear();
        for(int j=0;j<track.size();j++){
            if(track[j]<=num_cus){
                droneroute.push_back(track[j]);
            }
            else{
                if(feasibleDroneroute(droneroute)!=0)return make_pair(0,0);
                if(droneroute.size()>0){
                    pair<double,double> dresult=calculateDronetime(droneroute);
                    trackobj1+=dresult.first;
                    trackobj2+=dresult.second;
                }
                droneroute.clear();
            }
        }
        if(feasibleDroneroute(droneroute)!=0)return make_pair(0,0);
        if(droneroute.size()>0){
            pair<double,double> dresult=calculateDronetime(droneroute);
            trackobj1+=dresult.first;
            trackobj2+=dresult.second;
        }

    }
    return make_pair(trackobj1,trackobj2);
    
}
double computeLinear(double obj1,double obj2){
    return (obj1-minobj1)/obj1norm+(obj2-minobj2)/obj2norm;
}
#endif