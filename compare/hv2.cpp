#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>
#include <ctime>
#include<sstream>
#include <string>
#include <unistd.h>

using namespace std;
string inputfile="checkHV.txt";
double referencePoint[24][2]={{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
                                {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
                                {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}};
double best[24][2]={{1000000000,10000000000},{1000000000,10000000000},{1000000000,10000000000},{1000000000,10000000000},{1000000000,10000000000},{1000000000,10000000000},{1000000000,10000000000},{1000000000,10000000000},
                    {1000000000,10000000000},{1000000000,10000000000},{1000000000,10000000000},{1000000000,10000000000},{1000000000,10000000000},{1000000000,10000000000},{1000000000,10000000000},{1000000000,10000000000},
                    {1000000000,10000000000},{1000000000,10000000000},{1000000000,10000000000},{1000000000,10000000000},{1000000000,10000000000},{1000000000,10000000000},{1000000000,10000000000},{1000000000,10000000000}};
double nobj1[24];
double nobj2[24];
std::vector<std::string> splitString(const std::string& str, char delimiter) {
    std::vector<std::string> parts;
    std::stringstream ss(str);
    std::string part;
    
    while (std::getline(ss, part, delimiter)) {
        parts.push_back(part);
    }
    
    return parts;
}
double calculateHypervolume(const vector<vector<double>>& points,int bo) {
    // Sort the points based on the first objective value in ascending order

    double hypervolume = 0.0;
    double obj2 = referencePoint[bo][1];
    double obj1 = referencePoint[bo][0];

    for (int i=0;i<points.size();i++) {
        double height = (obj2-points[i][1])/nobj2[bo];
        double width=(obj1-points[i][0])/nobj1[bo];
        hypervolume += width *height;
        obj2= points[i][1];
    }

    // Add the remaining volume up to the reference point

    return hypervolume;
}
int checkdomination(vector<double> dominater,vector<double> dominated){
    bool ch = false;
    if(dominater[0]-dominated[0]>1e-9) 
        return false;
    else if(dominated[0]-dominater[0]>1e-3) ch=true;
    if(dominater[1]-dominated[1]>1e-3) return false;
    else if(dominated[1]-dominater[1]>1e-3) ch=true;
    return ch;
}
bool dominatedinpareto(vector<double> indi,vector<vector<double>>pareto){
    if(pareto.size()==0)return false;
    for(int i=0;i<pareto.size();i++){
        if(checkdomination(pareto[i],indi))return true;
    }
    return false;
}
bool comparefit1(vector<double>  indi1,vector<double> indi2){
    return indi1[0]<indi2[0];
}
void updatepareto(vector<double> indi,vector<vector<double>> pareto){
    vector<int> dominated;
    if(!dominatedinpareto(indi,pareto)){
        for(int i=0;i<pareto.size();i++){
            if(checkdomination(indi,pareto[i]))dominated.push_back(i);
        }
        sort(dominated.begin(),dominated.end());
        for(int i=0;i<dominated.size();i++){
            pareto.erase(pareto.begin()+dominated[i]-i);

        }
        pareto.push_back(indi);
    }
    
}
bool inpareto(vector<double> indi,vector<vector<double>> pareto){
    for(int i=0;i<pareto.size();i++){
        if(abs(indi[0]-pareto[i][0])<1e-4&&abs(indi[1]-pareto[i][1])<1e-4) return true;

    }
    return false;
}
double calculateCoverage(vector<vector<double>>res , vector<vector<double>> pareto){
    int dominatedcount=0;
    int dominatecount=0;
    int neutral=0;
    for(int i=0;i<res.size();i++){
        if(dominatedinpareto(res[i],pareto))dominatedcount++;
        else{
            int cou=0;
            for(int j=0;j<pareto.size();j++){
                if(checkdomination(res[i],pareto[j]))cou++;
            }
            if(cou>0)dominatecount++;
            else neutral++;
        }
    }
    cout<<dominatecount<<" "<<dominatedcount<<" "<<neutral<<" ";
    return dominatedcount/res.size();
}

int main() {
    // Define the reference point (ideal or utopian solution)
     // Modify this according to your problem

    // Define the set of solutions
    ifstream inputFile(inputfile);
    string line;
    vector<vector<double>>res[6][24];
    vector<vector<double>> pareto[24];
    int dominatedcount=0;
    int dominatecount=0;
    int neutral=0;
    for(int t=0;t<6;t++){
        getline(inputFile,line);
        for(int bo=0;bo<24;bo++){
            getline(inputFile,line);
            vector<string> sols=splitString(line,';');
            for(int i=0;i<sols.size();i++){
                vector<string>objectives=splitString(sols[i],',');
                double fitness1=0;
                double fitness2=0;
                if(i==0) fitness1=stod(objectives[0].substr(1));
                else fitness1=stod(objectives[0].substr(2));
                istringstream iss(objectives[1]);
                float value;
                while (iss >> value){
                    fitness2=value;
                }
                if(fitness1>referencePoint[bo][0])referencePoint[bo][0]=fitness1;
                if(fitness2>referencePoint[bo][1])referencePoint[bo][1]=fitness2;
                if(fitness1<best[bo][0])best[bo][0]=fitness1;
                if(fitness2<best[bo][1])best[bo][1]=fitness2;
                res[t][bo].push_back({fitness1,fitness2});
                if(!inpareto(res[t][bo][i],pareto[bo])){
                    updatepareto(res[t][bo][i],pareto[bo]);
                }   
            }
        }
    }
    for(int bo=0;bo<24;bo++){
    nobj1[bo]=referencePoint[bo][0]-best[bo][0];
    nobj2[bo]=referencePoint[bo][1]-best[bo][1];
    //cout<<referencePoint[bo][0]<<" "<<best[bo][0]<<" "<<referencePoint[bo][1]<<" "<<best[bo][1]<<" "<<nobj1[bo]<<" "<<nobj2[bo]<<endl;
    }
    for(int t=0;t<6;t++){
        cout<<t<<endl;
        for(int bo=0;bo<24;bo++){
            cout<<calculateHypervolume(res[t][bo],bo)<<endl;
        }
    }
    for(int t=0;t<0;t++){
        //cout<<t<<endl;
        for(int bo=0;bo<24;bo++){
           // cout<<calculateCoverage(res[t][bo],pareto[bo])<<endl;
        }
    }
    return 0;
}