#include "hash.h"
#include "tabuSearch.h"
#include "crossover.h"
#include "localSearch.h"
#include "gaBase.h"
#include "dominationCheck.h"
#include "output.h"
#include <ctime>
using namespace std;
extern int total_node;

extern vector<vector<double>> M;
vector<Individual> NSGA2(vector<Individual> &defaultpop){
    time_t start,end;
    int end_iter;
    time(&start);
    vector<Individual> pareto;
    int option=0;
    int havetabu=0;
    int haveadaptive=0;
    int nochangeStreak=0;
    int tabu_perform=0;
    sort(defaultpop.begin(),defaultpop.end(),comparefit1);
    //outputLog(0,defaultpop);
    vector<Individual>population=defaultpop;
    vector<Individual> newPopulation;
    int nRe[100][3];
    int nPe[100][3];
    
    double tbtime=0;
    for (int i = 0; i < maxGenerations; i++) {
        //if(i==0)cout<<crossoverProportion[0]<<" "<<crossoverProportion[1]<<" "<< crossoverProportion[2]<<endl;
        if(i%crossoverMod==0){
            for(int row=0;row<crossoverMod;row++){
                for(int col=0; col<3;col++){
                    nRe[row][col]=0;
                    nPe[row][col]=0;
                }
            }
        }
        int improveInpareto=0;

        vector<Py_uhash_t>hashRecord;
        hashRecord.clear();
        newPopulation.clear();
        for(int j=0;j<population.size();j++){
            Py_uhash_t hashpo=calculateHash(population[j].route);
            newPopulation.push_back(population[j]);
            if(find(hashRecord.begin(),hashRecord.end(),hashpo)==hashRecord.end()){
                hashRecord.push_back(hashpo);
            }
        }
        //cout<<newPopulation.size()<<endl;
        for(int j=0;j<population.size();j++){
            population[j].crowdingdistance=0;
        }
        int tabuselectcount=0;
        int pocnt=0;
        /*if(option==5||option==7){
            havetabu=1;
            if(i>=50){
                int curSize=population.size();
                while(tabuselectcount<20&&pocnt<curSize){ 
                    if(population[pocnt].tabusearch<=10){          
                        vector<Individual>Taburesult=tabu_search(population[pocnt].route,20,i);
                        for(int k=0;k<Taburesult.size();k++){
                            Py_uhash_t hashpo=calculateHash(Taburesult[k].route);
                            if(find(hashRecord.begin(),hashRecord.end(),hashpo)==hashRecord.end()){
                                hashRecord.push_back(hashpo);
                                newPopulation.push_back(Taburesult[k]);
                            } 
                        }
                        tabuselectcount++;
                    }
                    population[pocnt].tabusearch+=1;
                    pocnt++; 
                }
            }
        }*/
        int crossoverAlgo;
        int offpopu=0;
        int nReward;
        int nPenalty;
        
        while (offpopu< population.size()) {
            // Crossover
            crossoverAlgo=crossoverSelection();
            if(crossoverAlgo<0||crossoverAlgo>3)cout<<"error"<<endl;
            nReward=0;
            nPenalty=0;
            Individual offspring;
            Individual parent1;
            Individual parent2;
            int crossoverPerformed=0;
            while(crossoverPerformed==0){
                parent1=tournamentSelection(population);
                do{
                    parent2=tournamentSelection(population);
                }while(abs(parent1.fitness1-parent2.fitness1)<1e-3&&abs(parent1.fitness2-parent2.fitness2)<1e-3);
                double randomNum = (rand()%100) / 100.0;
            
            //cout<<to_string(offpopu)+" "+to_string(i)<<endl;
                if (randomNum <CROSSOVER_RATE) {
                    crossoverPerformed=1;
                    offspring=crossover(parent1,parent2,crossoverAlgo);
                    //if(!feasibleroute(offspring.route)) cout<<"error"<<endl;
                }
            }
            
            // Mutation

            if (((rand()%100) / 100.0) < MUTATION_RATE) {
                int point1 = rand() % total_node;
                int point2 = rand() % total_node;

                swap(offspring.route[point1], offspring.route[point2]);
                
                offspring.route=repairposition(offspring.route);
                offspring.route=repairroute(offspring.route);
            }
            
            pair<double,double> off1fit=calculatefitness(offspring.route);
            offspring.fitness1=off1fit.first;
            offspring.fitness2=off1fit.second;
            offspring.tabusearch=0;
            Py_uhash_t hashpooff=calculateHash(offspring.route);
            if(find(hashRecord.begin(),hashRecord.end(),hashpooff)==hashRecord.end()){
                hashRecord.push_back(hashpooff);
                newPopulation.push_back(offspring);
                offpopu++;
                if(crossoverPerformed==1){
                    if(checkdomination(parent1,parent2)){
                        if(checkdomination(parent1,offspring)) nPenalty+=1;
                        else nReward+=1;
                    }
                    else if(checkdomination(parent2,parent1)){
                        if(checkdomination(parent2,offspring)) nPenalty+=1;
                        else nReward+=1;
                    }
                    else{
                        if(!checkdomination(parent1,offspring)&&!checkdomination(parent2,offspring)) nReward+=1;
                        else nPenalty+=1;
                    }
                }
            }
            nRe[i%crossoverMod][crossoverAlgo]+=nReward;
            nPe[i%crossoverMod][crossoverAlgo]+=nPenalty;
        }
        
        
        
        //int popusize=400;
        //if(false){
            if(nochangeStreak%30==0&&nochangeStreak>0){
            //cout<<i<<"!"<<endl;
                //std::ofstream outputFile;
                //outputFile.open(outputtblog, std::ios::app);
                //outputFile<<"Generation "<<i<<" :";
                //outputFile<<pareto.size()<<endl;
                //tbtime++;
                vector<Individual>Taburesult;
                for(int f=0;f<pareto.size();f++){
                    //if(pareto[f].tabusearch>1)continue;
                    //outputFile<<"Solution "<<f+1<<":"<<endl;
                    //vector<Individual>Taburesult;
                    Taburesult.clear();
                    if(max_tabu_iter>0) Taburesult=tabu_search(pareto[f].route,max_tabu_iter);
                    for(int k=0;k<Taburesult.size();k++){
                        newPopulation.push_back(Taburesult[k]);
                    }
                    //pareto[f].tabusearch+=1;
                }
            //cout<<newPopulation.size()<<endl;
            //nochangeStreak=0;
            //population=selectNewPopulation(newPopulation,popusize,1);
            }
        //}
       // cout<<newPopulation.size()<<endl;

        population=selectNewPopulation(newPopulation,popusize);
         
        sort(population.begin(),population.end(),comparefit1);
        double bestobj1=population[0].fitness1;
        sort(population.begin(),population.end(),comparefit2);
        double bestobj2=population[0].fitness2;
        
        
        vector<vector<int>> paretonumlayer=fast_non_dominated_sort(population);
        vector<int> paretonum;
        paretonum.clear();
        paretonum=paretonumlayer[0];
        for(int j=0;j<paretonum.size();j++){
            if(!inpareto(population[paretonum[j]],pareto)){
                if(!dominatedinpareto(population[paretonum[j]],pareto)) improveInpareto=1;
                updatepareto(population[paretonum[j]],pareto);
            }   
        }
        if(improveInpareto>0){
            time_t las;
            time(&las);
            nochangeStreak=0;
            tbtime=las-start;
        }  
        else nochangeStreak+=1;
        if(nochangeStreak>60){end_iter=i+1;break;}
        sort(pareto.begin(),pareto.end(),comparefit1);
        if(paretonum.size()!=pareto.size()){
            vector<Individual> tester;
            tester.clear();
            for(int p=0;p<paretonum.size();p++){
                tester.push_back(population[paretonum[p]]);
            }
            //cout<<i<<endl;
            for(int p=0;p<pareto.size();p++){
                if(!inpareto(pareto[p],tester)){//cout<<1<<endl;
                population.push_back(pareto[p]);}
                //population=selectNewPopulation(population,popusize);
            }
            sort(tester.begin(),tester.end(),comparefit1);
            //outputLog(i,tester);
        }
        //outputLog(i,population);
        time(&end);
        //if(double(end-start)>timeLimit){end_iter=i+1; break;}
        if(i==maxGenerations-1) end_iter=maxGenerations;
        //cout<<population.size()<<endl;
    }
    time(&end);
    sort(pareto.begin(),pareto.end(),comparefit1);
    output(pareto,double(end-start),end_iter,tbtime,end_iter-nochangeStreak);
    //outputpareto(pareto);
    //cout<<havetabu<<" "<<haveadaptive<<endl;
    return pareto;
}
