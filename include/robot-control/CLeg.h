#ifndef __CLEG_H__
#define __CLEG_H__
#include <stdio.h>
#include <math.h>
#include "robot-control/CUsbDevice.h"
#include "robot-control/Defaults.h"
#include "robot-control/rotation.h"
#include "robot-control/CSolver_new.h"
#define LEG_DOF 3
#define LEG_READY 1
#define LEG_NOT_READY 0


class CLeg : public CSolver2{
    public:
        CLeg(CServo2 *servoSubSet, solverParams2_t *lengths, rot_vector_t *COBOffset);
        ~CLeg();
        void printServoAngles();
        void printPositions();
        void updatePos(); //TODO private
        double getX(uint8_t point);// 0-4 (4 = endpoint)
        double getY(uint8_t point);
        double getZ(uint8_t point);
        int moveEndPointTo(rot_vector_t *v);
        int relativeMoveEndPoint(rot_vector_t *v);
        int calcAndTest(rot_vector_t *v);

        int readyFlag; //1 = ready, 0= not ready
        //is set when calcAndTest returns succesfull
        //and a set of valid angles stored in the solver
        //is ready to be used. use commit() to use these angles

        void commit();
    private:
        ///physical position relative to the main body
        rot_vector_t *servoPos[LEG_DOF];
        ///physical position of the leg endpoitn to the main body
        rot_vector_t *endPoint;
        CServo2 *servos;
        int testAngles();
        void assignAngles();        
};

#endif
