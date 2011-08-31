#include "robot-control/KalmanFilter.h"
//------------
// CONSTRUCTOR
//------------
KalmanFilter::KalmanFilter(){
    init();
    setSz(0.0);
    setSw(0.0);
}

KalmanFilter::KalmanFilter(double newSz, double newSw){
    init();
    setSz(newSz);
    setSw(newSw);
}

void 
KalmanFilter::init(){
    x_last = 0.0;
    P_last = 0.0;
}


//------------
// GET X
//---------
double 
KalmanFilter::getX(){
    return x;
}



//------------
//STEP FILTER
//-----------
void 
KalmanFilter::step(double measurement){
    double x_temp, P_temp, K;

    x_temp = x_last;
    P_temp = P_last + Sw;

    K = ( 1.0 / ( P_temp + Sz ) * P_temp );
    x = x_temp + K * ( measurement - x_temp );
    P = ( 1.0 - K ) * P_temp;

    x_last = x;
    P_last = P;
}



//---------------
// SET SZ
//--------------
void 
KalmanFilter::setSz(double newSz){
    Sz = newSz;
}

//---------------
// SET Sw
//--------------
void 
KalmanFilter::setSw(double newSw){
    Sw = newSw;
}



//-----------
// GET SZ
//-----------
double
KalmanFilter::getSz(){
    return Sz;
}



//-----------
// GET SW
//-----------
double
KalmanFilter::getSw(){
    return Sw;
}
