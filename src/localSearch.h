#ifndef LOCALSEARCH_H
#define LOCALSEARCH_H
#include "localSearchUtility.h"
#include "paretoUpdate.h"
#include <iostream>
using namespace std;
extern vector<Customer> customers;
extern int total_node;
bool check1(int i,int k,int iTrack,int kTrack,vector<int> temp,int banList){
    if(temp[i]==0||temp[i]>num_cus) return false;
    if(temp[k]==0||temp[k]>num_cus) return false;
    if(banList==1)return false;
    if(iTrack>=num_trucks&&customers[temp[k]].OnlyByTruck==1)return false;
    if(kTrack>=num_trucks&&customers[temp[i]].OnlyByTruck==1)return false;
    return true;
}
bool check2(int i,int k,int iTrack,int kTrack,vector<int>temp,int banList){
    if(temp[i]==0||temp[i]>num_cus) return false;
    if(kTrack>=num_trucks&&customers[temp[i]].OnlyByTruck==1)return false;
    if(banList==1) return false;
    return true;
}
pair<double,double> swap11(vector<int> Tracki,vector<int>Trackk,int iTrack,int kTrack,int i,int k){
    vector<int> tempTracki=Tracki;
    vector<int> tempTrackk=Trackk;
    if(iTrack==kTrack){
        int te=tempTracki[i];
        tempTracki[i]=tempTracki[k];
        tempTracki[k]=te;
        return TrackResult(tempTracki,iTrack);
    }
    int temp=tempTracki[i];
    tempTracki[i]=tempTrackk[k];
    tempTrackk[k]=temp;
    pair<double,double>iResult=TrackResult(tempTracki,iTrack);
    pair<double,double>kResult=TrackResult(tempTrackk,kTrack);
    if(iResult.first==0||kResult.first==0){
        return make_pair(0,0);
    }
    return make_pair(max(iResult.first,kResult.first),iResult.second+kResult.second);
}
pair<double,double> swap10(vector<int> Tracki,vector<int>Trackk,int iTrack,int kTrack,int i,int k){
    vector<int> tempTracki=Tracki;
    vector<int> tempTrackk=Trackk;
    if(iTrack==kTrack){
        if(k>i){
            tempTracki.insert(tempTracki.begin()+k+1,tempTracki[i]);
            tempTracki.erase(tempTracki.begin()+i);
        }
        else{
            tempTracki.insert(tempTracki.begin()+k,tempTracki[i]);
            tempTracki.erase(tempTracki.begin()+i+1);
        }
        
        return TrackResult(tempTracki,iTrack);
    }
    int temp=tempTracki[i];
    if(iTrack<kTrack){
        tempTrackk.insert(tempTrackk.begin()+k+1,tempTracki[i]);
        tempTracki.erase(tempTracki.begin()+i);
    }
    else{
        tempTrackk.insert(tempTrackk.begin()+k,tempTracki[i]);
        tempTracki.erase(tempTracki.begin()+i);
    }
    pair<double,double>iResult=TrackResult(tempTracki,iTrack);
    pair<double,double>kResult=TrackResult(tempTrackk,kTrack);
    if(iResult.first==0||kResult.first==0)return make_pair(0,0);
    return make_pair(max(iResult.first,kResult.first),iResult.second+kResult.second);
}
/*, vector<Individual> &pareto*/

void LocalSearcher1(vector<int> tour,int choice,int type,vector<Individual> &pareto) {
    bool improved = true;
    int count=0;
    int obj1mul=type*0.5;
    vector<int>bestimprove=tour;
    vector<int>copytour=tour;
    
    static int banList[1000][1000];
    for(int i=0;i<1000;i++){
        for(int j=0;j<1000;j++){
            banList[i][j]=0;
        }
    }
    /*vector<vector<int>> banList(total_node+1);
    for(int i=0;i<total_node+1;i++){
        for(int j=0;j<total_node+1;j++){
            banList[i].push_back(0);
        }
    }*/
    int imove=-1;
    int kmove=-1;
    while (improved) {
        count++;
        improved = false;
        for (int i=total_node-1; i >=0 ; i--) {
            int newc=0;
            
            pair<double,double> fitbefore=calculatefitness(copytour);
            
            vector<vector<int>> Tracks=splitTracks(copytour).first;
            vector<int> endpoint=splitTracks(copytour).second;
            vector<double> obj1List;
            vector<double> obj2List;
            obj1List.clear();
            obj2List.clear();
            for(int j=0;j<num_trucks+num_drones;j++){
                pair<double,double> oriTrackres=TrackResult(Tracks[j],j);
                obj1List.push_back(oriTrackres.first);
                obj2List.push_back(oriTrackres.second);
                
            }
            int te=0;
            double fitBest;
            if(type==1) fitBest=computeLinear(fitbefore.first,fitbefore.second);
            else if(type==0) fitBest =fitbefore.second;
            else fitBest=fitbefore.first;
            double fitInit=fitBest;
            int klimit=0;
            

            for (int k =total_node-1; k >=0; k--) {
                vector<int> temp=copytour;
                int iTrack=0;
                while(i>endpoint[iTrack]){
                    iTrack++;
                }
                iTrack-=1;
                int kTrack=0;
                while(k>endpoint[kTrack]){
                    kTrack++;
                }
                kTrack-=1;
                pair<double,double> fitafter;
                pair<double,double> fitchange;
                fitchange.first=0;
                fitchange.second=0;
                
                // Perform the swap
                int error=0;
                if(i!=k){
                    if(choice==50||choice==150){
                        
                        if(check1(i,k,iTrack,kTrack,temp,banList[i][k])){
                            fitchange=swap11(Tracks[iTrack],Tracks[kTrack],iTrack,kTrack,i-endpoint[iTrack]-1,k-endpoint[kTrack]-1);
                            if(iTrack==kTrack){
                                fitafter.second=fitbefore.second+fitchange.second-obj2List[iTrack];
                                vector<double> obj1new;
                                obj1new.clear();
                                for(int j=0;j<obj1List.size();j++){
                                    if(j!=iTrack)obj1new.push_back(obj1List[j]);
                                }
                                obj1new.push_back(fitchange.first);
                                
                                sort(obj1new.begin(),obj1new.end());
                                fitafter.first=obj1new.back();
                            }
                            else{
                                fitafter.second=fitbefore.second+fitchange.second-obj2List[iTrack]-obj2List[kTrack];
                                vector<double> obj1new;
                                obj1new.clear();
                                for(int j=0;j<obj1List.size();j++){
                                    if(j!=iTrack&&j!=kTrack)obj1new.push_back(obj1List[j]);
                                }
                                obj1new.push_back(fitchange.first);
                                sort(obj1new.begin(),obj1new.end());
                                fitafter.first=obj1new.back();
                            }
                            
                        }
                        else error=1;
                    }
                    else{
                        if(check2(i,k,iTrack,kTrack,temp,banList[i][k])){
                            if(k==endpoint[kTrack+1]){
                                if(i<k) {
                                    if(check2(i,k,iTrack,kTrack+1,temp,banList[i][k])) {te=1;fitchange=swap10(Tracks[iTrack],Tracks[kTrack+1],iTrack,kTrack+1,i-endpoint[iTrack]-1,-1);}
                                    
                                    else fitchange=make_pair(0,0);
                                }
                                else fitchange=swap10(Tracks[iTrack],Tracks[kTrack],iTrack,kTrack,i-endpoint[iTrack]-1,Tracks[kTrack].size());
                            }
                            else fitchange=swap10(Tracks[iTrack],Tracks[kTrack],iTrack,kTrack,i-endpoint[iTrack]-1,k-endpoint[kTrack]-1);
                            if(iTrack==kTrack&&k!=endpoint[kTrack+1]){
                                fitafter.second=fitbefore.second+fitchange.second-obj2List[iTrack];
                                vector<double> obj1new;
                                obj1new.clear();
                                for(int j=0;j<num_trucks+num_drones;j++){
                                    if(j!=iTrack)obj1new.push_back(obj1List[j]);
                                }
                                obj1new.push_back(fitchange.first);
                                sort(obj1new.begin(),obj1new.end());
                                fitafter.first=obj1new.back();
                            }
                            else if(iTrack==kTrack&&k==endpoint[kTrack+1]){
                                fitafter.second=fitbefore.second+fitchange.second-obj2List[iTrack]-obj2List[kTrack+1];
                                vector<double> obj1new;
                                obj1new.clear();
                                for(int j=0;j<num_trucks+num_drones;j++){
                                    if(j!=iTrack&&j!=kTrack+1)obj1new.push_back(obj1List[j]);
                                }
                                obj1new.push_back(fitchange.first);
                                sort(obj1new.begin(),obj1new.end());
                                fitafter.first=obj1new.back();
                            }
                            else if(iTrack<kTrack&&k==endpoint[kTrack+1]){
                                fitafter.second=fitbefore.second+fitchange.second-obj2List[iTrack]-obj2List[kTrack+1];
                                vector<double> obj1new;
                                obj1new.clear();
                                for(int j=0;j<num_trucks+num_drones;j++){
                                    if(j!=iTrack&&j!=kTrack+1)obj1new.push_back(obj1List[j]);
                                }
                                obj1new.push_back(fitchange.first);
                                sort(obj1new.begin(),obj1new.end());
                                fitafter.first=obj1new.back();
                            }
                            else{
                                fitafter.second=fitbefore.second+fitchange.second-obj2List[iTrack]-obj2List[kTrack];
                                vector<double> obj1new;
                                obj1new.clear();
                                for(int j=0;j<num_trucks+num_drones;j++){
                                    if(j!=iTrack&&j!=kTrack)obj1new.push_back(obj1List[j]);
                                }
                                obj1new.push_back(fitchange.first);
                                sort(obj1new.begin(),obj1new.end());                                
                                fitafter.first=obj1new.back();
                            }
                        }
                        else error=1;
                    
                    }
                }
                double fitTemp;
                
                
                // Check if the swap improved the tour
                if(fitchange.first==0||fitchange.second==0)error=1;
                if(error==0){
                    if(choice==50||choice==150) swap(temp[i],temp[k]);
                    else{
                        if(k>i){
                            temp.insert(temp.begin()+k+1,temp[i]);
                            temp.erase(temp.begin()+i);
                        }
                        else{
                            temp.insert(temp.begin()+k,temp[i]);
                            temp.erase(temp.begin()+i+1);
                        }
                    }
                }
                
                if(type==1)fitTemp=computeLinear(fitafter.first,fitafter.second);
                else if(type==0) fitTemp =fitafter.second;
                else fitTemp=fitafter.first;

                if (fitBest-fitTemp>1e-3&&feasibleroute(temp)==true&&error==0) {
                    improved=true;
                    bestimprove=temp;
                    imove=i;
                    kmove=k;
                    newc=1;
                    fitBest=fitTemp;
                    Individual newsol;
                    newsol.route=copytour;
                    newsol.fitness1=fitafter.first;
                    newsol.fitness2=fitafter.second;
                    newsol.tabusearch=0;
                    newsol.localsearch=0;
                    if(!inpareto(newsol,pareto)){
                        updatepareto(newsol,pareto);
                    } 
                 
                }
                if(fitInit<fitTemp||error==1)banList[i][k]=1;
            }
            copytour=bestimprove;
            if(choice==50&&newc==1){
                for(int s=0;s<total_node;s++){
                    banList[imove][s]=0;
                    banList[kmove][s]=0;
                    banList[s][imove]=0;
                    banList[s][kmove]=0;
                    if(imove>0) {banList[imove-1][s]=0;banList[s][imove-1]=0;}
                    if(imove<total_node-1){banList[imove+1][s]=0;banList[s][imove+1]=0;}
                    if(kmove>0) {banList[kmove-1][s]=0;banList[s][kmove-1]=0;}
                    if(kmove<total_node-1){banList[kmove+1][s]=0;banList[s][kmove+1]=0;}
                }
            }else if(choice==100&&newc==1){
                if(imove<kmove){
                    for(int u=0;u<imove-1;u++){
                        for(int v=0;v<total_node;v++){
                            if(v==imove-1)banList[u][v]=0;
                            if(v>=imove&&v<kmove-1){int tp=banList[u][v+1]; banList[u][v]=tp;}
                            if(v==kmove-1||v==kmove)banList[u][v]=0;
                        }
                    }
                
                    for(int u=kmove+2;u<total_node;u++){
                        for(int v=0;v<total_node;v++){
                            if(v==imove)banList[u][v]=0;
                            if(v>imove&&v<kmove){int tp=banList[u][v+1];banList[u][v]=tp;}
                            if(v==kmove||v==kmove+1)banList[u][v]=0;
                        }
                    }
                
                    for(int u=imove+1;u<kmove-1;u++){
                        for(int v=0;v<total_node;v++){
                            if(v==imove)banList[u][v]=0;
                            else if(v>imove&&v<kmove-1){int tp=banList[u+1][v+1];banList[u][v]=tp;}
                            else if(v==kmove-1||v==kmove)banList[u][v]=0;
                            else {int tp=banList[u+1][v];banList[u][v]=tp;}
                        }
                    }
                
                    for(int u=0;u<total_node;u++){
                        if(imove>0)banList[imove-1][u]=0;
                        banList[imove][u]=0;
                        banList[kmove][u]=0;
                        banList[kmove-1][u]=0;
                        if(kmove<total_node-1)banList[kmove+1][u]=0;
                    }
                
                }
                else if(imove>kmove){
                    for(int u=total_node-1;u>imove+1;u--){
                        for(int v=total_node-1;v>=0;v--){
                            if(v==imove+1)banList[u][v]=0;
                            if(v<=imove&&v>kmove+1){int tp=banList[u][v-1];banList[u][v]=tp;}
                            if(v==kmove+1||v==kmove)banList[u][v]=0;
                        }
                    }
                    for(int u=kmove-2;u>=0;u--){
                        for(int v=total_node-1;v>=0;v--){
                            if(v==imove)banList[u][v]=0;
                            if(v<imove&&v>kmove){int tp=banList[u][v-1];banList[u][v]=tp;}
                            if(v==kmove||v==kmove-1)banList[u][v]=0;
                        }
                    }
                    for(int u=imove-1;u>kmove+1;u--){
                        for(int v=total_node-1;v>=0;v--){
                            if(v==imove)banList[u][v]=0;
                            else if(v<imove&&v>kmove+1){int tp=banList[u-1][v-1];banList[u][v]=tp;}
                            else if(v==kmove+1||v==kmove)banList[u][v]=0;
                            else {int tp=banList[u-1][v];banList[u][v]=tp;}
                        }
                    }
                    for(int u=0;u<total_node;u++){
                        banList[imove+1][u]=0;
                        banList[imove][u]=0;
                        banList[kmove][u]=0;
                        if(kmove>0)banList[kmove-1][u]=0;
                        if(kmove<total_node-1)banList[kmove+1][u]=0;
                    }
                }
                        
            }
        }
        if(count>=100){improved=false;}//cout<<"er"<<endl;}
    }
    //return bestimprove;
}
#endif