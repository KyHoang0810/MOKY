#include "moead.h"

#include "random.h"
#include<iostream>
#include<ctime>
double tchebycheff(const Fitness& fitness, const std::vector<double>& reference_point, const std::vector<double>& weight) {
    return std::max(weight[0]*std::abs(fitness[0] - reference_point[0]), weight[1]*std::abs(fitness[1] - reference_point[1]));
}

std::vector<std::vector<double>> uniform_weights(std::size_t population_size) {
    if (population_size <= 0) return {};
    if (population_size == 1) return {{0.5, 0.5}};

    std::vector<std::vector<double>> weights;
    weights.reserve(population_size);

    double step = 1.0/((double) population_size - 1);
    for (std::size_t i = 0; i < population_size; ++i) {
        weights.push_back({(double) i * step, (double) (population_size - i - 1) * step});
    }

    return weights;
}

Individual create_offspring(const std::vector<std::size_t>& neighbors, MOEADPopulation& population, const Problem& problem, const MOEADOptions& options) {
    // Chọn ngẫu nhiên 2 cá thể trong tập hàng xóm và sinh con
    std::uniform_int_distribution<std::size_t> distribution(0, neighbors.size() - 1);
    std::uniform_real_distribution<> chance_distribution(0, 1);
    double chance;

    std::size_t first = distribution(random_engine);
    std::size_t second = distribution(random_engine);
    while (first == second) second = distribution(random_engine);

    first = neighbors[first];
    second = neighbors[second];

    // Lai ghép
    //bool get_first_child =true;
    
    bool get_first_child = std::uniform_int_distribution<>(0, 1)(random_engine);
    Individual result = get_first_child
                        ? options.crossover(population.individual_list[first], population.individual_list[second]).first
                        : options.crossover(population.individual_list[first], population.individual_list[second]).second;

    // Nếu quay số vào ô đột biến hoặc nếu phải đột biến trên con tệ và con đang tệ thì đột biến
    chance = chance_distribution(random_engine);
    if (options.force_mutation_on_bad_crossover && (result == population.individual_list[first] || result == population.individual_list[second])) chance = 0;
    if (chance < options.mutation_rate) options.mutation(result);

    if (!is_valid(result, problem)) {
        result = options.repair(result, problem);
    }

    return result;
}

bool evolve_population(MOEADPopulation& population, const Problem& problem, const MOEADOptions& options) {
    std::uniform_real_distribution<> distribution(0, 1);
    double chance;
    bool update=false;
    // Duyệt qua toàn bộ cá thể trong quần thể
    for (std::size_t i = 0; i < population.size(); ++i) {
        // Chuẩn bị tập hàng xóm
        chance = distribution(random_engine);
        std::vector<std::size_t> neighbors;

        if (chance < options.select_parent_from_whole_population_probability) {
            neighbors.resize(population.size());
            std::iota(neighbors.begin(), neighbors.end(), 0);
        } else {
            neighbors = population.neighbor_indices[i];
        }

        // 1. Sinh con ứng với cá thể hiện tại
        Individual offspring = create_offspring(neighbors, population, problem, options);
        Fitness offspring_fitness = fitness(offspring, problem);

        // 2. Cập nhật z
        if (offspring_fitness[0] < population.reference_point[0]) population.reference_point[0] = offspring_fitness[0];
        if (offspring_fitness[1] < population.reference_point[1]) population.reference_point[1] = offspring_fitness[1];

        // 3. Cập nhật hàng xóm: Nếu nghiệm này tốt hơn hàng xóm cho bài toán của hàng xóm thì thay thế hàng xóm
        std::shuffle(neighbors.begin(), neighbors.end(), random_engine);
        int replaced_solution_count = 0;
        for (std::size_t index : neighbors) {
            if (replaced_solution_count >= options.maximum_number_of_replaced_solution_each_child) break;

            double fitness1 = tchebycheff(offspring_fitness, population.reference_point, population.weights[i]);
            double fitness2 = tchebycheff(population.fitness_list[index], population.reference_point, population.weights[i]);

            if (fitness1 < fitness2) {
                population.individual_list[index] = offspring;
                population.fitness_list[index] = offspring_fitness;
                ++replaced_solution_count;
            }
        }

        // 4. Cập nhật external population
        // 4.1. Loại bỏ các nghiệm bị trội bởi offspring khỏi EP
        std::size_t end = 0;
        for (std::size_t index = 0; index < population.external_population.size(); ++index) {
            if (is_better(offspring_fitness, population.external_population_fitness[index])) {
                continue;
            }
            if (end != index) {
                population.external_population[end] = population.external_population[index];
                population.external_population_fitness[end] = population.external_population_fitness[index];
            }
            ++end;
        }
        population.external_population.resize(end);
        population.external_population_fitness.resize(end);

        // 4.2. Thêm offspring vào EP nếu không có nghiệm nào trong EP trội nó
        bool do_insert = true;
        for (const Fitness& fitness : population.external_population_fitness) {
            if (is_better(fitness, offspring_fitness) || fitness == offspring_fitness) {
                do_insert = false;
                break;
            }
        }
        if (do_insert) {
            update=true;
            population.external_population.push_back(offspring);
            population.external_population_fitness.push_back(offspring_fitness);
        }
    }
    return update;
}
bool comparefit1(Fitness  &indi1,Fitness &indi2){
    return indi1[0]<indi2[0];
}
void output( MOEADPopulation pareto,double time,int end_iter,int tbtime,int last_update){
    std::ofstream outputFile;
    outputFile.open("./result.txt", std::ios::app); // Open the file for writing
    outputFile<<"Time:"<<time<<std::endl;
    outputFile<<"Last Iter:"<<end_iter<<std::endl;
    outputFile<<"Last Update:"<<last_update<<std::endl;
    outputFile<<"MOEAD:"<<tbtime<<std::endl;
    outputFile<<pareto.external_population.size()<<std::endl;
    std::vector<Fitness> cop=pareto.external_population_fitness;
    std::sort(cop.begin(),cop.end(),comparefit1);
    for(int i=0;i<cop.size();i++){
        outputFile << cop[i][0] <<" "<<cop[i][1]<<std::endl;
    }
    outputFile<<std::endl;
    outputFile.close();
}
Population moead(const Problem& problem, const MOEADOptions& options,double timelimit) {
    // 1. Initialization
    MOEADPopulation population(problem, options);
    std::cout<<timelimit<<std::endl;
    //log(population);
    time_t start,end;
    int enditer=0;
    double lastuptime=0;
    time(&start);
    // 2. Update
    for (int i = 1; i <= options.max_population_count; ++i) {
        bool haveupdate=evolve_population(population, problem, options);
        //log(population);
        if(haveupdate){
            time_t las;
            time(&las);
            lastuptime=double(las-start);
        }
        for (const Fitness& fitness : population.external_population_fitness) {
            //print(fitness);
        }
        //std::cout<<"Generation " << std::to_string(i)<<std::endl;
        time(&end);
        if(double(end-start)>=timelimit){enditer=i+1; break;}
    }
    output(population,double(end-start),enditer,lastuptime,0);
    std::cout<<double(end-start)<<std::endl;
    return population.external_population;
}
