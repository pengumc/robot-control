#ifndef __CQPED__
#define __CQPED__
#include "robot-control/CPSController.h"
#include "robot-control/CUsbDevice.h"
#include "robot-control/Defaults.h"
#include "robot-control/CServo.h"
#include "robot-control/CLeg.h"
#include "robot-control/rotation.h"
#define QP_SERVOS 12 //TODO should be changed to QP_LEGS*LEG_DOF
#define QP_LEGS 4
#define QP_CONTROLLER_TRESHOLD 32
#define QP_STICK_SPEED (0.2/128)
#define QP_BUTTON_SPEED (0.05)
#define KALMAN_TYPE double
typedef struct KALMAN{
  KALMAN_TYPE x;
  KALMAN_TYPE x_last;
  KALMAN_TYPE P;
  KALMAN_TYPE P_last;
  KALMAN_TYPE Sz;
  KALMAN_TYPE Sw;
} kalman_t;




//quadraped class--------------------------------------------------------------
///quadraped device, currently in 2 leg mode.
class CQPed{
    public:
        CQPed(){reset();}
        ~CQPed();
        ///reconnect and reset the entire thing.
        //TODO containts default values (laziness)
        void reset();

        ///returns the connection status of the usb helper
        int8_t getConnected(); 

        ///prints the x and y positions of all legs.
        void printPos();

        //move funtions
        int changeAllLegs(double X, double Y, double Z);
        int changeSingleLeg(uint8_t leg,double X, double Y, double Z);
        int moveSingleLegTo(uint8_t leg,double X, double Y, double Z);

        
        //get servo locations
        double getRelativeServoX(uint8_t leg, uint8_t servo);
        double getRelativeServoY(uint8_t leg, uint8_t servo);
        double getRelativeServoZ(uint8_t leg, uint8_t servo);
        void getRelativePos(rot_vector_t *returnVector, uint8_t leg, uint8_t point);
        void getAbsolutePos(rot_vector_t *returnVector, uint8_t leg, uint8_t point);        
        
        //rotation
        int changeMainBodyAngle(double xaxis, double yaxis, double zaxis);
        void getMainBodyRotation(rot_vector_t * returnVector);
        
        //usb and other
        void sendToDev();
        void readFromDev();
        CPSController pscon;
        void fillPSController();
        int getUsbData(); 

        ///change the angle of a single servo by a.
        void changeServo(uint8_t servo, double a);
        ///array of servos, 3 per leg.
        CServo2 servoArray[QP_SERVOS];
        ///print the servo angles from memory.
        void printAngles();
        double getAngle(uint8_t servo);
        uint8_t getPW(uint8_t servo);
        int moveByStick();
        int sequence();
        //TODO privatize
        CLeg *legs[QP_LEGS];
        
        //accelerometer
        uint8_t adc[2];
        uint8_t acc_mid[2]; 
        void fillADC();
        //X and Y as noted on accelerometer
        kalman_t filterX;
        kalman_t filterY;

    private:
        ///the usb helper.
        CUsbDevice usb;
        ///general purpose vector
        rot_vector_t *V;
        ///main body rotation angles
        rot_vector_t *mainBodyAngles;
        rot_vector_t *tempAngles;
        ///main body rotation matrix
        rot_matrix_t *mainBodyR;
        rot_matrix_t *inverseR;
        void flipTemp();
        uint8_t seq_index;
        uint8_t seq_dir;
        
        
};
#endif
