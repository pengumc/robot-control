#include "robot-control/CQPed.h"

/** Most default values are hardcoded into this function.
*/

void CQPed::reset(){
    usb.connect();
    char i;
    for (i=0;i<BUFLEN_SERVO_DATA;i++){
        servoArray[i].reset();
    }
    //servo orientation

    servoArray[2].offset = -(PI/2);
    servoArray[2].setAngle(-PI/2);
    servoArray[2].flipDirection();
    servoArray[3].mirrorZ();
//    servoArray[4].mirrorZ();
    servoArray[4].flipDirection();
    servoArray[5].offset = -(PI/2);
//    servoArray[5].mirrorZ();

    for(i=0;i<QP_SERVOS/2;i++)  {
        printf("servo %d:\n",i);
        servoArray[i].printDetails();
    }
    //start positions
    x[0] = 9.5;
    x[1] = -8;
    y[0] = -5.5;
    y[1] = -5.5;
    zAxis = 0;
    width=5.5;
    z[0] = 0;
    z[1] = 0;
    //leg 0
    lengths.A[0] = 3;
    lengths.B[0] = 6.5;
    lengths.C[0] = 5.5;
    //leg 1
    lengths.A[1] = 3;
    lengths.B[1] = 5;
    lengths.C[1] = 5.5;
    updateSolverParams();
    rot_vector_t *v = rot_vector_alloc();
    //new legs
    v[0] = 3;
    legs[0] = new CLeg(&servoArray[0], &solver[0].p, v);
    v[0] = -3;
    legs[1] = new CLeg(&servoArray[3], &solver[1].p, v);
    rot_free(v);
    
}
///returns 0 if nothing was done, 1 otherwise
int CQPed::moveByStick(){
    //assume pcscon is filled with valid data
    char trigger = 0;
    int8_t temp;
    if(pscon.getShoulderShapes(R1)){
    //move legs together
        //Rx
        temp = pscon.getRx();
        if(abs(temp) > QP_CONTROLLER_TRESHOLD){
            trigger = 1;
            moveRelative(-((float)temp)*QP_STICK_SPEED, 0, 0);
        }
        //Ry
        temp = pscon.getRy();
        if(abs(temp) > QP_CONTROLLER_TRESHOLD){
            trigger = 1;
            moveRelative(0, ((float)temp)*QP_STICK_SPEED, 0);
        }
    }else{
    //move legs per stick
        //Rx
        temp = pscon.getRx();
        if(abs(temp) > QP_CONTROLLER_TRESHOLD){
            trigger = 1;
            moveRelativeSingleLeg(0, -((float)temp)*QP_STICK_SPEED, 0, 0);
        }
        //Ry
        temp = pscon.getRy();
        if(abs(temp) > QP_CONTROLLER_TRESHOLD){
            trigger = 1;
            moveRelativeSingleLeg(0, 0, ((float)temp)*QP_STICK_SPEED, 0);
        }
        //Lx
        temp = pscon.getLx();
        if(abs(temp) > QP_CONTROLLER_TRESHOLD){
            trigger = 1;
            moveRelativeSingleLeg(1, -((float)temp)*QP_STICK_SPEED, 0, 0);
        }
        //Ly
        temp = pscon.getLy();
        if(abs(temp) > QP_CONTROLLER_TRESHOLD){
            trigger = 1;
            moveRelativeSingleLeg(1, 0, ((float)temp)*QP_STICK_SPEED, 0);
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

void CQPed::updateSolverParams(){
    uint8_t i;
    for (i=0;i<QP_LEGS;i++){
        solver[i].p.A = lengths.A[i];
        solver[i].p.B = lengths.B[i];
        solver[i].p.C = lengths.C[i];
    }
}

int8_t CQPed::getConnected(){
    return usb.connected;
}

double CQPed::getX(uint8_t leg){
    return x[leg];
}
double CQPed::getY(uint8_t leg){

    return y[leg];
}
double CQPed::getZ(uint8_t leg){

    return z[leg];
}
void CQPed::readPSController(){
    usb.getData();
}

double CQPed::getAngle(uint8_t servo){
    return servoArray[servo].getAngle();
}

uint8_t CQPed::getPW(uint8_t servo){
    return servoArray[servo].getPW();
}    


int CQPed::assignAngles(uint8_t s0, uint8_t s1, uint8_t s2, uint8_t leg){
    uint8_t valid=0;
    double a,b,c;
    if (leg%2==0){ 
        a = solver[leg].alpha;
        b = solver[leg].beta;
        c = solver[leg].gamma;
        valid = servoArray[s0].isValid(a) + servoArray[s1].isValid(b) + servoArray[s2].isValid(c);
        if (valid < 3) return 1;
        servoArray[s0].setAngle(solver[leg].alpha); 
        servoArray[s1].setAngle(solver[leg].beta); 
        servoArray[s2].setAngle(solver[leg].gamma);
    }else {
        a = PI - solver[leg].alpha;
        b = PI - solver[leg].beta;
        c = solver[leg].gamma;
        valid = servoArray[s0].isValid(a) + servoArray[s1].isValid(b) + servoArray[s2].isValid(c);
        if (valid < 3) return 1;
        servoArray[s0].setAngle(PI-solver[leg].alpha); 
        servoArray[s1].setAngle(PI - solver[leg].beta); 
        servoArray[s2].setAngle(solver[leg].gamma);
   }
   return 0;

}

int CQPed::moveRelativeSingleLeg(uint8_t leg, double X, double Y, double Z){
    x[leg] += X;
    y[leg] += Y;
    z[leg] += Z;
    if(calcAngles(leg)==0){
        if(assignAngles(leg*3, leg*3+1, leg*3+2, leg)==0) return 0;
//        else printf("assign failed for leg %d\n", leg);
    }//else printf("calcAngles failed for leg %d\n",leg);
    x[leg] -= X;
    y[leg] -= Y;
    z[leg] -= Z;
    return 1;
}

//returns 0 on success, 1 otherwise
int CQPed::moveRelative(double X, double Y, double Z){
    x[0] += X;
    x[1] += X;
    y[0] += Y;
    y[1] += Y;
    z[0] += Z;
    z[1] += Z;
    
    rot_vector_t *v = rot_vector_alloc();
    
    v[0] = x[0];
    v[1] = y[0];
    v[2] = z[0];
    legs[0]->moveEndPointTo(v);
    
    v[0] = x[1];
    v[1] = y[1];
    v[2] = z[1];
    printf("input leg 1: ");
    rot_vector_print(v);
    legs[1]->moveEndPointTo(v);
    rot_vector_print(legs[1]->resultVector);
    
    rot_free(v);
    legs[0]->updatePos();
    legs[1]->updatePos();
    
    
    int success = 0;
    success = calcAngles(0); 
    success += calcAngles(1);
    switch (success) {
    case 0:
//        if( assignAngles(0,1,2,0)) break;

//        if( assignAngles(3,4,5,1)) break;
        return 0;
    } //undo move
    x[0] -= X;
    x[1] -= X;
    y[0] -= Y;
    y[1] -= Y;
    z[0] -= Z;
    z[1] -= Z;
    return 1;
    //printf("success = %d\n",success);
}

void CQPed::printPos(){
    printf("x0 = %f\ny0 = %f\nx1 = %f\ny1 = %f\n",x[0],y[0],x[1],y[1]);
}

//returns 0 on success
uint8_t CQPed::calcAngles(uint8_t leg){
    double guess =0.01;
    uint8_t temp;
    if (x[leg] > solver[leg].p.C) guess = -0.01;
    if (leg%2==0) temp= solver[leg].solveFor(x[leg], y[leg], z[leg], guess);
    else temp =  solver[leg].solveFor(-x[leg], y[leg], z[leg], guess);
    return temp;    
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

