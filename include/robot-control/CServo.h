#ifndef __CSERVO_H__
#define __CSERVO_H__
#include <stdint.h>
#include <stdlib.h>
#include "robot-control/CAngle.h"


class CServo2{
    friend class CUsbDevice;
    public:
        CServo2();
        void reset();
        CAngle offset;
        double pulsewidthToAngle(); 
        double pulsewidthToAngle(uint8_t s); 
        uint8_t angleToPulsewidth();
        uint8_t angleToPulsewidth(double s);
        ///returns 1 if angle is valid
        uint8_t isValid(double s);
        void setAngle(double s);
        void changeAngle(double s);
        void mirrorZ();
        double getAngle();
        uint8_t getPW();
        void setPW(uint8_t p);
        void flipDirection();
    private:
        double K;
        uint8_t midPulse;
        uint8_t minPulse;
        uint8_t maxPulse;
        double direction;
        CAngle angle;
        uint8_t pw;
};
#endif

