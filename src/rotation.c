#include "robot-control/rotation.h"

void rot_free(void *x){
    free(x);
}


///alocate vector
rot_vector_t *rot_vector_alloc(){
    return (rot_vector_t*) calloc(ROT_MATRIX_DIMENSION, sizeof(rot_vector_t));
}

//set vector entry
void rot_vector_set(rot_vector_t *v, uint8_t n, rot_vector_t a){
    v[n] = a;
}

void rot_vector_setAll(rot_vector_t *v, rot_vector_t x, rot_vector_t y, rot_vector_t z){
    rot_vector_set(v, 0, x);
    rot_vector_set(v, 1, y);    
    rot_vector_set(v, 2, z);    
}

///change
void rot_vector_change(rot_vector_t *v, uint8_t n, rot_vector_t a){
    v[n]+=a;
}

void rot_vector_changeAll(rot_vector_t *v, rot_vector_t a, rot_vector_t b, rot_vector_t c){
    v[0] += a;
    v[1] += b;
    v[2] += c;
}

///copy
void rot_vector_copy(rot_vector_t *source, rot_vector_t *target){
    target[0] = source[0];
    target[1] = source[1];
    target[2] = source[2];
}

///get vector entry
rot_vector_t rot_vector_get(rot_vector_t *v, uint8_t n){
    return v[n];
}

///get lengths? unsused
double rot_vector_get_length_yz(rot_vector_t *v){
    return sqrt(pow(v[1],2) + pow(v[2],2));
}
double rot_vector_get_length_xz(rot_vector_t *v){
    return sqrt(pow(v[0],2) + pow(v[2],2));
}
double rot_vector_get_length(rot_vector_t *v){
    return sqrt(pow(v[0],2) + pow(v[2],2)+ pow(v[1],2));
}

///print vector
void rot_vector_print(rot_vector_t *v){
    printf("printing vector...\n%f, %f, %f\n\n", v[0], v[1], v[2]);
}

void rot_vector_add(rot_vector_t *v1, rot_vector_t *v2){
    v1[0] += v2[0];
    v1[1] += v2[1];
    v1[2] += v2[2];
}
void rot_vector_minus(rot_vector_t *v1, rot_vector_t *v2){
    v1[0] -= v2[0];
    v1[1] -= v2[1];
    v1[2] -= v2[2];
}

//allocate matrix
rot_matrix_t *rot_matrix_alloc(){
    return (rot_matrix_t*)calloc(ROT_MATRIX_SIZE, sizeof(rot_matrix_t));
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
//inverse matrix, manual //TODO LU decomposition
/*
a 0
b 1
c 2
d 3
e 4
f 5
g 6
h 7
k 8
*/
void rot_matrix_invert(rot_matrix_t *M, rot_matrix_t *result){
    const double A = M[4]*M[8] - M[5]*M[7];
    const double B = M[5]*M[6] - M[3]*M[8];
    const double C = M[3]*M[7] - M[4]*M[6];
    const double D = M[2]*M[7] - M[1]*M[8];
    const double E = M[0]*M[8] - M[2]*M[6];
    const double F = M[6]*M[1] - M[0]*M[7];
    const double G = M[1]*M[5] - M[2]*M[4];
    const double H = M[2]*M[3] - M[0]*M[5];
    const double K = M[0]*M[4] - M[1]*M[3];
    const double det = M[0]*A + M[1]*B + M[2]*C;
    
    result[0] = A;
    result[1] = D;
    result[2] = G;
    result[3] = B;
    result[4] = E;
    result[5] = H;
    result[6] = C;
    result[7] = F;
    result[8] = K;
    
    rot_matrix_scale(result, 1.0/det);
}


