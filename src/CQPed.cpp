#ifndef __CQPED__
#define __CQPED__
//quadraped class--------------------------------------------------------------
#define Q_LEGS 2
///quadraped device, currently in 2 leg mode.
class CQPed{
    public:
        CQPed(){reset();}
        ///reconnect and reset the entire thing.
        void reset_old();
        void reset();
        ///array of servos, 3 per leg.
        CServo2 servoArray[SERVOS];
        ///the usb helper.
        CUsbDevice usb;
        ///solver for x,y,z -> a,b,c
        CSolver solver[Q_LEGS];
        ///prints the x and y positions of all legs.
        void printPos();
        ///change the x and y position of the center body.
        void moveRelative(double X, double Y);
        ///send the servo states to the physical device.
        void sendToDev();
        ///read servo states from physical device.
        void readFromDev();
        ///change the angle of a single servo by a.
        void changeServo(uint8_t servo, double a);
        ///print the servo angles from memory.
        void printAngles();
        ///calculate the angles needed for the position specified by x[] and y[]
        uint8_t calcAngles(uint8_t leg, uint8_t upOrDown);
        ///chose the best solution and assign it to the servos
        void assignAngles(
            uint8_t s0, uint8_t s1, uint8_t s2, uint8_t leg);
        ///x positions per leg
        double x[2];
        ///y positions per leg
        double y[2];
        ///z positions per leg
        double z[2];
};

/** Most default values are hardcoded into this function.
*/
void CQPed::reset_old(){
    usb.connect();
    char i;
    for (i=0;i<BUFLEN_SERVO_DATA;i++){
        servoArray[i].reset();
    }
    servoArray[2].offset = -(PI/2);
    //servoArray[2].flipDirection();
    servoArray[2].setAngle(-PI/2);
    servoArray[4].offset = -PI;
    servoArray[4].setAngle(-PI);
    servoArray[5].offset = -(PI/2);
    //servoArray[5].flipDirection();
    servoArray[5].setAngle(-PI/2);
    x[0] = 9.5;
    x[1] = -8;
    y[0] = -5.5;
    y[1] = -5.5;
    z[0] = 0;
    z[1] = 0;
    //leg 0
    solver[0].p.A = 3;
    solver[0].p.B = 6.5;
    solver[0].p.C = 5.5;
    //leg 1
    solver[1].p.A = 3;
    solver[1].p.B = 5;
    solver[1].p.C = 5.5;
}

void CQPed::reset(){
    usb.connect();
    char i;
    for (i=0;i<BUFLEN_SERVO_DATA;i++){
        servoArray[i].reset();
    }
    servoArray[2].offset = -(PI/2);
    servoArray[2].setAngle(-PI/2);
    servoArray[2].flipDirection();
    servoArray[3].mirrorZ();
    servoArray[4].mirrorZ();
    servoArray[5].offset = -(PI/2);
    servoArray[5].mirrorZ();
    x[0] = 9.5;
    x[1] = -8;
    y[0] = -5.5;
    y[1] = -5.5;
    z[0] = 0;
    z[1] = 0;
    //leg 0
    solver[0].p.A = 3;
    solver[0].p.B = 6.5;
    solver[0].p.C = 5.5;
    //leg 1
    solver[1].p.A = 3;
    solver[1].p.B = 5;
    solver[1].p.C = 5.5;
}

void CQPed::assignAngles(uint8_t s0, uint8_t s1, uint8_t s2, uint8_t leg){
    if (leg%2==0){ 
        servoArray[s0].setAngle(solver[leg].alpha); 
        servoArray[s1].setAngle(solver[leg].beta); 
        servoArray[s2].setAngle(solver[leg].gamma);
    }else {
        servoArray[s0].setAngle(solver[leg].alpha-PI); 
        servoArray[s1].setAngle(PI - solver[leg].beta); 
        servoArray[s2].setAngle(solver[leg].gamma);
   }

}

void CQPed::moveRelative(double X, double Y){
    //TODO prevent movement on single leg fail
    x[0] += X;
    x[1] += X;
    y[0] += Y;
    y[1] += Y;
    uint8_t up = 0;
    if (x[0] > -solver[0].p.C ) up =1;
    printf("=== leg 0:\n");
    if (calcAngles(0, up)==0) assignAngles(0,1,2,0);
    else {//undo move
        x[0] -= X;
        x[1] -= X;
        y[0] -= Y;
        y[1] -= Y;
        return;
    }
    up = 1;
    if (x[1] > -solver[1].p.C ) up =0;
    printf("=== leg 1:\n");
    if (calcAngles(1,up)==0) assignAngles(3,4,5,1);
    else {//undo move
        x[0] -= X;
        x[1] -= X;
        y[0] -= Y;
        y[1] -= Y;
        return;
    }
    printPos();
}

void CQPed::printPos(){
    printf("x0 = %f\ny0 = %f\nx1 = %f\ny1 = %f\n",x[0],y[0],x[1],y[1]);
}

uint8_t CQPed::calcAngles(uint8_t leg, uint8_t upOrDown){
    double guess =-0.01;
    uint8_t temp;
    if (upOrDown) guess = 0.01;
    if (leg%2==0) temp= solver[leg].solveFor(x[leg], y[leg], z[leg], guess);
    else temp =  solver[leg].solveFor(-x[leg], y[leg], z[leg], guess);
    
    
    return temp;    
    
    //return poscalc.calculateAngles(x[leg],y[leg], z[leg], leg);
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
#endif
