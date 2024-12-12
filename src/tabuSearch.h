#include <algorithm> 
#include "paretoUpdate.h"
#include "localSearchUtility.h"
#include <iostream>
#include <chrono>
#include <fstream>
#include "nsgaIIBase.h"
#include "output.h"
using namespace std;
extern vector<Customer> customers;
extern vector<vector<double>> M;
bool in_tabu_list(vector<MoveRecord> tabulist, MoveRecord move,int type){
    if(tabulist.size()==0)return false;
    for(int i=0;i<tabulist.size();i++){
        if(move.start[0]==tabulist[i].start[0]&&move.start[1]==tabulist[i].start[1]&&move.end==tabulist[i].end){ 
            if(type%2==1)return true;
            else{
                if(move.back[1]==tabulist[i].back[0]&&move.front[1]==tabulist[i].front[0]&&move.back[0]==tabulist[i].back[1]&&move.front[0]==tabulist[i].front[1])
                    return true;
            }
        }
    }
    return false;
}
pair<double,double> swapTabu20(vector<vector<int>> Tracks,int iTrack,int position,int kTrack,int swapPo){
    vector<int> tempTracki=Tracks[iTrack];
    vector<int> tempTrackk=Tracks[kTrack];
    int temp=tempTracki[position];
    int temp2=tempTracki[position+1];
    if(iTrack>kTrack){
        tempTrackk.insert(tempTrackk.begin()+swapPo,temp2);
        tempTrackk.insert(tempTrackk.begin()+swapPo,temp);
        tempTracki.erase(tempTracki.begin()+position);
        tempTracki.erase(tempTracki.begin()+position);
    }
    else if(iTrack<kTrack){
        tempTrackk.insert(tempTrackk.begin()+swapPo+1,temp2);
        tempTrackk.insert(tempTrackk.begin()+swapPo+1,temp);
        tempTracki.erase(tempTracki.begin()+position);
        tempTracki.erase(tempTracki.begin()+position);
    }
    else{
        if(swapPo>position){
            tempTracki.insert(tempTracki.begin()+swapPo+1,temp2);
            tempTracki.insert(tempTracki.begin()+swapPo+1,temp);
            tempTracki.erase(tempTracki.begin()+position);
            tempTracki.erase(tempTracki.begin()+position);
        }else{
            tempTracki.erase(tempTracki.begin()+position);
            tempTracki.erase(tempTracki.begin()+position);
            tempTracki.insert(tempTracki.begin()+swapPo,temp2);
            tempTracki.insert(tempTracki.begin()+swapPo,temp);
        }
    }
    pair<double,double>iResult=TrackResult(tempTracki,iTrack);
    pair<double,double>kResult=TrackResult(tempTrackk,kTrack);
    if(iResult.first==0||kResult.first==0){
        return make_pair(0,0);
    }
    if(iTrack!=kTrack) return make_pair(max(iResult.first,kResult.first),iResult.second+kResult.second);
    else return make_pair(iResult.first,iResult.second);
}
pair<double,double> swapTabu21(vector<vector<int>> Tracks,int iTrack,int position,int kTrack,int swapPo){
    vector<int> tempTracki=Tracks[iTrack];
    vector<int> tempTrackk=Tracks[kTrack];
    int temp=tempTracki[position];
    int temp2=tempTracki[position+1];
    int temp3=tempTrackk[swapPo];
    if(iTrack!=kTrack){
        tempTracki[position]=temp3;
        tempTrackk[swapPo]=temp;
        tempTrackk.insert(tempTrackk.begin()+swapPo+1,temp2);
        tempTracki.erase(tempTracki.begin()+position+1);
    } else{
        tempTracki[position]=temp3;
        tempTracki[swapPo]=temp;
        if(swapPo>position){
            tempTracki.insert(tempTracki.begin()+swapPo+1,temp2);
            tempTracki.erase(tempTracki.begin()+position+1);
        }
        else{
            tempTracki.erase(tempTracki.begin()+position+1);
            tempTracki.insert(tempTracki.begin()+swapPo+1,temp2);
        }
    }
    pair<double,double>iResult=TrackResult(tempTracki,iTrack);
    pair<double,double>kResult=TrackResult(tempTrackk,kTrack);
    if(iResult.first==0||kResult.first==0){
        return make_pair(0,0);
    }
    if(iTrack!=kTrack) return make_pair(max(iResult.first,kResult.first),iResult.second+kResult.second);
    else return iResult;
}
pair<double,double> swapTabu10(vector<vector<int>> Tracks,int iTrack,int position,int kTrack,int swapPo){
    vector<int> tempTracki=Tracks[iTrack];
    vector<int> tempTrackk=Tracks[kTrack];
    int temp=tempTracki[position];
    if(iTrack>kTrack){
        tempTrackk.insert(tempTrackk.begin()+swapPo,temp);
        tempTracki.erase(tempTracki.begin()+position);
    }
    else if(iTrack<kTrack){
        tempTrackk.insert(tempTrackk.begin()+swapPo+1,temp);
        tempTracki.erase(tempTracki.begin()+position);
    }
    else{
        if(swapPo>position){
            tempTracki.insert(tempTracki.begin()+swapPo+1,temp);
            tempTracki.erase(tempTracki.begin()+position);
        }else{
            tempTracki.erase(tempTracki.begin()+position);
            tempTracki.insert(tempTracki.begin()+swapPo,temp);
        }
    }
    pair<double,double>iResult=TrackResult(tempTracki,iTrack);
    pair<double,double>kResult=TrackResult(tempTrackk,kTrack);
    if(iResult.first==0||kResult.first==0){
        return make_pair(0,0);
    }
    if(iTrack!=kTrack) return make_pair(max(iResult.first,kResult.first),iResult.second+kResult.second);
    else return iResult;
}
pair<double,double> swapTabu11(vector<vector<int>> Tracks,int iTrack,int position,int kTrack,int swapPo){
    vector<int> tempTracki=Tracks[iTrack];
    vector<int> tempTrackk=Tracks[kTrack];
    int temp=tempTracki[position];
    int temp2=tempTrackk[swapPo];
    if(iTrack!=kTrack){
        tempTracki[position]=temp2;
        tempTrackk[swapPo]=temp;
    } else{
        tempTracki[position]=temp2;
        tempTracki[swapPo]=temp;
    }
    pair<double,double>iResult=TrackResult(tempTracki,iTrack);
    pair<double,double>kResult=TrackResult(tempTrackk,kTrack);
    if(iResult.first==0||kResult.first==0){
        return make_pair(0,0);
    }
    if(iTrack!=kTrack) return make_pair(max(iResult.first,kResult.first),iResult.second+kResult.second);
    else return iResult;
}
pair<double,double> swapTabu(vector<vector<int>> Tracks,int iTrack,int position,int kTrack,int swapPo,int type){
    if(type==0) return swapTabu20(Tracks,iTrack,position,kTrack,swapPo);
    else if(type==1) return swapTabu21(Tracks,iTrack,position,kTrack,swapPo);
    else if(type==2) return swapTabu10(Tracks,iTrack,position,kTrack,swapPo);
    else return swapTabu11(Tracks,iTrack,position,kTrack,swapPo);
}

bool checkswaptabu20(vector<int> temp,vector<vector<int>>Tracks,vector<int>endpoint, int Tracknum,int position,int swapTrack,int swapPo){
    if(position==Tracks[Tracknum].size()-1)return false;
    if(swapTrack>=num_trucks&&customers[Tracks[Tracknum][position]].OnlyByTruck==1)return false;
    if(swapTrack>=num_trucks&&customers[Tracks[Tracknum][position+1]].OnlyByTruck==1)return false;
    if(Tracks[Tracknum][position]>num_cus) return false;
    if(Tracks[Tracknum][position+1]>num_cus) return false;
    int s2=temp[endpoint[Tracknum]+1+position+1];
    int s1=temp[endpoint[Tracknum]+1+position];
    int pos1=endpoint[Tracknum]+1+position;
    int pos2=endpoint[swapTrack]+1+swapPo;
    if(pos2==pos1||pos2==pos1+1) return false;
    else if(pos2>pos1+1){
        temp.insert(temp.begin()+endpoint[swapTrack]+swapPo+1,s2);
        temp.insert(temp.begin()+endpoint[swapTrack]+swapPo+1,s1);
        temp.erase(temp.begin()+endpoint[Tracknum]+1+position);
        temp.erase(temp.begin()+endpoint[Tracknum]+1+position);
    }else{
        temp.erase(temp.begin()+endpoint[Tracknum]+1+position);
        temp.erase(temp.begin()+endpoint[Tracknum]+1+position);
        temp.insert(temp.begin()+endpoint[swapTrack]+swapPo+1,s2);
        temp.insert(temp.begin()+endpoint[swapTrack]+swapPo+1,s1); 
    }
   
    
    if(!checkroute(temp)) return false;
    return true;
}
bool checkswaptabu21(vector<int> temp,vector<vector<int>>Tracks,vector<int>endpoint, int Tracknum,int position,int swapTrack,int swapPo){
    if(position==Tracks[Tracknum].size()-1)return false;
    if(swapTrack>=num_trucks&&customers[Tracks[Tracknum][position]].OnlyByTruck==1)return false;
    if(swapTrack>=num_trucks&&customers[Tracks[Tracknum][position+1]].OnlyByTruck==1)return false;
    if(Tracknum>=num_trucks&&customers[Tracks[swapTrack][swapPo]].OnlyByTruck==1)return false;
    if(Tracks[Tracknum][position]>num_cus) return false;
    if(Tracks[Tracknum][position+1]>num_cus) return false;
    if(Tracks[swapTrack][swapPo]>num_cus) return false;
    int s=temp[endpoint[Tracknum]+1+position+1];
    int pos1=endpoint[Tracknum]+1+position;
    int pos2=endpoint[swapTrack]+1+swapPo;
    if(pos1==pos2||pos1+1==pos2)return false;
    swap(temp[pos1],temp[pos2]);
    if(pos1>pos2){
        temp.erase(temp.begin()+pos1+1);
        temp.insert(temp.begin()+pos2+1,s);
    }
    else{
        temp.insert(temp.begin()+pos2+1,s);
        temp.erase(temp.begin()+pos1+1);
    }
    if(!checkroute(temp)) return false;
    return true;
}
bool checkswaptabu10(vector<int> temp,vector<vector<int>>Tracks,vector<int> endpoint, int Tracknum,int position,int swapTrack,int swapPo){
    if(Tracks[Tracknum][position]>num_cus) return false;
    if(swapTrack>=num_trucks&&customers[Tracks[Tracknum][position]].OnlyByTruck==1)return false;
    int pos1=endpoint[Tracknum]+1+position;
    int pos2=endpoint[swapTrack]+1+swapPo;
    int s=temp[pos1];
    if(pos2==pos1) return false;
    else if(pos2>pos1){
        temp.insert(temp.begin()+pos2+1,s);
        temp.erase(temp.begin()+pos1);
    }else{
        temp.erase(temp.begin()+pos1);
        temp.insert(temp.begin()+pos2,s);
 
    }
    if(!checkroute(temp)) return false;

    return true;
}
bool checkswaptabu11(vector<int> temp,vector<vector<int>>Tracks,vector<int> endpoint, int Tracknum,int position,int swapTrack,int swapPo){
    if(swapTrack>=num_trucks&&customers[Tracks[Tracknum][position]].OnlyByTruck==1)return false;
    if(Tracknum>=num_trucks&&customers[Tracks[swapTrack][swapPo]].OnlyByTruck==1)return false;
    if(Tracks[Tracknum][position]>num_cus) return false;
    if(Tracks[swapTrack][swapPo]>num_cus) return false;
    int pos1=endpoint[Tracknum]+1+position;
    int pos2=endpoint[swapTrack]+1+swapPo;
    swap(temp[pos1],temp[pos2]);
    if(!checkroute(temp)) return false;

    return true;
}
bool checkswaptabu(vector<int> temp,vector<vector<int>>Tracks,vector<int>endpoint, int Tracknum,int position,int swapTrack,int swapPo,int type){
    if(Tracks[Tracknum].size()==0)return false;
    vector<int> temp1=temp;
    if(type==0) return checkswaptabu20(temp1,Tracks,endpoint,Tracknum,position,swapTrack,swapPo);
    else if(type==1) return checkswaptabu21(temp1,Tracks,endpoint,Tracknum,position,swapTrack,swapPo);
    else if(type==2) return checkswaptabu10(temp1,Tracks,endpoint,Tracknum,position,swapTrack,swapPo);
    else return checkswaptabu11(temp1,Tracks,endpoint,Tracknum,position,swapTrack,swapPo);
}
vector<int> swap20(vector<int> temp,vector<int>endpoint,int Tracknum,int position,int swapTrack,int swapPo){
    int s2=temp[endpoint[Tracknum]+1+position+1];
    int s1=temp[endpoint[Tracknum]+1+position];
    int pos1=endpoint[Tracknum]+1+position;
    int pos2=endpoint[swapTrack]+1+swapPo;
    if(pos2>pos1+1){
        temp.insert(temp.begin()+pos2+1,s2);
        temp.insert(temp.begin()+pos2+1,s1);
        temp.erase(temp.begin()+pos1);
        temp.erase(temp.begin()+pos1);
    }else{
        temp.erase(temp.begin()+pos1);
        temp.erase(temp.begin()+pos1);
        temp.insert(temp.begin()+pos2,s2);
        temp.insert(temp.begin()+pos2,s1); 
    }
    return temp;
}
vector<int> swap21(vector<int> temp,vector<int>endpoint,int Tracknum,int position,int swapTrack,int swapPo){
    int s=temp[endpoint[Tracknum]+1+position+1];
    int pos1=endpoint[Tracknum]+1+position;
    int pos2=endpoint[swapTrack]+1+swapPo;
    swap(temp[pos1],temp[pos2]);
    if(pos1>pos2){
        temp.erase(temp.begin()+pos1+1);
        temp.insert(temp.begin()+pos2+1,s);
    }
    else{
        temp.insert(temp.begin()+pos2+1,s);
        temp.erase(temp.begin()+pos1+1);
    }
    return temp;
}
vector<int> swap10TB(vector<int> temp,vector<int>endpoint,int Tracknum,int position,int swapTrack,int swapPo){
    int pos1=endpoint[Tracknum]+1+position;
    int pos2=endpoint[swapTrack]+1+swapPo;
    int s=temp[pos1];
    if(pos2>pos1){
        temp.insert(temp.begin()+pos2+1,s);
        temp.erase(temp.begin()+pos1);
    }else{
        temp.erase(temp.begin()+pos1);
        temp.insert(temp.begin()+pos2,s);
    }
    return temp;
}
vector<int> swap11TB(vector<int> temp,vector<int>endpoint,int Tracknum,int position,int swapTrack,int swapPo){
    int pos1=endpoint[Tracknum]+1+position;
    int pos2=endpoint[swapTrack]+1+swapPo;
    swap(temp[pos1],temp[pos2]);
    return temp;
}
vector<Individual> tabu_search( const vector<int> &initial_solution,int max_iterations)
{ // Added const qualifier
	vector<int> current_solution = initial_solution;
	vector<MoveRecord> tabu_list[4];
    vector<Individual>Tabupareto;
    double best_fit1=calculatefitness(initial_solution).first;
    double best_fit2=calculatefitness(initial_solution).second;
    Tabupareto.clear();
    int noimprovecount=0;
    std::ofstream outputFile;
    outputFile.open(outputtblog, std::ios::app);
	for (int iter = 0; iter < max_iterations; iter++) {

        outputFile<<"Iteration "<<iter<<":"<<endl;
        outputFile<<"Current solution:";
        for(int z=0;z<total_node;z++){
            outputFile<<current_solution[z]<<" ";
        }
        outputFile<<endl;
        auto start = std::chrono::high_resolution_clock::now();
        vector<Individual> neighbors;
        neighbors.clear();
        vector<TabuRecord> taburecord;
        int type=rand()%4;
        pair<double,double> fitbefore=calculatefitness(current_solution);
        vector<vector<int>> Tracks=splitTracks(current_solution).first;
        vector<int> endpoint=splitTracks(current_solution).second;
        vector<double> obj1List;
        vector<double> obj2List;
        obj1List.clear();
        obj2List.clear();
        for(int j=0;j<num_trucks+num_drones;j++){
            pair<double,double> oriTrackres=TrackResult(Tracks[j],j);
            obj1List.push_back(oriTrackres.first);
            obj2List.push_back(oriTrackres.second);    
        }

        for(int i=0;i<num_drones+num_trucks;i++){
            for(int j=0;j<Tracks[i].size();j++){
                for(int k=0;k<num_drones+num_trucks;k++){
                    for(int l=0;l<Tracks[k].size();l++){
                        //cout<<"t1"<<endl;
                        vector<int> temp=current_solution;
                        pair<double,double> fitafter;
                        pair<double,double> fitchange;
                        fitchange.first=0;
                        fitchange.second=0;
                        //cout<<"t0"<<endl;
                        if(checkswaptabu(temp,Tracks,endpoint,i,j,k,l,type)){
                            fitchange=swapTabu(Tracks,i,j,k,l,type);
                            if(fitchange.first!=0&&fitchange.second!=0){
                                if(i!=k) fitafter.second=fitbefore.second+fitchange.second-obj2List[i]-obj2List[k];
                                else fitafter.second=fitbefore.second+fitchange.second-obj2List[i];
                                vector<double> obj1new;
                                obj1new.clear();
                                for(int t=0;t<obj1List.size();t++){
                                    if(t!=i&&t!=k)obj1new.push_back(obj1List[t]);
                                }
                                obj1new.push_back(fitchange.first);
                                sort(obj1new.begin(),obj1new.end());
                                fitafter.first=obj1new.back();
                                if(fitbefore.first-fitafter.first>1e-3||fitbefore.second-fitafter.second>1e-3){
                                    Individual newsol;
                                    if(type==0) temp=swap20(temp,endpoint,i,j,k,l);
                                    else if (type==1) temp=swap21(temp,endpoint,i,j,k,l);
                                    else if (type==2) temp=swap10TB(temp,endpoint,i,j,k,l);
                                    else temp=swap11TB(temp,endpoint,i,j,k,l);
                                    newsol.route=temp;
                                    pair<double,double> restest=calculatefitness(temp);
                                    newsol.fitness1=fitafter.first;
                                    newsol.fitness2=fitafter.second;
                                    newsol.tabusearch=0;
                                    newsol.localsearch=0;
                                    neighbors.push_back(newsol);
                                    TabuRecord newrecord;
                                    newrecord.indi=newsol;
                                    newrecord.record.start[0]=Tracks[i][j];
                                    if(type<2)newrecord.record.start[1]=Tracks[i][j+1];
                                    else newrecord.record.start[1]=-1;
                                    newrecord.record.end=Tracks[k][l];
                                    int pos1=endpoint[i]+1+j;
                                    int pos2=endpoint[k]+1+l;
                                    if(type==0||type==2){
                                        if(pos1>0)newrecord.record.front[0]=current_solution[pos1-1];
                                        else newrecord.record.front[0]=-1;
                                        if(type==2){
                                            if(pos1<total_node-1)newrecord.record.back[0]=current_solution[pos1+1];
                                            else newrecord.record.back[0]=-1;
                                        }else{
                                            if(pos1<total_node-2)newrecord.record.back[0]=current_solution[pos1+2];
                                            else newrecord.record.back[0]=-1;
                                        }
                                        if(pos1>pos2){
                                            newrecord.record.back[1]=current_solution[pos2];
                                            if(pos2>0)newrecord.record.front[1]=current_solution[pos2-1];
                                            else newrecord.record.front[1]=-1;
                                        }
                                        else{
                                            newrecord.record.front[1]=current_solution[pos2];
                                            if(pos2<total_node-1)newrecord.record.back[1]=current_solution[pos2+1];
                                            else newrecord.record.back[1]=-1;
                                        }
                                    }
                                    taburecord.push_back(newrecord);
                                }
                            }
                        }
                        //cout<<"t2"<<endl;
                    }
                }
            }        
        }
        
        vector<vector<int>> front=fast_non_dominated_sort(neighbors);
        bool check=false;
        int cnt=0;
        if(neighbors.size()==0) noimprovecount++;
        else {
            for(int j=0;j<front[0].size();j++){
                if(!inpareto(neighbors[front[0][j]],Tabupareto)){
                    updatepareto(neighbors[front[0][j]],Tabupareto);
                }   
            }
            noimprovecount=0;   
            while(!check&&cnt<20){
                check=true;
                int select=rand()%(front[0].size());
                if(best_fit1-neighbors[front[0][select]].fitness1>1e-3&&best_fit2-neighbors[front[0][select]].fitness2>1e-3){
                    current_solution=neighbors[front[0][select]].route;
                    best_fit1=neighbors[front[0][select]].fitness1;
                    best_fit2=neighbors[front[0][select]].fitness2;

                    if(!in_tabu_list(tabu_list[type],taburecord[front[0][select]].record,type)){
                        tabu_list[type].push_back(taburecord[front[0][select]].record);
                    }
                    outputFile<<"New Solution: ";
                    for(int z=0;z<total_node;z++){
                        outputFile<<current_solution[z]<<" ";
                    }
                    outputFile<<endl;
                    outputFile<<"Move:"<<taburecord[front[0][select]].record.start[0]<<" "<<taburecord[front[0][select]].record.end<<endl;
                    
                }   
                else{
                    
                    if(!in_tabu_list(tabu_list[type],taburecord[front[0][select]].record,type)){

                        tabu_list[type].push_back(taburecord[front[0][select]].record);

                        current_solution=neighbors[front[0][select]].route;
                        outputFile<<"New Solution: ";
                        for(int z=0;z<total_node;z++){
                            outputFile<<current_solution[z]<<" ";
                        }
                        outputFile<<endl;
                        outputFile<<"Move:"<<taburecord[front[0][select]].record.start[0]<<" "<<taburecord[front[0][select]].record.end<<endl;
                    }
                    else{check=false;cnt++;}
                }

            }
        if(tabu_list[type].size()>5)tabu_list[type].erase(tabu_list[type].begin());
        }

        outputFile<<"Type:"<<type<<endl;
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = end - start;
        outputFile<<"Time:"<<duration.count()<<"ms"<<endl;

        //if(noimprovecount>15)return Tabupareto;
	}
   // cout<<Tabupareto.size()<<endl;
	return Tabupareto;
}
vector<Individual> tabu_search2( const vector<int> &initial_solution,int max_iterations)
{ // Added const qualifier
    time_t starttb,endtb;
    time(&starttb);
    int end_iter=0;
	vector<int> current_solution = initial_solution;
	vector<MoveRecord> tabu_list[4];
    vector<Individual>Tabupareto;
    double best_fit1=calculatefitness(initial_solution).first;
    double best_fit2=calculatefitness(initial_solution).second;
    Tabupareto.clear();
    int noimprovecount=0;
    std::ofstream outputFile;
    outputFile.open(outputtblog, std::ios::app);
	for (int iter = 0; iter < max_iterations; iter++) {

        outputFile<<"Iteration "<<iter<<":"<<endl;
        outputFile<<"Current solution:";
        for(int z=0;z<total_node;z++){
            outputFile<<current_solution[z]<<" ";
        }
        outputFile<<endl;
        auto start = std::chrono::high_resolution_clock::now();
        vector<Individual> neighbors;
        neighbors.clear();
        vector<TabuRecord> taburecord;
        int type=rand()%4;
        pair<double,double> fitbefore=calculatefitness(current_solution);
        vector<vector<int>> Tracks=splitTracks(current_solution).first;
        vector<int> endpoint=splitTracks(current_solution).second;
        vector<double> obj1List;
        vector<double> obj2List;
        obj1List.clear();
        obj2List.clear();
        for(int j=0;j<num_trucks+num_drones;j++){
            pair<double,double> oriTrackres=TrackResult(Tracks[j],j);
            obj1List.push_back(oriTrackres.first);
            obj2List.push_back(oriTrackres.second);    
        }

        for(int i=0;i<num_drones+num_trucks;i++){
            for(int j=0;j<Tracks[i].size();j++){
                for(int k=0;k<num_drones+num_trucks;k++){
                    for(int l=0;l<Tracks[k].size();l++){
                        //cout<<"t1"<<endl;
                        vector<int> temp=current_solution;
                        pair<double,double> fitafter;
                        pair<double,double> fitchange;
                        fitchange.first=0;
                        fitchange.second=0;
                        //cout<<"t0"<<endl;
                        if(checkswaptabu(temp,Tracks,endpoint,i,j,k,l,type)){
                            fitchange=swapTabu(Tracks,i,j,k,l,type);
                            if(fitchange.first!=0&&fitchange.second!=0){
                                if(i!=k) fitafter.second=fitbefore.second+fitchange.second-obj2List[i]-obj2List[k];
                                else fitafter.second=fitbefore.second+fitchange.second-obj2List[i];
                                vector<double> obj1new;
                                obj1new.clear();
                                for(int t=0;t<obj1List.size();t++){
                                    if(t!=i&&t!=k)obj1new.push_back(obj1List[t]);
                                }
                                obj1new.push_back(fitchange.first);
                                sort(obj1new.begin(),obj1new.end());
                                fitafter.first=obj1new.back();
                                if(fitbefore.first-fitafter.first>1e-3||fitbefore.second-fitafter.second>1e-3){
                                    Individual newsol;
                                    if(type==0) temp=swap20(temp,endpoint,i,j,k,l);
                                    else if (type==1) temp=swap21(temp,endpoint,i,j,k,l);
                                    else if (type==2) temp=swap10TB(temp,endpoint,i,j,k,l);
                                    else temp=swap11TB(temp,endpoint,i,j,k,l);
                                    newsol.route=temp;
                                    pair<double,double> restest=calculatefitness(temp);
                                    newsol.fitness1=fitafter.first;
                                    newsol.fitness2=fitafter.second;
                                    newsol.tabusearch=0;
                                    newsol.localsearch=0;
                                    neighbors.push_back(newsol);
                                    TabuRecord newrecord;
                                    newrecord.indi=newsol;
                                    newrecord.record.start[0]=Tracks[i][j];
                                    if(type<2)newrecord.record.start[1]=Tracks[i][j+1];
                                    else newrecord.record.start[1]=-1;
                                    newrecord.record.end=Tracks[k][l];
                                    int pos1=endpoint[i]+1+j;
                                    int pos2=endpoint[k]+1+l;
                                    if(type==0||type==2){
                                        if(pos1>0)newrecord.record.front[0]=current_solution[pos1-1];
                                        else newrecord.record.front[0]=-1;
                                        if(type==2){
                                            if(pos1<total_node-1)newrecord.record.back[0]=current_solution[pos1+1];
                                            else newrecord.record.back[0]=-1;
                                        }else{
                                            if(pos1<total_node-2)newrecord.record.back[0]=current_solution[pos1+2];
                                            else newrecord.record.back[0]=-1;
                                        }
                                        if(pos1>pos2){
                                            newrecord.record.back[1]=current_solution[pos2];
                                            if(pos2>0)newrecord.record.front[1]=current_solution[pos2-1];
                                            else newrecord.record.front[1]=-1;
                                        }
                                        else{
                                            newrecord.record.front[1]=current_solution[pos2];
                                            if(pos2<total_node-1)newrecord.record.back[1]=current_solution[pos2+1];
                                            else newrecord.record.back[1]=-1;
                                        }
                                    }
                                    taburecord.push_back(newrecord);
                                }
                            }
                        }
                        //cout<<"t2"<<endl;
                    }
                }
            }        
        }
        
        vector<vector<int>> front=fast_non_dominated_sort(neighbors);
        bool check=false;
        int cnt=0;
        if(neighbors.size()==0) noimprovecount++;
        else {
            bool update=false;
            for(int j=0;j<front[0].size();j++){
                if(!inpareto(neighbors[front[0][j]],Tabupareto)){
                    update=true;
                    updatepareto(neighbors[front[0][j]],Tabupareto);
                }   
            }
            if(update)noimprovecount=0;
            else noimprovecount++;   
            while(!check&&cnt<20){
                check=true;
                int select=rand()%(front[0].size());
                if(best_fit1-neighbors[front[0][select]].fitness1>1e-3&&best_fit2-neighbors[front[0][select]].fitness2>1e-3){
                    current_solution=neighbors[front[0][select]].route;
                    best_fit1=neighbors[front[0][select]].fitness1;
                    best_fit2=neighbors[front[0][select]].fitness2;

                    if(!in_tabu_list(tabu_list[type],taburecord[front[0][select]].record,type)){
                        tabu_list[type].push_back(taburecord[front[0][select]].record);
                    }
                    outputFile<<"New Solution: ";
                    for(int z=0;z<total_node;z++){
                        outputFile<<current_solution[z]<<" ";
                    }
                    outputFile<<endl;
                    outputFile<<"Move:"<<taburecord[front[0][select]].record.start[0]<<" "<<taburecord[front[0][select]].record.end<<endl;
                    
                }   
                else{
                    
                    if(!in_tabu_list(tabu_list[type],taburecord[front[0][select]].record,type)){

                        tabu_list[type].push_back(taburecord[front[0][select]].record);

                        current_solution=neighbors[front[0][select]].route;
                        outputFile<<"New Solution: ";
                        for(int z=0;z<total_node;z++){
                            outputFile<<current_solution[z]<<" ";
                        }
                        outputFile<<endl;
                        outputFile<<"Move:"<<taburecord[front[0][select]].record.start[0]<<" "<<taburecord[front[0][select]].record.end<<endl;
                    }
                    else{check=false;cnt++;}
                }

            }
        if(tabu_list[type].size()>5)tabu_list[type].erase(tabu_list[type].begin());
        }

        outputFile<<"Type:"<<type<<endl;
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = end - start;
        outputFile<<"Time:"<<duration.count()<<"ms"<<endl;
        //if(noimprovecount>100){end_iter=iter+1;break;}
        time(&endtb);
        if(double(endtb-starttb)>timeLimit){end_iter=iter+1;break;}
        if(iter==max_iterations-1)end_iter=max_iterations;
	}
    time(&endtb);
    output(Tabupareto,double(endtb-starttb),end_iter,end_iter,end_iter);
   // cout<<Tabupareto.size()<<endl;
	return Tabupareto;
}