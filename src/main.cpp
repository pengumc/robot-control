#define __MAIN__
#include <stdint.h>
#include <math.h>
#define PI M_PI



#include "robot-control/CQPed.h"
#include "robot-control/CGtk.h"
using namespace std;

int main(int argc, char* argv[]){
    CQPed qwop;
    CGtk gui(&qwop);
    gui.run();
    return 0;
}


