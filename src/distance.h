#include "parameter.h"
#include <cmath>
extern vector<Customer> customers;
extern vector<vector<double>> M;
extern int total_node;
double calculateDistance(int i, int j){
    double xDiff = customers[i].x - customers[j].x;
    double yDiff = customers[i].y - customers[j].y;
    double distance = sqrt(pow(xDiff, 2) + pow(yDiff, 2));
    return distance;
}

void initMatrix(){
    for(int i=0;i<=num_cus;i++){
        for(int j=0;j<total_node;j++){
            if(j>num_cus)M[i].push_back(calculateDistance(i, 0));
            else M[i].push_back(calculateDistance(i, j));
        }
    }
    for(int i=num_cus+1;i<total_node;i++){
        for(int j=0;j<total_node;j++){
            if(j>num_cus)M[i].push_back(0);
            else M[i].push_back(calculateDistance(0,j));
        }
    }
}