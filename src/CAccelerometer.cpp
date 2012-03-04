#include "robot-control/CAccelerometer.h"
#include <math.h>

CAccelerometer::CAccelerometer(double Sz, double Sw){
    filterPhi.setSz(Sz);
    filterPhi.setSw(Sw);
    filterTheta.setSz(Sz);
    filterTheta.setSw(Sw);

}

double CAccelerometer::get_phi(){
    return filterPhi.getX();
}
double CAccelerometer::get_theta(){
    return filterTheta.getX();
}

double* CAccelerometer::_calcTilt(){
    const double Ax = adc[0] - ACC_MID;
    const double Ay = adc[1] - ACC_MID;
    const double Az = adc[2] - ACC_MID;
    const double phi = atan(Ay / sqrt(Ax * Ax + Az * Az));
    const double theta = atan(Ax / sqrt(Ay*Ay + Az*Az));
    double ret[2];
    ret[0] = phi;
    ret[1] = theta;
    return(ret)
}

void CAccelerometer::step(uint8_t adcx, uint8_t adcy, uint8_t adcz){
    adc[0] = adcx;
    adc[1] = adcy;
    adc[2] = adcz;
    double* tilt = _calcTilt();
    filterPhi.step(tilt[0]);
    filterTheta.step(tile[1]);

}

void CAccelerometer::storeAnglesInVector(rot_vector_t* vectorToFill){
    //the accelerometer y-axis is our z-axis
    vectorToFill.setX(filterPhi.getX());
    vectorToFill.setY(0.0);
    vectorToFill.setZ(filterTheta.getX());
}
