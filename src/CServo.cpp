//-----------------------------------------------------------------------------
#ifndef __CSERVO_CPP__
#define __CSERVO_CPP__
#include <stdint.h>
#include "CAngle.cpp"
#include <stdlib.h>
//#include "CUsbDevice.cpp"

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

CServo2::CServo2(){reset();}


void CServo2::reset(){
    minPulse = 48;
    maxPulse = 96;
    midPulse = (minPulse + maxPulse)/2;
    direction=1.0;
    offset = 0;
    K = 0.034;
    angle = 0.0;
    pw = midPulse;
    //printf("servo created:\nmid %d\npw %d\nangle %f\n", midPulse, pw, angle.get());
}

double CServo2::getAngle(){
    return angle.get();
}

uint8_t CServo2::getPW(){
    return pw;
}

void CServo2::flipDirection(){
    direction *=-1.0;
    const uint8_t x = minPulse;
    minPulse = maxPulse;
    maxPulse = x;
}
void CServo2::mirrorZ(){
    //flipDirection();
    offset.flipY();
    angle = pulsewidthToAngle();
}

uint8_t CServo2::angleToPulsewidth(){
    double s = angle.anglize(angle.get()-offset.get());
    return (((s) / K) / direction) + midPulse;
}


uint8_t CServo2::angleToPulsewidth(double s){
    s = angle.anglize(s-offset.get());
    return (((s) / K) / direction) + midPulse;
}

double CServo2::pulsewidthToAngle(){
    return (pw - (midPulse)) * direction * K + offset.get();
    
}

double CServo2::pulsewidthToAngle(uint8_t s){
    return ((s - midPulse) * direction * K + offset.get());

}



uint8_t CServo2::isValid(double s){
    return angle.isBetween(
        pulsewidthToAngle(minPulse),
        pulsewidthToAngle(maxPulse),
        s);
}

void CServo2::setAngle(double s){
    if (isValid(s)) {
        angle = s;
        pw = angleToPulsewidth();
    }
}

void CServo2::setPW(uint8_t p){
    if (isValid(pulsewidthToAngle(p))){
        pw = p;
        angle = pulsewidthToAngle(p);
    }
}

void CServo2::changeAngle(double s){
    s = angle.get() +s;
    if (isValid(s)) {
        angle = s;
        pw = angleToPulsewidth();
   }
}

#ifndef __MAIN__
//testing
int main(int argc, char *argv[]){
    CServo2 S;
    S.offset =-(PI/2);
    S.setAngle(-PI/2);
    S.flipDirection();
    S.changeAngle(atof(argv[1]));
    printf("pw %d\nangle %f\n", S.getPW(), S.getAngle());
}

#endif
#endif
