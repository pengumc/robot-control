#ifndef __CACC__
#define __CACC__
#include "robot-control/KalmanFilter.h"
#include "robot-control/rotation.h"
#define ACC_MID 127

/** Class that holds the accelerometer data and filters
This class takes 3 adc measurements from the accelerometer
and converts them to tiltsensing in our xz-plane:
angle phi around the x-axis and
angle theta around the y-axis (that's the y-axis of the accelerometer,
our z-axis)
*/
class CAccelerometer{
    public:
        CAccelerometer(double Sz, double Sw);    
        double get_phi(); //around x-axis
        double get_theta(); //around y-axis
        KalmanFilter filterPhi;
        KalmanFilter filterTheta;
        void step(uint8_t adcx, uint8_t adcy, uint8_t adcz);
        void storeAnglesInVector(rot_vector_t* vectorToFill);
    private:
        uint8_t adc[3];
        double* _calcTilt();
};


#endif
