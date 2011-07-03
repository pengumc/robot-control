#include "robot-control/CQPed.h"

/** Most default values are hardcoded into this function.
*/

void CQPed::reset(){
    usb.connect();
    char i;
    for (i=0;i<BUFLEN_SERVO_DATA;i++){
        servoArray[i].reset();
    }
    //servo orientations
    servoArray[2].offset = -(PI/2);
    servoArray[2].flipDirection();
    servoArray[2].setAngle(-PI/2);
    servoArray[3].mirrorZ();
    servoArray[4].flipDirection();
    servoArray[5].offset = -(PI/2);
    servoArray[5].setAngle(-PI/2);
    //print start states
    for(i=0;i<QP_SERVOS/2;i++)  {
        printf("servo %d:\n",i);
        servoArray[i].printDetails();
    }
    //create leg instances
    solverParams2_t P;
    P.A = 3;
    P.B = 6.5;
    P.C = 5.5;
    V = rot_vector_alloc();

    rot_vector_setAll(V, 3, 0, 0);
    legs[0] = new CLeg(&servoArray[0], &P, V);

    rot_vector_setAll(V, -3, 0, 0);
    P.B = 5;
    legs[1] = new CLeg(&servoArray[3], &P, V);
}

CQPed::~CQPed(){
    rot_free(V);
    char i;
    for(i=0;i<QP_LEGS;i++) free(legs[i]);
}


///returns 0 if nothing was done, 1 otherwise
int CQPed::moveByStick(){
    //assume pcscon is filled with valid data
    char trigger = 0;
    int8_t temp;
    if(!pscon.getShoulderShapes(R1)){
    //move legs together
        //Rx
        temp = pscon.getRx();
        if(abs(temp) > QP_CONTROLLER_TRESHOLD){
            trigger = 1;
            changeAllLegs(-((float)temp)*QP_STICK_SPEED, 0, 0);
        }
        //Ry
        temp = pscon.getRy();
        if(abs(temp) > QP_CONTROLLER_TRESHOLD){
            trigger = 1;
            changeAllLegs(0, ((float)temp)*QP_STICK_SPEED, 0);
        }
    }else{
    //move legs per stick
        //Rx
        temp = pscon.getRx();
        if(abs(temp) > QP_CONTROLLER_TRESHOLD){
            trigger = 1;
            changeSingleLeg(0, -((float)temp)*QP_STICK_SPEED, 0, 0);
        }
        //Ry
        temp = pscon.getRy();
        if(abs(temp) > QP_CONTROLLER_TRESHOLD){
            trigger = 1;
            changeSingleLeg(0, 0, ((float)temp)*QP_STICK_SPEED, 0);
        }
        //Lx
        temp = pscon.getLx();
        if(abs(temp) > QP_CONTROLLER_TRESHOLD){
            trigger = 1;
            changeSingleLeg(1, -((float)temp)*QP_STICK_SPEED, 0, 0);
        }
        //Ly
        temp = pscon.getLy();
        if(abs(temp) > QP_CONTROLLER_TRESHOLD){
            trigger = 1;
            changeSingleLeg(1, 0, ((float)temp)*QP_STICK_SPEED, 0);
        }
    }
    
    return trigger;    
}

void CQPed::fillPSController(){
    if(usb.getData()){
        pscon.setData(
            usb.PSControllerDataBuffer[1],
            usb.PSControllerDataBuffer[2],
            usb.PSControllerDataBuffer[5],
            usb.PSControllerDataBuffer[6],
            usb.PSControllerDataBuffer[7],
            usb.PSControllerDataBuffer[8]
        );
    }
}

int8_t CQPed::getConnected(){
    return usb.connected;
}

double CQPed::getX(uint8_t leg){
    return legs[leg]->getX(3);
}
double CQPed::getY(uint8_t leg){

    return legs[leg]->getY(3);
}
double CQPed::getZ(uint8_t leg){
    return legs[leg]->getZ(3);
}

double CQPed::getAngle(uint8_t servo){
    return servoArray[servo].getAngle();
}

uint8_t CQPed::getPW(uint8_t servo){
    return servoArray[servo].getPW();
}    


int CQPed::changeSingleLeg(uint8_t leg, double X, double Y, double Z){
    rot_vector_setAll(V, X, Y, Z);
    legs[leg]->relativeMoveEndPoint(V);        
    if(legs[leg]->readyFlag) legs[leg]->commit();
}

int CQPed::changeAllLegs(double X, double Y, double Z){
    rot_vector_setAll(V, X, Y, Z);
    legs[0]->relativeMoveEndPoint(V);
    rot_vector_setAll(V, X, Y, Z);
    legs[1]->relativeMoveEndPoint(V);
    int success = 0;
    char i;
    for(i=0;i<QP_LEGS;i++) success += legs[i]->readyFlag;
    if(success == QP_LEGS) {
        for(i=0;i<QP_LEGS;i++) legs[i]->commit(); 
        return 0;
    }else{    
        return 1;
    }
}

void CQPed::printPos(){
    printf("x0 = % .2g\ny0 = % .2g\nx1 = % .2g\ny1 = % .2g\n",
    getX(0), getY(0), getX(1), getY(1));
}



void CQPed::printAngles(){
    char i;
    for (i=0;i<BUFLEN_SERVO_DATA;i++){
        printf("servo %d = %f\n",i,servoArray[i].getAngle());
    }
}

void CQPed::changeServo(uint8_t servo, double a){
    servoArray[servo].changeAngle(a);
    sendToDev();
}

void CQPed::sendToDev(){
    usb.sendServoData(servoArray);
}

void CQPed::readFromDev(){
    usb.readServoData(servoArray);
}

