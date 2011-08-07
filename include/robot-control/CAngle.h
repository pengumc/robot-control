#ifndef __CANGLE_H__
#define __CANGLE_H__
#include <stdio.h>
#include <stdlib.h>
#include "robot-control/Defaults.h"
//-----------------------------------------------------------------------------

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
#endif
