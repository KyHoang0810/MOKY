#ifndef STRUCT_H
#define STRUCT_H
#include <vector>
using namespace std;
struct Customer {
    double x;
    double y;
    double demand;
    double OnlyByTruck;
    double TruckServiceTime;
    double DroneServiceTime;
};
struct Individual{
    vector<int> route;
    double fitness1;
    double fitness2;
    double crowdingdistance;
    int tabusearch;
    int localsearch;
};
struct MoveRecord{
    int start[2];
    int end;
    int back[2];
    int front[2];
};
struct TabuRecord{
    Individual indi;
    MoveRecord record;
};
struct ParetoRecord{
    Individual indi;
    double normFit1;
    double normFit2;
    double criteria;
    int ranking; 
};

struct DistanceRanking{
    int node;
    double ranking;
};
struct NodeDistance{
    int node;
    double distance;
};
#endif // MY_STRUCT_H
