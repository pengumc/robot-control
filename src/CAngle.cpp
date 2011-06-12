#ifndef __ANGLE_H__
#define __ANGLE_H__
#include "robot-control/CAngle.h"

CAngle::CAngle(double s){
    set(s);
}

void CAngle::flipY(){
    set(PI - a);
}

CAngle CAngle::operator- (double s){
    CAngle temp;
    temp = a - s;
    return temp;
}

CAngle CAngle::operator- (CAngle b){
    CAngle temp;
    temp = a - b.a;
    return temp;
}

CAngle CAngle::operator+ (double s){
    CAngle temp;
    temp = a + s;
    return temp;
}

CAngle CAngle::operator+ (CAngle b){
    CAngle temp;
    temp = a + b.a;
    return temp;
}

double CAngle::get(){
    return a;
}

/** returns 0 if test is not between min and max.<BR>
*   returns 1 otherwise.<BR><BR>
*   isBetween( -PI/2, PI/2, 0)  will return 1.<BR>
*   isBetween( PI/2, -PI/2, 0)  will return 0.<BR>
*   isBetween( -PI/2, PI/2, PI) will return 0.<BR>
*   isBetween( PI/2, -PI/2, PI) will return 1.<BR>
*/
char CAngle::isBetween(double min, double max, double test){
    //printf("%f > %f && %f < %f\n",test, min, test, max);
    min = anglize(min);
    max = anglize(max);
    test = anglize(test);
    //check if we the frame includes PI
    if( min < max){//easy case
        //printf("%f > %f && %f < %f\n",test, min, test, max);
        if ( test >= min && test <= max) return 1;
        else return 0;
    }else{//hard case
        double shift =  max + PI;
        min -= shift;
        test = anglize(test - shift);
        //printf("%f > %f && %f < %f\n",test, min, test, PI);
        if ( test >= min && test <= PI) return 1;
        else return 0;
    }
}

void CAngle::operator= (double s){
    set(s);
}

void CAngle::set(double s){
    a = anglize(s);
}

double CAngle::anglize(double s){
    while (s > PI) s -= 2*PI;
    while (s < -PI) s+= 2*PI;
    return s;
}

void CAngle::printAngle(){
    printf("angle = %f\n",a);
}

#endif
