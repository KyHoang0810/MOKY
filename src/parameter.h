#ifndef PARAMETER_H
#define PARAMETER_H
#include <vector>
#include <string>
#include <algorithm>
#include "struct.h"
using namespace std;
int crossoverMod=50;
int num_cus=20;
int num_trucks=2;
int num_drones=2;
int popusize=200;
#define droneservetime 30
#define truckservetime 60
#define populationSize   200
int maxGenerations=2000;
#define MUTATION_RATE   0.05
#define CROSSOVER_RATE   0.9
#define VmaxTruck 15.557
#define takeoffSpeed 15.6464
#define cruiseSpeed 31.2928
#define landingSpeed 7.8232
#define cruiseAlt 50
#define takeoffTime cruiseAlt/takeoffSpeed
#define landingTime cruiseAlt/landingSpeed
#define capacityC 2.27
#define batteryPower 904033
#define betaB 210.8
#define gama 181.2
#define mvalue 1e20

string txt=".txt";
string testcount="1";
string problem="20.20.2";//4
string resultsrc="../result9/";
string inputfile="./data/random_data/"+problem+txt;
string outputfilename;
string outputparetofilename;
string outputparetoselectionfilename;
string outputlog;
string outputgraph;
string outputtblog;
vector<Customer> customers;
vector<vector<double>> M(10000);
int total_node;
int canuseddrone;
int drone_max_tracks;
int max_tabu_iter;
double maxobj1=1000000000;
double maxobj2=1000000000;
double minobj1=0;
double minobj2=0;
double obj1norm;
double obj2norm;
double timeLimit;
vector<double> crossoverProportion={1.0/3.0,1.0/3.0,1.0/3.0};

#endif