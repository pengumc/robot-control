#ifndef __CSOLVER_H__
#define __CSOLVER_H__
#include <stdlib.h>
#include <stdint.h>

#include <math.h>
#include "gsl/gsl_vector.h"
#include "gsl/gsl_multiroots.h"

typedef struct PARAMS{
    double A;
    double B;
    double C;
    double X;
    double Y;
    double Z;
} solverParams_t;

class CSolver{
    public:
        CSolver();
        ~CSolver();
        int solveFor(double x, double y, double z, double betaGuess );
        int solveForFDF(double x, double y, double z);
        double alpha;
        double beta;
        double gamma;
        solverParams_t p;
//    private:
        int iter;
        int status;
        gsl_vector *x; //initialization vector
        static const size_t n=3;
        const gsl_multiroot_fsolver_type *T;
        gsl_multiroot_fsolver *s;
        gsl_multiroot_function f;
};


#endif

