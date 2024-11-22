#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>
#include <ctime>
#include<sstream>
#include <string>
using namespace std;
string problem[24]={"20.5.1","20.5.2","20.5.3","20.5.4",
                "20.10.1","20.10.2","20.10.3","20.10.4",
                "20.20.1","20.20.2","20.20.3","20.20.4",
                "50.10.1","50.10.2","50.10.3","50.10.4",
                "50.20.1","50.20.2","50.20.3","50.20.4",
                "50.30.1","50.30.2","50.30.3","50.30.4"};
string txtLocation;
string outputparetoselectionfilename="result6/"+problem[0]+"/selection.txt";
string txt=".txt";
int bo=8;
int paretosize;
vector<vector<double>> data;
vector<string>path;
int id=0;
int popsize=200;
int gen=200;
struct Individual{
    double fitness1;
    double fitness2;
};
struct ParetoRecord{
    Individual indi;
    double normFit1;
    double normFit2;
    double criteria;
    int ranking; 
};
bool compareCriteria(ParetoRecord  &indi1,ParetoRecord &indi2){
    return indi1.criteria<indi2.criteria;
}
bool compareRanking(ParetoRecord  &indi1,ParetoRecord &indi2){
    return indi1.ranking<indi2.ranking;
}
vector<Individual> pareto;
int checkdomination(Individual dominater,Individual dominated){
    bool ch = false;
    if(dominater.fitness1-dominated.fitness1>1e-9) 
        return false;
    else if(dominated.fitness1-dominater.fitness1>1e-3) ch=true;
    if(dominater.fitness2-dominated.fitness2>1e-3) return false;
    else if(dominated.fitness2-dominater.fitness2>1e-3) ch=true;
    return ch;
}
bool dominatedinpareto(Individual indi){
    if(pareto.size()==0)return false;
    for(int i=0;i<pareto.size();i++){
        if(checkdomination(pareto[i],indi))return true;
    }
    return false;
}
bool comparefit1(Individual  &indi1,Individual &indi2){
    return indi1.fitness1<indi2.fitness1;
}
void updatepareto(Individual indi){
    vector<int> dominated;
    if(!dominatedinpareto(indi)){
        for(int i=0;i<pareto.size();i++){
            if(checkdomination(indi,pareto[i]))dominated.push_back(i);
        }
        sort(dominated.begin(),dominated.end());
        for(int i=0;i<dominated.size();i++){
            pareto.erase(pareto.begin()+dominated[i]-i);

        }
        pareto.push_back(indi);
    }
    
}
bool inpareto(Individual indi){
    for(int i=0;i<pareto.size();i++){
        if(abs(indi.fitness1-pareto[i].fitness1)<1e-4&&abs(indi.fitness2-pareto[i].fitness2)<1e-4) return true;

    }
    return false;
}
void writeCSV(const string& filename)
{
    ofstream outputFile;
    outputFile.open(filename, ios::app);
    if (!outputFile)
    {
        cerr << "Error opening file: " << filename << endl;
        return;
    }
    //outputFile<<pareto.size();
    //outputFile<<"[";
    for (int j=0;j<pareto.size();j++)
    {   
        outputFile<<"("<<pareto[j].fitness1<<", " <<pareto[j].fitness2<<")";
        if(j<pareto.size()-1)outputFile<<"; ";

    }
    //outputFile<<"]";
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
    int time;
    istringstream iss2(line);
    if(iss2>>time){}

    getline(file,line);
    int last_iter;
    istringstream iss2(line);
    if(iss2>>time){}
    
    getline(file,line);
    istringstream iss1(line);
    if(iss1>>paretosize){

    }

    for(int i=0;i<paretosize;i++)
    {   
         
        getline(file,line);
        getline(file,line);
        vector<double> row;
        double value;
        istringstream iss(line);
        while (iss >> value){
            row.push_back(value);
        }
        Individual indi;
        indi.fitness1=row[0];
        indi.fitness2=row[1];
        if(!inpareto(indi)){
            updatepareto(indi);
        }
        data.push_back(row);
    }
    file.close();
}
void outputParetoResult(vector<ParetoRecord> paretoRecords,int k){
    std::ofstream outputFile;
    outputFile.open(outputparetoselectionfilename,std::ios::app); 
    int limit=paretoRecords.size()>5?5:paretoRecords.size();
    if(k!=0){outputFile<<"Criteria "+to_string(k)+": "+to_string(0.25*k)+" fitness 1 and "+to_string(0.25*(3-k))+" fitness2"<<endl;}
    else {
        outputFile<<"General Ranking:"<<endl;
    }
    for(int i=0;i<limit;i++){
        outputFile << paretoRecords[i].indi.fitness1 <<" "<<paretoRecords[i].indi.fitness2<< endl;
    }
}
int main()
{
    string csvFilename = "./mine.csv";
    //string rescount;
    //std::cin>>rescount;
    //for(int rescount=1;rescount<7;rescount++){
    //for(int t=0;t<24;t++){
        pareto.clear();
        txtLocation = "./result";
        for(int i=1;i<=10;i++){
            //if(t<12&&i>5)continue;
            string num=to_string(i);
            string txtFilename=txtLocation+num+txt;
            readDataFromTextFile(txtFilename);
            data.clear();
            path.clear();
        }
        sort(pareto.begin(),pareto.end(),comparefit1);
        writeCSV(csvFilename);
    //}
    //}

    // Read data from text file

    // Write data to CSV file
    

    return 0;
}