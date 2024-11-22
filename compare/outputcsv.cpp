#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>
#include <ctime>
#include<sstream>
#include <string>
#include <unistd.h>
using namespace std;
int bo=2;
string problem="20.5.2";
string txtLocation = "result5/"+problem+"/re";
string txt=".txt";

int paretosize;
vector<vector<double>> data;
vector<string>path;
int id=0;
int popsize=200;
int gen=200;
void writeCSV(const string& filename)
{
    ofstream outputFile;
    outputFile.open(filename, ios::app);
    if (!outputFile)
    {
        cerr << "Error opening file: " << filename << endl;
        return;
    }
    outputFile<<id<<","<<problem<<","<<popsize<<","<<gen<<","<<paretosize<<","<<",";
    outputFile<<"[";
    outputFile<<"("<<data[0][0]<<" "<<data[0][1]<<")";

    for (int j=1;j<path.size();j++)
    {   
        outputFile<<" ; ";
        outputFile<<"("<<data[j][0]<<" "<<data[j][1]<<")";

    }
    outputFile<<"]";
    outputFile<<endl;

    outputFile.close();
    cout << "CSV file created: " << filename << endl;
}

void readDataFromTextFile(const string& filename)
{

    ifstream file(filename);
    if (!file)
    {
        cerr << "Error opening file: " << filename << endl;
    }
    string line;
    getline(file,line);
    istringstream iss1(line);
    if(iss1>>paretosize){

    }

    for(int i=0;i<paretosize;i++)
    {   
         
        getline(file,line);
        path.push_back(line);
        getline(file,line);
        vector<double> row;
        double value;
        istringstream iss(line);
        while (iss >> value){
            row.push_back(value);
        }
        data.push_back(row);
    }
    cout<<data.size()<<endl;
    file.close();
}

int main()
{
    string csvFilename = "result2.csv";
    for(int i=1;i<=5;i++){
        id=bo*5+i;
        string num=to_string(i);
        string txtFilename=txtLocation+num+txt;
        readDataFromTextFile(txtFilename);
        writeCSV(csvFilename);
        data.clear();
        path.clear();
    }
    // Read data from text file

    // Write data to CSV file
    

    return 0;
}