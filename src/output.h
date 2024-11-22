#include <fstream>
#include <iostream>
#include "parameter.h"
using namespace std;
extern string outputfilename;
extern string outputparetofilename;
extern string outputparetoselectionfilename;
extern string outputlog;
extern string outputgraph;
extern int total_node;
extern int drone_max_tracks;
void outputGraphdata(double obj1,double obj2, int gen){
    std::ofstream outputFile;
    outputFile.open(outputgraph, std::ios::app);
    outputFile<<gen+1<<" "<<obj1<<" "<<obj2<<endl;
    outputFile.close();
}
void outputLog(int gen,vector<Individual> pareto){
    std::ofstream outputFile;
    outputFile.open(outputlog, std::ios::app);
     // Open the file for writing
     outputFile<<"Generation "<<gen<<": "<<pareto.size()<<" sols"<<endl;
    for(int i=0;i<pareto.size();i++){
        for(int j=0;j<total_node;j++){
            outputFile<<pareto[i].route[j]<<" ";
        }

        outputFile<<endl << pareto[i].fitness1 <<" "<<pareto[i].fitness2<< endl;
    }
    outputFile<<endl;
    outputFile.close();
}
void output( vector<Individual> pareto,double time,int end_iter){
    std::ofstream outputFile;
    outputFile.open(outputfilename, std::ios::app); // Open the file for writing
    outputFile<<"Time:"<<time<<endl;
    outputFile<<"Last Iter:"<<end_iter<<endl;
    outputFile<<pareto.size()<<endl;
    for(int i=0;i<pareto.size();i++){
        for(int j=0;j<total_node;j++){
            if(pareto[i].route[j]==0)outputFile<<"||";
            else if(pareto[i].route[j]>num_cus){
                if(pareto[i].route[j-1]==num_cus)outputFile<<"|";
                if(pareto[i].route[j]-pareto[i].route[j-1]!=1&&pareto[i].route[j-1]!=0&&j!=total_node-1){
                    outputFile<<"|";
                }
            }
            else outputFile<<pareto[i].route[j]<<" ";
        }
        outputFile<<endl;
        outputFile << pareto[i].fitness1 <<" "<<pareto[i].fitness2<< endl;
    }
    outputFile<<endl;
    outputFile.close();
}
void outputpareto( vector<Individual> pareto){
    std::ofstream outputFile(outputparetofilename); // Open the file for writing
    for(int i=0;i<pareto.size();i++){
        outputFile <<"Solution "<<i+1<<":"<<endl;
        vector<int>route=pareto[i].route;
        int numdrone=1;
        outputFile<<"Trucks:"<<endl;
        for(int i=0;i<route.size();i++){
            if(route[i]>num_cus){
                if(route[i]>num_cus+num_trucks){
                    int a=route[i]-num_cus-num_trucks+1;
                    if(a%drone_max_tracks==0){
                        if(route[i]-route[i-1]!=1)outputFile<<endl;
                        numdrone++;
                        outputFile<<"Drone "<<numdrone<<":"<<endl;
                    } 
                    else {
                        if(route[i]-route[i-1]!=1) outputFile<<endl;
                    }

                }
                else {
                    outputFile<<endl;
                }
                
            }
            else if(route[i]==0){
                outputFile<<endl;
                outputFile<<"Drone 1:"<<endl;
            }
            else outputFile<<route[i]<<" ";
        }
        outputFile<<endl;
        for(int i=0;i<total_node;i++){
            outputFile<<route[i]<<", ";
        }
        outputFile<<endl;
        outputFile << pareto[i].fitness1 <<" "<<pareto[i].fitness2<< endl<<endl;
        
    }
    outputFile.close();
}
void outputParetoResult(vector<ParetoRecord> paretoRecords,int k){
    std::ofstream outputFile;
    outputFile.open(outputparetoselectionfilename,std::ios::app); 
    int limit=paretoRecords.size()>5?5:paretoRecords.size();
    if(k!=0){outputFile<<"Criteria "+to_string(k)+": "+to_string(0.25*k)+" fitness 1 and "+to_string(0.25*(3-k))+" fitness2"<<endl;}
    else {
        outputFile<<"General Ranking:"<<endl;
    }
    for(int i=0;i<limit;i++){
        for(int j=0;j<total_node;j++){
            if(paretoRecords[i].indi.route[j]==0)outputFile<<"||";
            else if(paretoRecords[i].indi.route[j]>num_cus){
                if(paretoRecords[i].indi.route[j-1]==num_cus)outputFile<<"|";
                if(paretoRecords[i].indi.route[j]-paretoRecords[i].indi.route[j-1]!=1&&paretoRecords[i].indi.route[j-1]!=0&&j!=total_node-1){
                    outputFile<<"|";
                }
            }
            else outputFile<<paretoRecords[i].indi.route[j]<<" ";
        }
        outputFile<<endl;
        outputFile << paretoRecords[i].indi.fitness1 <<" "<<paretoRecords[i].indi.fitness2<< endl;
    }
}