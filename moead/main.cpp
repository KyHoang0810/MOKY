#include "problem.h"
#include "genetic_operator.h"
#include "local_search.h"
#include "moead.h"
#include <iostream>
#include <fstream>
// Chỉnh loại drone tuyến tính hay phi tuyến trong drone_type.h
double inputTimeLimit(std::string instance){
    std::ifstream inputfile("./src/time.txt");
    std::string line;
    double timeLimit=0;
    for(int i=0;i<60;i++){
        getline(inputfile,line);
        if(line.find(instance)!=std::string::npos){
            int po=line.find(";");
            std::string time=line.substr(po+1);
            timeLimit=stod(time);
        }
    }
    return timeLimit;
}
int main(int argc,char *argv[]) {
    TruckConfig truck_config("./config_parameter/truck_config.json");
    DroneConfig drone_config("./config_parameter/drone_linear_config.json", "4");
    std::string instance=std::string(argv[1]);
    Problem problem = Problem::from_file("./data/random_data/"+instance+".txt", truck_config, drone_config);
    MOEADOptions options;
    options.population_size = 200;
    options.neighbor_count = 10;
    options.max_population_count = std::stoi(argv[2]);


    options.crossover_rate = 0.9;
    options.mutation_rate = 0.05;

    options.initialization = create_random_population;
    options.generate_weights = uniform_weights;
    options.crossover = crossover;
    options.mutation = mutation;
    options.repair = repair;
    options.postprocessing = [] (Individual&, const Problem&) {};
    options.local_search = first_improvement_permutation_swap_hill_climbing;
    double timeLimit=inputTimeLimit(instance);
    
    for(int i=0;i<5;i++){
        Population pareto= moead(problem, options,timeLimit);
    }
    return 0;
}
