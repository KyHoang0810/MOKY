#ifndef DOMINATIONCHECK_H
#define DOMINATIONCHECK_H
#include "struct.h"
using namespace std;
int checkdomination(Individual &dominater,Individual &dominated){
    bool ch = false;
    if(dominater.fitness1-dominated.fitness1>1e-3) 
        return false;
    else if(dominated.fitness1-dominater.fitness1>1e-3) ch=true;
    if(dominater.fitness2-dominated.fitness2>1e-3) return false;
    else if(dominated.fitness2-dominater.fitness2>1e-3) ch=true;
    return ch;
}
#endif