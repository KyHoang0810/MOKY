#include <string>
#include <iostream>
#include <fstream>
#include<sstream>
#include "parameter.h"
using namespace std;
extern vector<Customer> customers;
extern int canuseddrone;
extern double timeLimit;
bool checkcustomer(int a){
    double totalweight=customers[a].demand;
    if(totalweight>capacityC)return false;
    double weight=0;
    double totalenergy=0;
    double xDiff = customers[0].x - customers[a].x;
    double yDiff = customers[0].y - customers[a].y;
    double distance = sqrt(pow(xDiff, 2) + pow(yDiff, 2));
    for(int i=0;i<2;i++){ 
        double energypersec=gama+betaB*weight;
        double time=distance/cruiseSpeed;
    
        totalenergy+=(takeoffTime+landingTime+time)*energypersec;
        if(totalenergy> batteryPower) return false;
        weight=totalweight;
    }
    return true;
}
void input(){
    
    Customer depot;
    depot.x=0;
    depot.y=0;
    depot.demand=0;
    customers.push_back(depot);
    ifstream inputFile(inputfile); // Open input.txt for reading
    int currentLine = 1;
    string line;   
    while (currentLine < 6 && getline(inputFile, line)) {
        currentLine++;
    }
    vector<float>a;
    for(int i=0;i<num_cus;i++){
        a.clear();
        a.resize(0);
        getline(inputFile, line);
        
        istringstream iss(line);
        float value;
        while (iss >> value){
            a.push_back(value);
        }
        Customer b;
        b.x=a[0];
        b.y=a[1];
        b.demand=a[2]*10;
        b.OnlyByTruck=a[3];
        if(a[3]==0)canuseddrone++;
        b.TruckServiceTime=a[4];
        b.DroneServiceTime=a[5];
        customers.push_back(b);
    }
    for(int i=1;i<=num_cus;i++){
        if(customers[i].OnlyByTruck==0){
            if(!checkcustomer(i)){customers[i].OnlyByTruck=1;canuseddrone--;}
        }
    }
}
void inputTimeLimit(string instance){
    ifstream inputfile("./src/time.txt");
    string line;
    for(int i=0;i<60;i++){
        getline(inputfile,line);
        if(line.find(instance)!=string::npos){
            int po=line.find(";");
            string time=line.substr(po+1);
            timeLimit=stod(time);
        }
    }
}