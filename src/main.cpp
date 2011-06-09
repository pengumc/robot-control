//windows: g++ test.cpp -o test.exe
//linux g++ test.cpp -o test -lpthread
//------------------------------------------------------------------------------
#define __MAIN__
#include <iostream>
#include <stdint.h>
#include <math.h>
#define PI M_PI
#include <stdio.h>
//thread stuff is included for backup reasons
#ifdef _WIN32
    #include <conio.h>
    #include <win32_input.cpp>
		#define usleep(x) Sleep(x/1000)
#else
    #include <pthreads_input.cpp>
#endif

#include "CSolver.cpp"
#include "CPSController.cpp"
#include "CServo.cpp"
#include "CUsbDevice.cpp"
#include "CQPed.cpp"
#include "CAngle.cpp"
#include "CGtk.cpp"
using namespace std;

int main(int argc, char* argv[]){
    CQPed qwop;
    CGtk gui(&qwop);
    gui.run();
    return 0;
}


