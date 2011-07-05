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
    
    //rotation matrix
    mainBodyAngles = rot_vector_alloc();
    tempAngles = rot_vector_alloc();
    mainBodyR = rot_matrix_alloc();
    tempM = rot_matrix_alloc();
    rot_matrix_build_from_angles(mainBodyR, mainBodyAngles);
    rot_matrix_print(mainBodyR);
}

CQPed::~CQPed(){
    rot_free(V);
    rot_free(mainBodyAngles);
    rot_free(tempAngles);
    rot_free(mainBodyR);
    rot_free(tempM);
    char i;
    for(i=0;i<QP_LEGS;i++) free(legs[i]);
}

void CQPed::flipTemp(){
    rot_vector_t *vt;
    rot_matrix_t *mt;
    vt = mainBodyAngles;
    mainBodyAngles = tempAngles;
    tempAngles = vt;
    mt = mainBodyR;
    mainBodyR = tempM;
    tempM = mt;
}


void CQPed::getRelativePos(rot_vector_t *returnVector, uint8_t leg, uint8_t point){
    legs[leg]->fillWithPos(returnVector, point);
}

void CQPed::getAbsolutePos(rot_vector_t *returnVector, uint8_t leg, uint8_t point){
    legs[leg]->fillWithPos(V, point);
    rot_matrix_dot_vector(mainBodyR, V, returnVector);
}

void CQPed::changeMainBodyAngle(double xaxis, double yaxis, double zaxis){
    rot_vector_changeAll(mainBodyAngles, xaxis, yaxis, zaxis);
    rot_matrix_build_from_angles(mainBodyR, mainBodyAngles);
    
    
}

void CQPed::getMainBodyRotation(rot_vector_t *returnVector){
    rot_vector_copy(mainBodyAngles, returnVector);
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

//servo = 0..3 (3 = endPoint)
double CQPed::getRelativeServoX(uint8_t leg, uint8_t servo){
    return legs[leg]->getX(servo);
}

double CQPed::getRelativeServoY(uint8_t leg, uint8_t servo){
    return legs[leg]->getY(servo);
}

double CQPed::getRelativeServoZ(uint8_t leg, uint8_t servo){
    return legs[leg]->getZ(servo);
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
    getRelativeServoX(0, LEG_ENDPOINT),
    getRelativeServoY(0, LEG_ENDPOINT),
    getRelativeServoX(1, LEG_ENDPOINT),
    getRelativeServoY(1, LEG_ENDPOINT));
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

