#include <string>
#include <iostream>
#include <fstream>
#include<sstream>
#include "parameter.h"
using namespace std;
extern vector<Customer> customers;
extern int canuseddrone;
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
        b.demand=a[2];
        b.OnlyByTruck=a[3];
        if(a[3]==0)canuseddrone++;
        b.TruckServiceTime=a[4];
        b.DroneServiceTime=a[5];
        customers.push_back(b);
    }
}