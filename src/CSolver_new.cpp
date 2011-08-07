#include "robot-control/CSolver_new.h"

int _trig_f2 (const gsl_vector *in, void *params, gsl_vector *out){
    const double A = ((struct PARAMS*)params)->A;
    const double B = ((struct PARAMS*)params)->B;
    const double C = ((struct PARAMS*)params)->C;
    const double X = ((struct PARAMS*)params)->X;
    const double Y = ((struct PARAMS*)params)->Y;

    const double b = gsl_vector_get(in, 0);
    const double c = gsl_vector_get(in, 1);

    const double x =  A + (B * cos(b)) + (C * cos(c+b)) - X; 
    const double y = (sin(b) *B) + (sin(c+b) * C) -Y;

    gsl_vector_set(out, 0, x);
    gsl_vector_set(out, 1, y);

    return GSL_SUCCESS;
}


CSolver2::CSolver2(){
    T = gsl_multiroot_fsolver_hybrid;
    s = gsl_multiroot_fsolver_alloc(T,n);
    x = gsl_vector_alloc(n);
    resultVector = rot_vector_alloc();
    tempVector = rot_vector_alloc();

}

CSolver2::~CSolver2(){
    gsl_multiroot_fsolver_free(s);
    gsl_vector_free(x);
    rot_free(resultVector);
    rot_free(tempVector);
}

double CSolver2::getAlphaFromZ(){
    const double X = p.X;
    const double Z = p.Z;
    rot_vector_set(tempVector, 0, X);
    rot_vector_set(tempVector, 1, 0);
    rot_vector_set(tempVector, 2, Z);    

    double length = rot_vector_get_length(tempVector);
//    printf("length = %.4g\n", length);
    double alpha = asin(Z/length);
//    printf("asin(z/length) = %.4g\n", alpha);
    //switch X in params for length
    p.X = rot_vector_get_length(tempVector);

    //fix quadrant
    if (X < 0) {
//        printf("X smaller than 0\n");
        if(Z < 0) alpha = -M_PI - alpha;
        else alpha = M_PI - alpha;
    }
    return alpha;

}

//assume params are correct
int CSolver2::solveFor(){
    //setup beta
    double beta = 0.01;
    if(p.Y < p.C) beta *= -1.0;
    //if(p.B < p.C)   beta *= -1.0;
    gsl_vector_set(x, 0, beta);
    //calculate alpha, and set new X
    rot_vector_set(resultVector, 0, getAlphaFromZ());
//    printParams();
//    printf("beta guess: %.4g\n", gsl_vector_get(x,0));
    gsl_vector_set(x, 1,-M_PI/2);
    f = {&_trig_f2, n, &p};
    gsl_multiroot_fsolver_set(s, &f, x);
    int iter =0;
    int status=-1;
        do{
        iter++; 
        status = gsl_multiroot_fsolver_iterate(s);
        if (status) break;
        status = gsl_multiroot_test_residual(s->f, 1E-4);
    }while (status == GSL_CONTINUE && iter < 1000);
    if(status != GSL_SUCCESS){
        printf("status: %s\niter: %d\n", gsl_strerror(status), iter);
        printf("%.4g, %.4g, %s\n",
            gsl_vector_get(s->x,0),
            gsl_vector_get(s->x,1),
            gsl_strerror(status)
        );
    }
    rot_vector_set(resultVector, 1, gsl_vector_get(s->x, 0));
    rot_vector_set(resultVector, 2, gsl_vector_get(s->x, 1));

    return status;
}

void CSolver2::printParams(){
    printf("A %.4g\nB %.4g\nC %.4g\nX %.4g\nY %.4g\nZ %.4g\n",
    p.A, p.B, p.C, p.X, p.Y, p.Z);
}
