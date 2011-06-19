#ifndef __CQPED__
#define __CQPED__
#include "robot-control/CPSController.h"
#include "robot-control/CUsbDevice.h"
#include "robot-control/CSolver.h"
#include "robot-control/Defaults.h"
#include "robot-control/CServo.h"
#include "robot-control/CLeg.h"
#include "robot-control/rotation.h"
#define QP_SERVOS 12 //6 unused, TODO should be changed to QP_LEGS*LEG_DOF
#define QP_LEGS 2
#define QP_CONTROLLER_TRESHOLD 32
#define QP_STICK_SPEED (0.2/128)



typedef struct STRUCT_LENGTHS{
    double A[QP_LEGS];
    double B[QP_LEGS];
    double C[QP_LEGS];
} qp_lengths_t;

typedef struct STRUCT_VECTOR{
    double x;
    double y;
    double z;
} qp_vector_t;

typedef struct STRUCT_PIVOTS{
    qp_vector_t s0[QP_LEGS];
    qp_vector_t s1[QP_LEGS];
    qp_vector_t s2[QP_LEGS];
} qp_pivots_t;

//quadraped class--------------------------------------------------------------
///quadraped device, currently in 2 leg mode.
class CQPed{
    public:
        CQPed(){reset();}
        ///reconnect and reset the entire thing.
        void reset();
        ///returns the connection status of the usb helper
        int8_t getConnected(); 
        ///prints the x and y positions of all legs.
        void printPos();
        ///change the x and y position of the center body.
        int moveRelative(double X, double Y, double Z);
        int moveRelativeSingleLeg(uint8_t leg, double X, double Y, double Z);
        ///object to store and parse playstation controler data
        CPSController pscon;
        ///send the servo states to the physical device.
        void sendToDev();
        ///read servo states from physical device.
        void readFromDev();
        ///read PS controller data
        void readPSController();
        ///change the angle of a single servo by a.
        void changeServo(uint8_t servo, double a);
        ///array of servos, 3 per leg.
        CServo2 servoArray[SERVOS];
        qp_lengths_t lengths;
        ///print the servo angles from memory.
        void printAngles();
        double getAngle(uint8_t servo);
        uint8_t getPW(uint8_t servo);
        double getX(uint8_t leg);
        double getY(uint8_t leg);
        double getZ(uint8_t leg);
        void updateSolverParams();
        void updatePivots();//TODO not done
        void fillPSController(); 
        int moveByStick();
        CLeg *legs[QP_LEGS];

    private:
        ///the usb helper.
        CUsbDevice usb;
        ///x positions per leg
        double x[QP_LEGS];
        ///y positions per leg
        double y[QP_LEGS];
        ///z positions per leg
        double z[QP_LEGS];
        ///rotation of the main body around the zAxis
        CAngle zAxis;
        ///width of the main body
        double width;
        ///chose the best solution and assign it to the servos, returns 1 on failure
        int assignAngles(
            uint8_t s0, uint8_t s1, uint8_t s2, uint8_t leg);
        ///solver for x,y,z -> a,b,c
        CSolver solver[QP_LEGS];
        ///calculate the angles needed for the position specified by x[] and y[]
        uint8_t calcAngles(uint8_t leg );
        
};
#endif
