//
// Created by alan gomes on 07/09/2022.
//
#include <stdio.h>
#include <mpi.h>
int main(int argc, char** argv) {
    // variables
    int my_rank;
    int p; // number_of_process
    float a = 0.0, b = 1.0; // range to calculate
    int n = 1024;// float number of trapezoids
    float h; // height
    float local_a, local_b; // trapezoid ends
    int local_n; // fx at n
    float integral; // integral
    float total; // total
    int source; // integral sender
    int dest = 0;// destination of integral (node 0)
    int tag = 200; // type of message (unique)
    MPI_Status status; // status

    // functions
    float calculate(float local_a, float local_b, int local_n, float h);
    // MPI starts
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    // more code
    h = (b - a) / n;
    local_n = n / p;
    local_a = a + my_rank * local_n * h;
    local_b = local_a + local_n * h;
    integral = calculate(local_a, local_b, local_n, h);
    // MPI program original without reduce
    if (my_rank == 0) {
        total = integral;
        for (source = 1; source < p; source++) {

            MPI_Recv(&integral, 1, MPI_FLOAT, source, tag,
                     MPI_COMM_WORLD, &status);
         total += integral;
        }
    } else{MPI_Send(&integral, 1, MPI_FLOAT, dest, tag, MPI_COMM_WORLD);}
    if (my_rank == 0){ printf("Resultado: %f\n", total);}
    MPI_Finalize();
    return 0;
}

float calculate(float local_a, float local_b, int local_n, float h){
    float integral;
    float x, i;
    float f(float x);
    integral = (f(local_a) + f(local_b)) / 2.0;
    x = local_a;
    for(i=1; i<=local_n; i++){
        x +=h;
        integral += f(x);
    }
    integral *= h;
    return integral;
}

float f(float x){
    float fx;
    fx = x*x;
    return fx;
}