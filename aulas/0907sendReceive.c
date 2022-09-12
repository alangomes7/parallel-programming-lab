//
// Created by alan gomes on 05/09/2022.
//
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mpi.h>

int main(int argc, char** argv) {
    // variables
    int meu_rank, np, origem, destino, tag = 0;
    char msg[100];
    MPI_Status status;

    // MPI start
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &meu_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &np);

    // MPI program
    printf("Starting process %d \n", meu_rank);

    double start_time = MPI_Wtime();
    if(meu_rank != 0){
        sprintf(msg, "Process %d is alive", meu_rank);
        destino = 0;
        int tam = strlen (msg)+1;
        MPI_Send(msg, tam, MPI_CHAR, destino, tag, MPI_COMM_WORLD);
    }else // if meu_rank ==
        for (origem=1; origem<np;origem++) {
            MPI_Recv(msg, 100, MPI_CHAR, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &status);
            printf("%s\n", msg);
        }
    // print time execution
    double end_time = MPI_Wtime();
    double execution_time = end_time - start_time;
    printf("Execution time: %f \n", execution_time);

    // MPI finalize
    MPI_Finalize();
    return 0;
}