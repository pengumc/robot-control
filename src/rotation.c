#include "robot-control/rotation.h"

///alocate vector
rot_vector_t *rot_vector_alloc(){
    return (rot_vector_t*) calloc(ROT_MATRIX_DIMENSION, sizeof(rot_vector_t));
}

//allocate matrix
rot_matrix_t *rot_matrix_alloc(){
    return (rot_matrix_t*)calloc(ROT_MATRIX_SIZE, sizeof(rot_matrix_t));
}

//set vector entry
void rot_vector_set(rot_vector_t *v, uint8_t n, rot_vector_t a){
    v[n] = a;
}
 
double rot_vector_get_length_yz(rot_vector_t *v){
    return sqrt(pow(v[1],2) + pow(v[2],2));
}
double rot_vector_get_length_xz(rot_vector_t *v){
    return sqrt(pow(v[0],2) + pow(v[2],2));
}

///print vector
void rot_vector_print(rot_vector_t *v){
    printf("printing vector...\n%f, %f, %f\n\n", v[0], v[1], v[2]);
}
    
void rot_free(void *x){
    free(x);
}

//returns 1 on error
int rot_matrix_fill(rot_matrix_t *M, rot_matrix_t *array, size_t size){
    int s = size/sizeof(rot_matrix_t);
    if(s <= ROT_MATRIX_SIZE){
        char i;
        for (i=0; i<ROT_MATRIX_SIZE; i++){
            M[i] = array[i];
        }
    }else return 1;
    return 0;
}

void rot_matrix_print(rot_matrix_t *M){
    char i;
    printf("printing matrix...\n");
    for(i=0;i<ROT_MATRIX_DIMENSION; i++){
        printf("%f, %f, %f\n", M[i*3], M[i*3+1], M[i*3+2]);
    }    
    printf("\n");
}

void rot_matrix_scale(rot_matrix_t* M, double factor){
    char i;
    for(i=0;i<ROT_MATRIX_SIZE;i++){
        M[i] *= factor;
    }
}

void rot_matrix_set_all(rot_matrix_t *M, rot_matrix_t a){
    char i;
    for(i=0;i<ROT_MATRIX_SIZE;i++){
        M[i] = a;
    }
}

void rot_matrix_set(rot_matrix_t *M, uint8_t row, uint8_t col, rot_matrix_t a){
    M[row * ROT_MATRIX_DIMENSION + col] = a;
}

//matrix * vector = vector (3x3 * 3 only)
void rot_matrix_dot_vector(rot_matrix_t *M, rot_vector_t *x, rot_vector_t *y){
    const rot_vector_t i = M[0] * x[0] + M[1] * x[1] + M[2] * x[2];
    const rot_vector_t j = M[3] * x[0] + M[4] * x[1] + M[5] * x[2];    
    const rot_vector_t k = M[6] * x[0] + M[7] * x[1] + M[8] * x[2];    
    
    rot_vector_set(y, 0, i);   
    rot_vector_set(y, 1, j);
    rot_vector_set(y, 2, k);        
};

//create rotation matrix from angles
void rot_matrix_build_from_angles(rot_matrix_t *M, rot_vector_t *v ){
//[[1,0,0],[0,cos(theta), -sin(theta)],[0,sin(theta), cos(theta)]]
//[[cos(psi),0,-sin(psi)],[0,1,0],[sin(psi),0,cos(psi)]]
//[[cos(phi),-sin(phi), 0],[sin(phi),cos(phi),0],[0,0,1]]
    const double ctheta = cos(v[0]); //around x-axis
    const double cpsi = cos(v[1]); //y-axis
    const double cphi = cos(v[2]); //z-axis
    const double stheta = sin(v[0]);
    const double spsi = sin(v[1]); 
    const double sphi = sin(v[2]); 
    
    rot_matrix_set(M, 0, 0, cphi*cpsi);
    rot_matrix_set(M, 0, 1, -cpsi*sphi);
    rot_matrix_set(M, 0, 2, -spsi);

    rot_matrix_set(M, 1, 0, ctheta*sphi - cphi*stheta*spsi);
    rot_matrix_set(M, 1, 1, ctheta*cphi + stheta*sphi*spsi);
    rot_matrix_set(M, 1, 2, -cpsi*stheta);

    rot_matrix_set(M, 2, 0, stheta*sphi + ctheta*cphi*spsi);
    rot_matrix_set(M, 2, 1, cphi*stheta - ctheta*sphi*spsi);
    rot_matrix_set(M, 2, 2, ctheta*cpsi);
}



