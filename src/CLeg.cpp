#include "robot-control/CLeg.h"

///constructor
CLeg::CLeg(CServo2 *servoSubSet, solverParams_t *lengths, rot_vector_t *COBOffset){
    servos = servoSubSet;
    //store params
    p.A = lengths->A;
    p.B = lengths->B;
    p.C = lengths->C;
    //allocate vectors
    char i;
    for(i=0;i<LEG_DOF;i++) servoPos[i] = rot_vector_alloc();
    endPoint = rot_vector_alloc();
    ///set offset
    rot_vector_set(servoPos[0], 0, COBOffset[0]);
    rot_vector_set(servoPos[0], 1, COBOffset[1]);    
    rot_vector_set(servoPos[0], 2, COBOffset[2]);    
}

//decontructor
CLeg::~CLeg(){
    char i;
    for(i=0;i<LEG_DOF;i++) rot_free(servoPos[i]);
    rot_free(endPoint);
}

//returns 0 on success, nonsense otherwise
int CLeg::moveEndPointTo(rot_vector_t *v){
    p.X = rot_vector_get(v, 0);
    p.Y = rot_vector_get(v, 1);
    p.Z = rot_vector_get(v, 2);
    if (solveFor() ==0){    //solve and store in resultVector
        if (testAngles()==0){       //test angles from resultVector
            assignAngles();
            return 0;
        }
    }
    printf("resultVector: ");
    return -1;

}

int CLeg::testAngles(){
    int error = -LEG_DOF;
    char i;
    for(i=0;i<LEG_DOF;i++) error += servos[i].isValid(rot_vector_get(resultVector, i));
    return error;
}

void CLeg::assignAngles(){
    char i;
    for(i=0;i<LEG_DOF;i++) servos[i].setAngle(rot_vector_get(resultVector, i));
}


///return X of a position
double CLeg::getX(uint8_t point){
    if(point < LEG_DOF){
        return servoPos[point][0];
    }else return endPoint[0];
}

///return Y of a position
double CLeg::getY(uint8_t point){
    if(point < LEG_DOF){
        return servoPos[point][1];
    }else return endPoint[1];
}

///return Z of a position
double CLeg::getZ(uint8_t point){
    if(point < LEG_DOF){
        return servoPos[point][2];
    }else return endPoint[2];
}


///print angles
void CLeg::printServoAngles(){
    char i;
    for(i=0;i<LEG_DOF;i++) printf("servo %d: %d\n", i, servos[i].getPW());
}

void CLeg::printPositions(){
    char i;
    for(i=0;i<LEG_DOF;i++) {
        printf("servo %d ",i);
        rot_vector_print(servoPos[i]);
    }
    printf("endPoint ");
    rot_vector_print(endPoint);

}

///update positions, endpoint
void CLeg::updatePos(){
    rot_vector_t *x = rot_vector_alloc();
    rot_matrix_t *M = rot_matrix_alloc();
    rot_vector_t *angles = rot_vector_alloc();

    //servo 0 -> 1    (y-axis)
    angles[1] = servos[0].getAngle();
    rot_matrix_build_from_angles(M, angles);
    x[0] = p.A;
    rot_matrix_dot_vector(M, x, servoPos[1]);
    rot_vector_add(servoPos[1], servoPos[0]);

    //servo 1 -> 2 (z-axis)
    angles[2] += -servos[1].getAngle();
    rot_matrix_build_from_angles(M, angles);
    x[0] = p.B;
    rot_matrix_dot_vector(M, x, servoPos[2]);
    rot_vector_add(servoPos[2], servoPos[1]);

    //servo 2 -> endpoint (z-axis)
    angles[2] += -servos[2].getAngle(); 
    rot_matrix_build_from_angles(M, angles);
    x[0] = p.C;
    rot_matrix_dot_vector(M, x, endPoint);
    rot_vector_add(endPoint, servoPos[2]);

    //free memory
    rot_free(M);
    rot_free(x);
    rot_free(angles);
}
