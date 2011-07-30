#ifndef __CSOLVER2_H__
#define __CSOLVER2_H__
#include <stdlib.h>
#include <stdint.h>

#include <math.h>
#include "gsl/gsl_vector.h"
#include "gsl/gsl_multiroots.h"
#include "robot-control/rotation.h"

typedef struct PARAMS{
    double A;
    double B;
    double C;
    double X;
    double Y;
    double Z;
} solverParams2_t;



class CSolver2{
    public:
        CSolver2();
        ~CSolver2();
        rot_vector_t *resultVector;
        void printParams();
//    protected:
        int solveFor();
        solverParams2_t p;
    private:
        gsl_vector *x; //initialization vector
        static const size_t n=2;
        const gsl_multiroot_fsolver_type *T;
        gsl_multiroot_fsolver *s;
        gsl_multiroot_function f;
        double getAlphaFromZ();
        rot_vector_t *tempVector;
        friend int _trig_f2 (const gsl_vector *in, void *params, gsl_vector *out);
};
#endif
