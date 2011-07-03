#ifndef __CQPED__
#define __CQPED__
#include "robot-control/CPSController.h"
#include "robot-control/CUsbDevice.h"
#include "robot-control/Defaults.h"
#include "robot-control/CServo.h"
#include "robot-control/CLeg.h"
#include "robot-control/rotation.h"
#define QP_SERVOS 12 //6 unused, TODO should be changed to QP_LEGS*LEG_DOF
#define QP_LEGS 2
#define QP_CONTROLLER_TRESHOLD 32
#define QP_STICK_SPEED (0.2/128)




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
        ///print the servo angles from memory.
        void printAngles();
        double getAngle(uint8_t servo);
        uint8_t getPW(uint8_t servo);
        double getX(uint8_t leg);
        double getY(uint8_t leg);
        double getZ(uint8_t leg);
        void fillPSController(); 
        int moveByStick();
        //new and improved legs
        CLeg *legs[QP_LEGS];

    private:
        ///the usb helper.
        CUsbDevice usb;
        ///general purpose vector
        rot_vector_t *V;
        
};
#endif
