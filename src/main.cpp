#include "NSGAII.h"
#include "input.h"
#include "distance.h"
#include <string>
#include <ctime>
using namespace std;
extern string problem;
extern string resultsrc;
extern string outputfilename;
extern string inputfile;
extern int total_node;
extern int canuseddrone;
extern int drone_max_tracks;
extern double maxobj1;
extern double maxobj2;
extern double minobj1;
extern double minobj2;
extern double obj1norm;
extern double obj2norm;
extern string testcount;
extern int crossoverMod;
extern vector<double> crossoverProportion;
extern int max_tabu_iter;
extern int maxGenerations;
extern int num_cus;
extern int num_trucks;
extern int num_drones;
extern int popusize;
int main(int argc,char *argv[]){
    srand(time(nullptr));
    string instance=string(argv[1]);
    stringstream ss(instance);
    string token;
    getline(ss, token, '.');
    if(stoi(token)==20){
        num_cus=20;
        num_trucks=2;
        num_drones=2;
    }else if(stoi(token)==50){
        num_cus=50;
        num_trucks=3;
        num_drones=3;
    }else if(stoi(token)==100){
        num_cus=100;
        num_trucks=4;
        num_drones=4;
    }
    else if(stoi(token)==200){
        num_cus=200;
        num_trucks=10;
        num_drones=4;
    }
    inputTimeLimit(instance);
    //cout<<timeLimit<<endl;
    inputfile="./data/random_data/"+instance+txt;
    maxGenerations=stoi(argv[2]);  
    //maxGenerations=100000;
    //popusize=stoi(argv[2]);
    max_tabu_iter=stoi(argv[3]);
    time_t start,end;
    ios_base::sync_with_stdio(false);
    input();
    //cout<<1<<endl;
    drone_max_tracks=(int)(canuseddrone/num_drones)+1;
    total_node=num_cus+num_trucks+drone_max_tracks*num_drones-1;
    //cout<<drone_max_tracks<<" "<<total_node<<endl;
    initMatrix();
    vector<Individual> population;
    population.clear();
    
    //population(populationSize);

    //testcount=to_string(testcountint);
    for(int testcountint=1;testcountint<=10;testcountint++){
        srand(time(nullptr));
        population=selectPopulation(popusize);
        
        //cout<<1<<endl;
    //for(int i=6;i<12;i++){
        maxobj1=1000000000;
        maxobj2=1000000000;
        minobj1=0;
        minobj2=0;
        crossoverMod=50;
        crossoverProportion={1.0/3.0,1.0/3.0,1.0/3.0};
        outputfilename="./result"+txt;
        minobj1=population[0].fitness1;
        maxobj1=population[0].fitness1;
        minobj2=population[0].fitness2;
        maxobj2=population[0].fitness2;
        for(int j=0;j<population.size();j++){
            if(population[j].fitness1<minobj1) minobj1=population[j].fitness1;
            if(population[j].fitness2<minobj2) minobj2=population[j].fitness2;
            if(population[j].fitness1>maxobj1) maxobj1=population[j].fitness1;
            if(population[j].fitness2>maxobj1) maxobj1=population[j].fitness2;
        }
        obj1norm=maxobj1-minobj1;
        obj2norm=maxobj2-minobj2;
        vector<Individual> populationcopy;
        populationcopy.clear();
        populationcopy=population;
        vector<Individual> pareto;
        pareto.clear();
        vector<int> init=generateSol3();
        pareto=tabu_search2(init,max_tabu_iter);
        //pareto= NSGA2(populationcopy);;
        /*
        int paretosize=pareto.size();
        double minfit1=pareto[0].fitness1;
        double minfit2=pareto[paretosize-1].fitness2;
        double maxfit1=pareto[paretosize-1].fitness1;
        double maxfit2=pareto[0].fitness2;
        vector<ParetoRecord> paretoRecords;
        paretoRecords.clear(); 
        for(int j=0;j<pareto.size();j++){
            ParetoRecord paretoRecord;
            paretoRecord.indi=pareto[j];
            paretoRecord.normFit1=(pareto[j].fitness1-minfit1)/(maxfit1-minfit1);
            paretoRecord.normFit2=(pareto[j].fitness2-minfit2)/(maxfit2-minfit2);
            paretoRecords.push_back(paretoRecord);
        }*/  
        

    //}*/
    }
    
    return 0;
}