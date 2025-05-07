#include "routefix.h"
using namespace std;
extern vector<double> crossoverProportion;
int crossoverSelection() {
    double r = static_cast<double>(rand()) / RAND_MAX;
    double cumulativeProbability = 0.0;

    for (int i = 0; i < crossoverProportion.size(); ++i) {
        cumulativeProbability += crossoverProportion[i];
        if (r <= cumulativeProbability) {
            return i;
        }
    }

    // In case of numerical issues, return the last index
    return rand()%3;
}
Individual CX(Individual parent1,Individual parent2){
    Individual offspring;
    int totalNodes= parent1.route.size();
    // Initialize offspring with -1 to indicate unassigned positions
    offspring.route.assign(totalNodes, -1);
    vector<int> visited;
    for(int i=0;i<totalNodes;i++){
        visited.push_back(0);
    }
    int start=0;
    int cycNo=0;
    while (std::find(visited.begin(), visited.end(), 0) != visited.end()) {
        if (visited[start]) {
            start = std::find(visited.begin(), visited.end(), 0) - visited.begin();
        }

        int pos = start;
        std::vector<int> cycle;

        // Create the cycle starting from the start position
        if(cycNo%2==0){
            do {
                cycle.push_back(pos);
                visited[pos] = 1;
                pos = std::find(parent1.route.begin(), parent1.route.end(), parent2.route[pos]) - parent1.route.begin();
            } while (pos != start);
        }
        else{
            do {
                cycle.push_back(pos);
                visited[pos] = 1;
                pos = std::find(parent2.route.begin(), parent2.route.end(), parent1.route[pos]) - parent2.route.begin();
            } while (pos != start);
        }
        // Use the cycle to create the offspring
        for (int i = 0; i < cycle.size(); ++i) {
            if (cycNo % 2 == 0) {
                offspring.route[cycle[i]] = parent1.route[cycle[i]];
            } else {
                offspring.route[cycle[i]] = parent2.route[cycle[i]];
            }
        }
        cycNo+=1;
    }
    
    
    offspring.route= repairposition(offspring.route);
    offspring.route=repairroute(offspring.route);
    visited.clear();
    return offspring;
}
Individual OX(Individual parent1,Individual parent2){
    Individual offspring;
    int totalNodes= parent1.route.size();
    int startPos = rand() % totalNodes;
    int endPos;
    do{
        endPos=rand() % totalNodes;
    }while(startPos==endPos);
    // Ensure the start position is before the end position
    if (startPos > endPos) {
        swap(startPos, endPos);
    }
    // Initialize offspring with -1 to indicate unassigned positions
    offspring.route.assign(totalNodes, -1);
    vector<int> visited;
    for(int i=0;i<totalNodes;i++){
        visited.push_back(0);
    }
    // Copy the selected segment from parent 1 to offspring 1 and parent 2 to offspring 2
    for (int i = startPos; i <= endPos; ++i) {
        offspring.route[i] = parent1.route[i];
        visited[parent1.route[i]]=1;
        
    }
    // Fill the remaining positions in offspring using the order of the other parent
    int index1=startPos;
    vector<int>remain;
    for(int x=startPos;x<totalNodes;x++){
        if(!visited[parent2.route[x]]){
            int e=parent2.route[x];
            remain.push_back(e);
        }
    }
    for(int x=0;x<startPos;x++){
        if(!visited[parent2.route[x]]){
            int e=parent2.route[x];
            remain.push_back(e);
        }
    }
        
    int t=0;
    if(endPos==totalNodes-1) {
        for(int x=0;x<startPos;x++){
            offspring.route[x]=remain[t];
            t++;
        }
    }
    else{
        for(int x=endPos+1;x<totalNodes;x++){
            offspring.route[x]=remain[t];
            t++;
        }
        for(int x=0;x<startPos;x++){
            offspring.route[x]=remain[t];
            t++;
        }
    }
    offspring.route= repairposition(offspring.route);
    offspring.route=repairroute(offspring.route);
    visited.clear();
    return offspring;
}
Individual PMX(Individual &parent1,Individual &parent2){
    Individual offspring;
    int totalNodes= parent1.route.size();
    int startPos = rand() % totalNodes;
    int endPos;
    do{
        endPos=rand() % totalNodes;
    }while(startPos==endPos);
    // Ensure the start position is before the end position
    if (startPos > endPos) {
        swap(startPos, endPos);
    }
    // Initialize offspring with -1 to indicate unassigned positions
    offspring.route.assign(totalNodes, -1);
    vector<int> visited;
    for(int i=0;i<totalNodes;i++){
        visited.push_back(0);
    }
    // Copy the selected segment from parent 1 to offspring 1 and parent 2 to offspring 2
    for (int i = startPos; i <= endPos; ++i) {
        offspring.route[i] = parent1.route[i];
        visited[parent1.route[i]]=1;    
    }
    for (int i = 0; i <totalNodes; ++i) {
        int value = parent2.route[i];
        if(i<startPos||i>endPos){
            if (visited[value]==0) {
                offspring.route[i] = parent2.route[i];
            }
            else{
                int j = std::find(parent1.route.begin(), parent1.route.end(), value) - parent1.route.begin();
                while (j >= startPos && j <= endPos) {
                    value = parent2.route[j];
                    j = std::find(parent1.route.begin(), parent1.route.end(), value) - parent1.route.begin();
                }
                offspring.route[i] = value;
            }
            visited[value]=1;
        }
    }
    
    
    offspring.route= repairposition(offspring.route);
    offspring.route=repairroute(offspring.route);
    visited.clear();
    return offspring;
}
Individual APX(Individual parent1,Individual parent2){
    Individual offspring;
    int totalNodes= parent1.route.size();
    // Initialize offspring with -1 to indicate unassigned positions
    offspring.route.assign(totalNodes, -1);
    vector<int> visited;
    for(int i=0;i<totalNodes;i++){
        visited.push_back(0);
    }
    vector<int> merge;
    for(int i=0;i<totalNodes;i++){
        merge.push_back(parent1.route[i]);
        merge.push_back(parent2.route[i]);
    }
    int po=0;
    for(int i=0;i<totalNodes;i++ ){
        int value=merge[po];
        while(visited[value]!=0){
            po++;
            value=merge[po];
        }
        offspring.route[i]=value;
        visited[value]=1;
        po++;
    }
    
    
    offspring.route= repairposition(offspring.route);
    offspring.route=repairroute(offspring.route);
    visited.clear();
    return offspring;
}
Individual POS(Individual parent1,Individual parent2){
    Individual offspring;
    int totalNodes= parent1.route.size();
    // Initialize offspring with -1 to indicate unassigned positions
    offspring.route.assign(totalNodes, -1);
    vector<int> visited;
    for(int i=0;i<totalNodes;i++){
        visited.push_back(0);
    }
    vector<int> selectedPositions;
    int numPositions = rand() % totalNodes + 1;
    while (selectedPositions.size() < numPositions) {
        int pos = rand() % totalNodes;
        if(find(selectedPositions.begin(),selectedPositions.end(),pos)==selectedPositions.end())selectedPositions.push_back(pos);
    }
    
    // Copy the selected positions from the first parent to the offspring
    for (int pos : selectedPositions) {
        offspring.route[pos] = parent1.route[pos];
        visited[parent1.route[pos]]=1;
    }
    int currentPos = 0;
    for (int i = 0; i < totalNodes; ++i) {
        if (offspring.route[i] == -1) {
            while (visited[parent2.route[currentPos]]==1) {
                currentPos++;
            }
            offspring.route[i] = parent2.route[currentPos];
            visited[parent2.route[currentPos]]=1;
            currentPos++;
        }
    }
    
    
    offspring.route= repairposition(offspring.route);
    offspring.route=repairroute(offspring.route);
    visited.clear();
    return offspring;
}

Individual crossover(Individual &indi1, Individual &indi2, int selection){
    switch (selection)
    {
    case 0: return PMX(indi1, indi2);
    //case 1: return OX(indi1, indi2);
    case 1: return POS(indi1, indi2);
    case 2: return CX(indi1, indi2);
    }
    return PMX(indi1,indi2);
}
void updateCrossoverProportion(int (&nRe)[100][3], int (&nPe)[100][3]){
    int Re[3]={0,0,0};
    int Pe[3]={0,0,0};
    double sum=0.0;
    double stra[3]={0.0,0.0,0.0};
    for(int i=0;i<crossoverMod;i++){
        for(int j=0;j<3;j++){
            Re[j]+=nRe[i][j];
            Pe[j]+=nPe[i][j];
        }
    }
    double remain=1.0;
    for(int i=0;i<3;i++){
        if(Re[i]==0 &&Pe[i]==0) {remain-=crossoverProportion[i];stra[i]=0.0;}
        else stra[i]=double(Re[i])/double(Pe[i]+Re[i]);
        sum+=stra[i];
    }
    for(int i=0;i<3;i++){
        if(Re[i]>0||Pe[i]>0) crossoverProportion[i]=stra[i]*remain/sum;
    }
}
