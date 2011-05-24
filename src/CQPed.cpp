#ifndef __CQPED__
#define __CQPED__
//quadraped class--------------------------------------------------------------
#define Q_LEGS 2
///quadraped device, currently in 2 leg mode.
class CQPed{
    public:
        CQPed(){reset();}
        ///reconnect and reset the entire thing.
        void reset();
        ///the usb helper.
        CUsbDevice usb;
        ///prints the x and y positions of all legs.
        void printPos();
        ///change the x and y position of the center body.
        void moveRelative(double X, double Y, double Z);
        ///send the servo states to the physical device.
        void sendToDev();
        ///read servo states from physical device.
        void readFromDev();
        ///change the angle of a single servo by a.
        void changeServo(uint8_t servo, double a);
        ///print the servo angles from memory.
        void printAngles();
        ///x positions per leg
        double x[2];
        ///y positions per leg
        double y[2];
        ///z positions per leg
        double z[2];
        ///rotation of the main body around the zAxis
        CAngle zAxis;
        ///width of the main body
        double width;
        void determineRotation();
        void changeRotation(double xaxis, double yaxis, double zaxis);
    private:
        ///array of servos, 3 per leg.
        CServo2 servoArray[SERVOS];
        ///chose the best solution and assign it to the servos
        void assignAngles(
            uint8_t s0, uint8_t s1, uint8_t s2, uint8_t leg);
        ///solver for x,y,z -> a,b,c
        CSolver solver[Q_LEGS];
        ///calculate the angles needed for the position specified by x[] and y[]
        uint8_t calcAngles(uint8_t leg );
        
};

/** Most default values are hardcoded into this function.
*/
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
    zAxis = 0;
    width=5.5;
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

/** have a look at the positions of the legs and determine the rotations
* from them
*/
void CQPed::determineRotation(){
    zAxis = asin((y[1]-y[2])/width);
    //yAxis
    //xAxis
}

void CQPed::changeRotation(double xaxis, double yaxis, double zaxis){
    int success =0;
    zAxis = zAxis + zaxis; //about time to overload +=?
    double shiftY = sin(zAxis.get())*width/2;
    double shiftX = (acos(zAxis.get()))*width/2;//WRONG
    y[0] += shiftY;
    y[1] -= shiftY;
    x[0] += shiftX;
    x[1] -= shiftX;
    printPos();
    success = calcAngles(0) + calcAngles(1);
    if (success == 0) {
        assignAngles(0,1,2,0);
        assignAngles(3,4,5,1);
    }else{
        y[0] -= shiftY;
        y[1] += shiftY;
        x[0] -= shiftX;
        x[1] += shiftX;
        determineRotation();
    }


}

void CQPed::assignAngles(uint8_t s0, uint8_t s1, uint8_t s2, uint8_t leg){
    if (leg%2==0){ 
        servoArray[s0].setAngle(solver[leg].alpha); 
        servoArray[s1].setAngle(solver[leg].beta); 
        servoArray[s2].setAngle(solver[leg].gamma);
    }else {
        servoArray[s0].setAngle(PI-solver[leg].alpha); 
        servoArray[s1].setAngle(PI - solver[leg].beta); 
        servoArray[s2].setAngle(solver[leg].gamma);
   }

}

void CQPed::moveRelative(double X, double Y, double Z){
    //TODO prevent movement on single leg fail
    x[0] += X;
    x[1] += X;
    y[0] += Y;
    y[1] += Y;
    z[0] += Z;
    z[1] += Z;
    uint8_t up = 0;
    int success = 0;
    if (x[0] > -solver[0].p.C ) up =1;
    printf("=== leg 0:\n");
    success = calcAngles(0); 
    up = 1;
    if (x[1] > -solver[1].p.C ) up =0;
    printf("=== leg 1:\n");
    success += calcAngles(1);
    if (success==0) {
        assignAngles(0,1,2,0);
        assignAngles(3,4,5,1);
        printPos();
    }else {//undo move
        x[0] -= X;
        x[1] -= X;
        y[0] -= Y;
        y[1] -= Y;
        z[0] -= Z;
        z[1] -= Z;
    }
    printf("success = %d\n",success);
}

void CQPed::printPos(){
    printf("x0 = %f\ny0 = %f\nx1 = %f\ny1 = %f\n",x[0],y[0],x[1],y[1]);
}

uint8_t CQPed::calcAngles(uint8_t leg){
    double guess =-0.01;
    uint8_t temp;
    if (x[leg] > solver[leg].p.C) guess = 0.01;
    if (leg%2==0) temp= solver[leg].solveFor(x[leg], y[leg], z[leg], guess);
    else temp =  solver[leg].solveFor(-x[leg], y[leg], z[leg], -guess);
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
#endif
