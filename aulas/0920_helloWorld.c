//
// Created by alang on 20/09/2022.
//
#include <stdio.h>
#include <omp.h>
int main(int argc, char** argv){
    double A[100];
    omp_set_num_threads(4);
    #pragma omp parallel {

        int ID = omp_get_thread_num();
        printf("hello (%d)", ID);
        printf("world (%d)", ID);
    }
    print("Fora das threads");
    return 0;
}
