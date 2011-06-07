#ifndef __CSOLVER_CPP__
#define __CSOLVER_CPP__
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include <math.h>
#include "../include/gsl/gsl_vector.h"
#include "../include/gsl/gsl_multiroots.h"

typedef struct PARAMS{
    double A;
    double B;
    double C;
    double X;
    double Y;
    double Z;
} solverParams_t;

int _trig_f (const gsl_vector *in, void *params, gsl_vector *out){
    const double A = ((struct PARAMS*)params)->A;
    const double B = ((struct PARAMS*)params)->B;
    const double C = ((struct PARAMS*)params)->C;
    const double X = ((struct PARAMS*)params)->X;
    const double Y = ((struct PARAMS*)params)->Y;
    const double Z = ((struct PARAMS*)params)->Z;

    const double a = gsl_vector_get(in, 0);
    const double b = gsl_vector_get(in, 1);
    const double c = gsl_vector_get(in, 2);

    const double x = cos(a)*(A + (B * cos(b)) + (C * cos(c+b))) - X; 
    const double y = (sin(b) *B) + (sin(c+b) * C) -Y;
    const double z = sin(a)*(A + (B * cos(b)) + (C * cos(c+b))) - Z;

    gsl_vector_set(out, 0, x);
    gsl_vector_set(out, 1, y);
    gsl_vector_set(out, 2, z);

    return GSL_SUCCESS;

}

int _trig_df(const gsl_vector *in, void *params, gsl_matrix *J){
    //printf("df called\n");
    const double A = ((struct PARAMS*)params)->A;
    const double B = ((struct PARAMS*)params)->B;
    const double C = ((struct PARAMS*)params)->C;
    const double X = ((struct PARAMS*)params)->X;
    const double Y = ((struct PARAMS*)params)->Y;
    const double Z = ((struct PARAMS*)params)->Z;
    
    const double a = gsl_vector_get(in, 0);
    const double b = gsl_vector_get(in, 1);
    const double c = gsl_vector_get(in, 2);

    const double df00 = -sin(a)*(A+B*cos(b)+C*cos(c+b));
    const double df01 = cos(a)*(-B*sin(b)-C*sin(c+b));
    const double df02 = -cos(a)*C*sin(c+b);
    const double df10 = 0;
    const double df11 = B*cos(b)+C*cos(c+b);
    const double df12 = C*cos(c+b);
    const double df20 = cos(a)*(A+B*cos(b)+C*cos(c+b));
    const double df21 = sin(a)*(-B*sin(b)-C*sin(c+b));
    const double df22 = -sin(a)*C*sin(c+b);
    gsl_matrix_set(J, 0,0, df00);
    gsl_matrix_set(J, 0,1, df01);
    gsl_matrix_set(J, 0,2, df02);
    gsl_matrix_set(J, 1,0, df10);
    gsl_matrix_set(J, 1,1, df11);
    gsl_matrix_set(J, 1,2, df12);
    gsl_matrix_set(J, 2,0, df20);
    gsl_matrix_set(J, 2,1, df21);
    gsl_matrix_set(J, 2,2, df22);
    //printf("%f %f %f\n%f %f %f\n%f %f %f", df00,df01,df02,df10,df11,df12,df20,df21,df22);

    return GSL_SUCCESS;

}

int _trig_fdf(const gsl_vector *in, void *params, gsl_vector *out, gsl_matrix *J){
    _trig_f(in, params, out);
    _trig_df(in, params, J);
    return GSL_SUCCESS;
}

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
    //private:<F2>
        int iter;
        int status;
        gsl_vector *x; //initialization vector
        static const size_t n=3;
        const gsl_multiroot_fsolver_type *T;
        gsl_multiroot_fsolver *s;
        gsl_multiroot_function f;
        const gsl_multiroot_fdfsolver_type *fdf_T;
        gsl_multiroot_fdfsolver *fdf_s;
        gsl_multiroot_function_fdf fdf_f;
        gsl_matrix *M;
        


};
void sanitize(gsl_vector *v){
    int i;
    for(i=0;i<3;i++){
        while(gsl_vector_get(v, i) > M_PI){
            gsl_vector_set(v, i, gsl_vector_get(v,i)-M_PI*2);
        }
        while(gsl_vector_get(v, i) < -M_PI){
            gsl_vector_set(v, i, gsl_vector_get(v,i)+M_PI*2);
        }

    }
}

CSolver::CSolver(){
    T = gsl_multiroot_fsolver_hybrid;
    s = gsl_multiroot_fsolver_alloc(T,n);
    fdf_T = gsl_multiroot_fdfsolver_hybridj;
    fdf_s = gsl_multiroot_fdfsolver_alloc(fdf_T,n);
    p = {3, 6.5, 5.5, 0,0,0};//default values for testing
    x = gsl_vector_alloc(n);

}

CSolver::~CSolver(){
    gsl_multiroot_fsolver_free(s);
    gsl_multiroot_fdfsolver_free(fdf_s);
    gsl_vector_free(x);
}
int test_res(gsl_multiroot_fsolver *s, solverParams_t *params){
    gsl_vector *temp = gsl_vector_alloc(3);
    _trig_f(s->x, params, temp);
    double resx = gsl_vector_get(temp, 0) ;
    double resy = gsl_vector_get(temp, 1) ;
    double resz = gsl_vector_get(temp, 2) ;
    printf("residual: %f, %f, %f\n", resx, resy, resz);
    gsl_vector_free(temp);
}


int CSolver::solveFor(double X, double Y, double Z, double betaGuess){
    gsl_vector_set(x,0,0);
    gsl_vector_set(x,1,betaGuess);
    gsl_vector_set(x,2,0);
    p.X = X;
    p.Y = Y;
    p.Z = Z;
    f = {&_trig_f, n, &p};
    gsl_multiroot_fsolver_set(s, &f, x);
    iter =0;
    do{
        iter++;
        status = gsl_multiroot_fsolver_iterate(s);
        if (status) break;
        status = gsl_multiroot_test_residual(s->f, 1E-4);
    }while (status == GSL_CONTINUE && iter < 1000);
    //if(status != GSL_SUCCESS){
        //printf("status: %s\niter: %d\n", gsl_strerror(status), iter);
        //sanitize(s->x);
        printf("%f, %f, %f, %f, %s\n",
            gsl_vector_get(s->x,0),
            gsl_vector_get(s->x,1),
            gsl_vector_get(s->x,2),
            p.X,
            gsl_strerror(status)
        );
        test_res(s, &p);
    //}
    alpha = gsl_vector_get(s->x, 0);    
    beta= gsl_vector_get(s->x, 1);    
    gamma = gsl_vector_get(s->x, 2);    

    return status;
}

int CSolver::solveForFDF(double X, double Y, double Z){
    gsl_vector_set(x, 0, 0);
    gsl_vector_set(x, 1, 0.1);
    gsl_vector_set(x, 2, 0);
    p.X = X;
    p.Y = Y;
    p.Z = Z;
    fdf_f  = {&_trig_f, &_trig_df, &_trig_fdf, n, &p};
    gsl_multiroot_fdfsolver_set(fdf_s, &fdf_f, x);
    iter = 0;

    do{
        iter++;
        status = gsl_multiroot_fdfsolver_iterate(fdf_s);
        if (status) break;
        status = gsl_multiroot_test_residual(fdf_s->f, 1E-5);
    }while (status == GSL_CONTINUE && iter < 1000);
    if(status != GSL_SUCCESS){
        printf("status: %s\niter: %d\n", gsl_strerror(status), iter);
        printf("current pos: [%f , %f , %f]\n",
            gsl_vector_get(fdf_s->x,0),
            gsl_vector_get(fdf_s->x,1),
            gsl_vector_get(fdf_s->x,2)
        );
    }
    alpha = gsl_vector_get(fdf_s->x, 0);    
    beta= gsl_vector_get(fdf_s->x, 1);    
    gamma = gsl_vector_get(fdf_s->x, 2);    

    return status;
}

#endif
#ifndef __MAIN__
//testing
int main(int argc, char *argv[]){
    CSolver solver;
    double X = atof(argv[1]);
    double Y = atof(argv[2]);
    double Z = atof(argv[3]);
    const double guess= atof(argv[4]);
    int i;
    
    do{
        X+=0.2;
        i = solver.solveFor(X,Y,Z,guess);
    }while (i==0);
}
#endif
