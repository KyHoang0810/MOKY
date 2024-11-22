#ifndef COMPARE_H
#define COMPARE_H
#include "struct.h"
using namespace std;
bool comparefit1(Individual  &indi1,Individual &indi2){
    return indi1.fitness1<indi2.fitness1;
}
bool comparefit2(Individual  &indi1,Individual &indi2){
    return indi1.fitness2<indi2.fitness2;
}
bool compareDistance(Individual  &indi1,Individual &indi2){
    return indi1.crowdingdistance>indi2.crowdingdistance;
}
bool compareCriteria(ParetoRecord  &indi1,ParetoRecord &indi2){
    return indi1.criteria<indi2.criteria;
}
bool compareRanking(ParetoRecord  &indi1,ParetoRecord &indi2){
    return indi1.ranking<indi2.ranking;
}
bool compareNodeDistance(NodeDistance &nd1, NodeDistance &nd2){
    return nd1.distance<nd2.distance;
}
bool compareDistanceRanking(DistanceRanking &dr1, DistanceRanking &dr2){
    return dr1.ranking<dr2.ranking;
}
#endif