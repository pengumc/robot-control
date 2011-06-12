#ifndef __ANGLE_H__
#define __ANGLE_H__
#include <stdlib.h>
#ifndef PI
    #include <math.h>
    #define PI M_PI
#endif
//-----------------------------------------------------------------------------
using namespace std;


/// Brief this class holds an angle that ranges from -PI to PI.
/** setting an angle outside the range will automatically convert the angle
*   to one within the range:<BR>
*   CAngle Phi();<BR>
*   phi = 3; <BR>will result in a = 3.0<BR><BR>
*   phi = 4; <BR>will result in a = 4 - 2*PI = -2.28....
*/

class CAngle{
    public:
        ///create an angle at 0 rad.
        CAngle(){set(0);}
        ///create an angle at s rad.
        CAngle(double s);
        ///return the angle in rad.
        double get();
        ///print the angle in rad to stdout.
        void printAngle();
        ///mirror the angle across the y axis of the unity circle
        void flipY();
        ///check if test lies between min and max.
        char isBetween(double min, double max, double test);
        ///converts s to a value within the -PI..PI range.
        double anglize(double s);//seriously, i'm out of ideas here
        void operator= (double);
        CAngle operator- (double);
        CAngle operator- (CAngle);
        CAngle operator+ (double);
        CAngle operator+ (CAngle);
    private:
        ///the stored angle.
        double a;
        ///store s in a (after fixing for range).
        void set(double s);
};

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



//int main(int argc, char *argv[] ){
    //CAngle a(atof(argv[1]));
    //CAngle b(atof(argv[2]));
    //CAngle temp = a -1.0 ;
    //temp.printAngle();
//}

#endif
