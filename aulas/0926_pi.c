//
// Created by alang on 26/09/2022.
//
#include <stdio.h>
#include <omp.h>

static long num_steps = 100000;
double step;
#define NUM_THREADS 8

void main (){

    int i;
    double x, pi = 0.0;
    int nthreads = NUM_THREADS;
    double sum[nthreads];

    step = 1.0/(double) num_steps;
    omp_set_num_threads (NUM_THREADS);

#pragma omp parallel private(x)
    {
        int i, id, nthrds;
        id = omp_get_thread_num();
        nthrds = omp_get_num_threads();
        if (id == 0)nthreads = nthrds;
        for (i=id, sum[id]=0.0;i< num_steps;i=i+nthrds){
            x = (i+0.5)*step;
            sum[id]+= 4.0/(1.0+x*x);
        }

    }

    for( i=0; i < nthreads; i++){
        pi += sum[i]*step;
    }

    printf("O valor de pi é %f\n", pi);
}