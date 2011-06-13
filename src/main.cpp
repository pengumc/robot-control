#define __MAIN__

#include "robot-control/CQPed.h"
#include "robot-control/CGtk.h"


int main(int argc, char* argv[]){
    CQPed qwop;
    CGtk gui(&qwop);
    gui.run();
    return 0;
}


