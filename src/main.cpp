#define __MAIN__

#include "robot-control/CQPed.h"
#include "robot-control/CGtk.h"



/*
void testSolver(double X, double Y, double Z){
    CSolver2 solver;
    solverParams_t params;
    params.A = 3;
    params.B = 6.5;
    params.C = 5.5;
    params.X = X;
    params.Y = Y;
    params.Z = Z;
    solver.solveFor(&params);
    rot_vector_print(solver.resultVector);
    
}
*/
int main(int argc, char* argv[]){
  CQPed qwop;
    CGtk gui(&qwop);
    gui.run();

//    testSolver(atof(argv[1]), atof(argv[2]), atof(argv[3]));
    return 0;
}


