#include "CSolver.cpp"
#include "rotation.c"

#define LENGTH_B 6.5
#define LENGTH_C 5.5
#define MAX_ERROR 0.08

int main2(int argc, char *argv[]){
    rot_vector_t *v = rot_vector_alloc();
    const double X = atof(argv[1]);
    const double Y = atof(argv[2]);
    const double Z = atof(argv[3]);
    
    int i, solutions, previous_had_solution;
    double phi, error, previous_error;
    double answers[20];
    solutions=0;
    error = MAX_ERROR;
    for(i=0;i<314;i++){
        //optimalization check:
        //if we have already have 2 solutions and the previous angle had none, 
        //then we're done
        //----------------------------
        if((previous_had_solution==0) & (solutions > 1)){
            break;
        }
        //----------------------------
        
        phi = i*0.01;
        rot_vector_set(v,0,X + LENGTH_C*cos(phi));
        rot_vector_set(v,1,Y + LENGTH_C*sin(phi));
        previous_error=error;
        error = fabs(rot_vector_get_length(v)-LENGTH_B);
                    //is it close enough?
        if(error < MAX_ERROR) {
            //if the last angle also had a solution:
            if(previous_had_solution){
                //check if it's better than the last
                if(previous_error > error){
                    //better solution, replace it
                    answers[solutions-1] = phi;
                    printf("better than %.4g", previous_error);
                }
            }else{
                //new solution, add it
                answers[solutions] = phi;
                solutions++;
                printf("new");
            }
            previous_had_solution = 1;

        }else previous_had_solution = 0;
        printf("% #.4g => %.4g\n", phi, error);        
    }

    double b, c, temp; //angles
    for(i=0;i<solutions;i++){
        printf("solution %d = %# .4g\n",i, answers[i]);
        rot_vector_set(v,0,X + LENGTH_C*cos(answers[i]));
        rot_vector_set(v,1,Y + LENGTH_C*sin(answers[i]));
        rot_vector_print(v);
        temp = rot_vector_get(v, 1)/LENGTH_B;
        if(temp>1.0) temp = 1.0;
        if(temp<-1.0)temp = -1.0;
        b = asin(temp);
        printf("b  % .4g\n", b);
        c = -answers[i] - b;
        printf("c  % .4g\n", c);
    double finalX = cos(b) * LENGTH_B + cos(c+b) *LENGTH_C;
    double finalY = sin(b) * LENGTH_B + sin(c+b) *LENGTH_C;
    printf("final: % .2g % .2g\n", finalX, finalY);

    }

    //test
    rot_free(v);

}


int main(int argc, char *argv[]){
    CSolver solver;
    solver.p.A = 3; 
    solver.p.B = 6.5;
    solver.p.C = 5.5;
    gsl_vector_set(solver.x, 0, atof(argv[5]));
    solver.solveFor(atof(argv[1]), atof(argv[2]), atof(argv[3]), atof(argv[4]));
    
} 

