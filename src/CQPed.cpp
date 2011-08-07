#include "robot-control/CQPed.h"


void stepKalman(struct KALMAN *kal, KALMAN_TYPE measurement){
  KALMAN_TYPE P_temp, K, x_temp;
    //predict
    x_temp = kal->x_last;
    P_temp = kal->P_last + kal->Sw; //Q
    //update
    K = (1/(P_temp + kal->Sz)) * P_temp; //R
    kal->x = x_temp + K * (measurement - x_temp);
    kal->P = (1 - K) * P_temp;
    //save previous states
    kal->x_last = kal->x;
    kal->P_last = kal->P;
  
}

/** Most default values are hardcoded into this function.
*/

void CQPed::reset(){
    usb.connect();
    seq_index=0;
    char i;
    for (i=0;i<BUFLEN_SERVO_DATA;i++){
        servoArray[i].reset();
    }
    //servo orientations
/*
    servoArray[2].offset = -(PI/2);
    servoArray[2].flipDirection();
    servoArray[2].setAngle(-PI/2);
*/
    servoArray[0].flipDirection();
    servoArray[1].flipDirection();
    servoArray[2].offset = -(PI/2);
    servoArray[2].setAngle(-PI/2);

    servoArray[3].mirrorZ();
    servoArray[3].flipDirection();
    
    servoArray[5].offset = -(PI/2);
    servoArray[5].flipDirection();
    servoArray[5].setAngle(-PI/2);


    //second set
    servoArray[6].flipDirection();
    servoArray[8].offset = -(PI/2);
    servoArray[8].flipDirection();
    servoArray[8].setAngle(-PI/2);

    servoArray[9].mirrorZ();
    servoArray[9].flipDirection();
    servoArray[10].flipDirection();
    servoArray[11].offset = -(PI/2);
    servoArray[11].setAngle(-PI/2);


/*
    //print start states
    for(i=0;i<QP_SERVOS/2;i++)  {
        printf("servo %d:\n",i);
        servoArray[i].printDetails();
    }
*/    
    //create leg instances
    solverParams2_t P;
    P.A = 3.2;
    P.B = 6.1;
    P.C = 6.2;
    V = rot_vector_alloc(); //general purpose vector
    rot_vector_setAll(V, 3.8, 0, 5.7);
    legs[0] = new CLeg(&servoArray[0], &P, V);
    rot_vector_changeAll(V, P.A + P.C, -6, 0);
    legs[0]->setEndPoint(V);


    P.A = 3.2;
    P.B = 6.3;
    P.C = 6.1;
    rot_vector_setAll(V, -3.8, 0, 5.7);
    legs[1] = new CLeg(&servoArray[3], &P, V);
    rot_vector_changeAll(V, -P.A - P.C, -6, 0);
    legs[1]->setEndPoint(V);

    P.A = 3.1;
    P.B = 5.8;
    P.C = 6.3;
    rot_vector_setAll(V, 3.8, 0,-5.7);
    legs[2] = new CLeg(&servoArray[6], &P, V);
    rot_vector_changeAll(V, P.A + P.C, -6, 0);
    legs[2]->setEndPoint(V);

    P.A = 3.2;
    P.B = 5.6;
    P.C = 6.5;
    rot_vector_setAll(V, -3.8, 0, -5.7);
    legs[3] = new CLeg(&servoArray[9], &P, V);
    rot_vector_changeAll(V, -P.A - P.C, -6, 0);
    legs[3]->setEndPoint(V);

    for(i=0;i<QP_LEGS;i++) if(legs[i]->readyFlag) legs[i]->commit();   
    
    //rotation matrix
    mainBodyAngles = rot_vector_alloc();
    tempAngles = rot_vector_alloc();
    mainBodyR = rot_matrix_alloc();
    inverseR = rot_matrix_alloc();
    changeMainBodyAngle(0,0,0);
    //rot_matrix_print(mainBodyR);

    //kalman
    acc_mid[0] = 128;
    acc_mid[1] = 128;
	filterX.Sz = 0.5;
	filterX.Sw = 0.01;
	filterY.Sz = 0.5;
	filterY.Sw = 0.01;

}

CQPed::~CQPed(){
    rot_free(V);
    rot_free(mainBodyAngles);
    rot_free(tempAngles);
    rot_free(mainBodyR);
    rot_free(inverseR);
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
    mainBodyR = inverseR;
    inverseR = mt;
}


void CQPed::getRelativePos(rot_vector_t *returnVector, uint8_t leg, uint8_t point){
    legs[leg]->fillWithPos(returnVector, point);
}

void CQPed::getAbsolutePos(rot_vector_t *returnVector, uint8_t leg, uint8_t point){
    legs[leg]->fillWithPos(V, point);
    rot_matrix_dot_vector(mainBodyR, V, returnVector);
}

//0 on success
int CQPed::changeMainBodyAngle(double xaxis, double yaxis, double zaxis){
    //TODO rollback
    rot_vector_setAll(tempAngles, xaxis, yaxis, zaxis);
    rot_matrix_build_from_angles(mainBodyR, tempAngles);
    rot_matrix_invert(mainBodyR, inverseR);
    char i;
    char error = 0;
    for(i=0;i<QP_LEGS;i++){
        //move leg to back rotated point
        legs[i]->fillWithPos(V, LEG_ENDPOINT);
//        printf("leg %d endPoint: ",i);
//        rot_vector_print(V);
        rot_matrix_dot_vector(inverseR, V, V);
//        printf("leg %d endPoint rotated: ",i);
//        rot_vector_print(V);
        error += legs[i]->setEndPoint(V);
    }
    if(error==0){
        rot_vector_changeAll(mainBodyAngles, xaxis, yaxis, zaxis);
        printf("angles, "); rot_vector_print(mainBodyAngles);
        for(i=0;i<QP_LEGS;i++) if(legs[i]->readyFlag) legs[i]->commit();
    }else printf("Rotation failed\n");
    //setup matrices for further movement
    rot_matrix_build_from_angles(mainBodyR, mainBodyAngles);  
    rot_matrix_invert(mainBodyR, inverseR);    
    return error;
}

void CQPed::getMainBodyRotation(rot_vector_t *returnVector){
    rot_vector_copy(mainBodyAngles, returnVector);
}

///returns 0 if nothing was done, 1 otherwise
int CQPed::moveByStick(){
    //assume pcscon is filled with valid data
    char trigger = 0;
    int8_t temp;
    uint8_t mode = 0;
    if(pscon.getShoulderShapes(R1)) mode = 1;
    if(pscon.getShoulderShapes(R2) && mode == 0) mode = 3;
    if(pscon.getShoulderShapes(L1) && mode == 0) mode = 2;
    if(pscon.getShoulderShapes(L2) && mode == 0) mode = 4;
    
    switch(mode){
    case 0:
        //move legs together R = xz plane
        //in mode 0, dpad is rotation of main body
        temp = pscon.getRx();
        if(abs(temp) > QP_CONTROLLER_TRESHOLD){
            trigger = 1;
            changeAllLegs(-((float)temp)*QP_STICK_SPEED, 0, 0);
        }
        //Ry
        temp = pscon.getRy();
        if(abs(temp) > QP_CONTROLLER_TRESHOLD){
            trigger = 1;
            changeAllLegs(0, 0,((float)temp)*QP_STICK_SPEED);
        }
        //Ly
        temp = pscon.getLy();
        if(abs(temp) > QP_CONTROLLER_TRESHOLD){
            trigger = 1;
            changeAllLegs(0,((float)temp)*QP_STICK_SPEED,0);
        }
        //rotation, dpad
        if(pscon.getSSDpad(UP)) {
            trigger = 1;
            changeMainBodyAngle(0,0,QP_BUTTON_SPEED);
        }
        if(pscon.getSSDpad(DOWN)) {
            trigger = 1;
            changeMainBodyAngle(0,0,-QP_BUTTON_SPEED);
        }
        if(pscon.getSSDpad(LEFT)) {
            trigger = 1;
            changeMainBodyAngle(0,QP_BUTTON_SPEED,0);
        }
        if(pscon.getSSDpad(RIGHT)) {
            trigger = 1;
            changeMainBodyAngle(0,-QP_BUTTON_SPEED,0);
        }
        //reset
        if(pscon.getSSDpad(START)){
            trigger = 1;
            reset();
        }
        //sequence
        if(pscon.getSSDpad(SELECT)){
            trigger = 1;
            sequence();
        }
        break;
    default:
        //shoulder buttons designate leg
        //all movement *-1 to prevent confusion
        mode--;
        temp = pscon.getRx();
        if(abs(temp) > QP_CONTROLLER_TRESHOLD){
            trigger = 1;
            changeSingleLeg(mode, ((float)temp)*QP_STICK_SPEED, 0, 0);
        }
        //Ry
        temp = pscon.getRy();
        if(abs(temp) > QP_CONTROLLER_TRESHOLD){
            trigger = 1;
            changeSingleLeg(mode, 0, 0,-((float)temp)*QP_STICK_SPEED);
        }
        //Ly
        temp = pscon.getLy();
        if(abs(temp) > QP_CONTROLLER_TRESHOLD){
            trigger = 1;
            changeSingleLeg(mode, 0,-((float)temp)*QP_STICK_SPEED,0);
        }

        break;    
    }
    
    return trigger;    
}
int CQPed::getUsbData(){
    return usb.getData();
}

void CQPed::fillPSController(){
    if(usb.connected>0){
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

void CQPed::fillADC(){
	//filters
	adc[0] = usb.PSControllerDataBuffer[3];
	adc[1] = usb.PSControllerDataBuffer[4];
	stepKalman(&filterX, ((double)adc[0])-acc_mid[0] );
	stepKalman(&filterY, ((double)adc[1])-acc_mid[1] );
    double phi = asin(filterX.x/30);
    rot_vector_setAll(V, 0,0,phi);
    rot_vector_minus(V, mainBodyAngles);
    if(changeMainBodyAngle(V[0],V[1],V[2])==0);
    sendToDev();
//    printf("adc: %d, %d\n", adc[0], adc[1]);
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
    rot_matrix_dot_vector(inverseR, V,V);
    legs[leg]->relativeMoveEndPoint(V);        
    if(legs[leg]->readyFlag){
        legs[leg]->commit();
        return 0;
    } else return -1;
}

int CQPed::changeAllLegs(double X, double Y, double Z){
    char i;
    rot_vector_setAll(V, X, Y, Z);
    rot_matrix_dot_vector(inverseR, V,V);
    for(i=0;i<QP_LEGS;i++) legs[i]->relativeMoveEndPoint(V);
    int success = 0;
    for(i=0;i<QP_LEGS;i++) success += legs[i]->readyFlag;
    if(success == QP_LEGS) {
        for(i=0;i<QP_LEGS;i++) legs[i]->commit(); 
        return 0;
    }else{    
        //printf("fail %i\n", success);;
        return 1;
    }
}

void CQPed::printPos(){
    char i;
    printf("leg endpoints:\n");
    for(i=0;i<QP_LEGS;i++){
        printf("x%d = % .2g\ny%d = % .2g\nz%d = % .2g\n",
            i, getRelativeServoX(i, LEG_ENDPOINT),
            i, getRelativeServoY(i, LEG_ENDPOINT),
            i, getRelativeServoZ(i, LEG_ENDPOINT)
        );
        
    }
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

//------------
// SEQUENCE
//------------
int CQPed::sequence(){
    //leg 0 up->+z->down
    double stride = 4;
    double clearance = 4;
    #define SSPEED 5
    uint8_t seq_length = SSPEED*3+1;
    int result = 0;
    uint8_t leg = seq_index / seq_length;
    //ranzige fix
    switch(leg){
    case 1:
        leg=3;
        break;
    case 2:
        leg=1;
        break;
    case 3:
        leg=2;
        break;
    }        
        
    //actual sequence      
    switch(seq_index%seq_length){
    case SSPEED:
        result = changeSingleLeg(leg, 0,clearance,0);
        break;
    case SSPEED*2:
        result = changeSingleLeg(leg, 0,0,stride);
        break;
    case SSPEED*3:
        result = changeSingleLeg(leg, 0,-clearance,0);
        break;
    }
    if (result == 0 ) {
        seq_index++;
        result = changeAllLegs(0,0,-stride/(seq_length*QP_LEGS));
    }
    if(seq_index/seq_length == QP_LEGS) seq_index = 0;
    return result;
}


