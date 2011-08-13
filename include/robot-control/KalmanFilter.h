#ifndef __KALMAN_CPP__
#define __KALMAN_CPP__

class KalmanFilter{
    public:
        KalmanFilter();
        KalmanFilter(double newSz, double newSw);
        double getX();
        void step(double measurement);
        void setSz(double newSz);
        void setSw(double newSw);
        double getSz();
        double getSw();
    private:
        double x;
        double x_last;
        double P;
        double P_last;
        double Sz; //R
        double Sw; //Q
        void init();
};

#endif

