#include "stdio.h"
#include "stdlib.h"
#include "robot-control/CSolver_new.h"

int main(int argc, char* argv[]){
    CSolver2 solver;
    solver.p.A = 0;
    solver.p.Z = 0;
    solver.p.B = atof(argv[1]);
    solver.p.C = atof(argv[2]);
    solver.p.X = atof(argv[3]);
    solver.p.Y = atof(argv[4]);
    solver.solveFor();
}

