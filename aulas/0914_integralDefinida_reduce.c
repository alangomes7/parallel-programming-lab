// Paralelização de cálculo de integral definida usando MPI_Reduce
// MPI_Reduce(&integral, &total, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
// Compilação: mpicc propagate.c -o propagate
// Execução: mpirun -np 6 propagate

#include <stdio.h>
#include <string.h>
#include <mpi.h>

#define THREADS 6

int main(int argc, char** argv){
    int my_rank;
    int local_neighbours;
    int p; // número de processos
    int tag = 50;
    MPI_Status status;

    char message[100] = "Eae";
    char received[100];

    int neighbourhoods[THREADS][THREADS] = {
    {0,1,1,0,0,0},
    {1,0,1,1,1,0},
    {1,1,0,0,0,1},
    {0,1,0,0,0,0},
    {0,1,0,0,0,0},
    {0,0,1,0,0,0}
    };
    int count_neighbours(int rank) {
        int neighbours = 0;
        for (int i = 0; i < THREADS; i++) {
            if (neighbourhoods[rank][i]) {
                neighbours++;
            }
        }
    return neighbours;
    }
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    if (p != THREADS) {
    printf("Espera-se %d processos, não %d.\n", THREADS, p);
    return 1;
    }

    local_neighbours = count_neighbours(my_rank);

    if (my_rank == 0) {
     // enviando a todos meus vizinhos
     for (int i = 0; i < THREADS; i++) {
         if (neighbourhoods[my_rank][i]) {
            printf("Enviando de %d para %d...\n", my_rank, i);
            MPI_Send(message, strlen(message)+1, MPI_CHAR, i, tag, MPI_COMM_WORLD);
         }
    }
     // recebendo de todos meus vizinhos
     for (int i = 0; i < (local_neighbours - 1); i++) {
         MPI_Recv(received, 100, MPI_CHAR, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &status);
         printf("Recebi em %d um \"%s\" de %d.\n", my_rank, received, status.MPI_SOURCE);
     }
     } else {
        // recebendo de um vizinho qualquer
        MPI_Recv(received, 100, MPI_CHAR, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &status);
        printf("Recebi em %d um \"%s\" de %d.\n", my_rank, received, status.MPI_SOURCE);
        // encaminhando a todos os vizinhos
        for (int i = 0; i < THREADS; i++) {
            if (neighbourhoods[my_rank][i]) {
            printf("Enviando de %d para %d...\n", my_rank, i);
            MPI_Send(received, strlen(received)+1, MPI_CHAR, i, tag, MPI_COMM_WORLD);
            }
        }
    // recebendo dos vizinhos restantes
    for (int i = 0; i < (local_neighbours - 1); i++) {
        MPI_Recv(received, 100, MPI_CHAR, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &status);
        printf("Recebi em %d um \"%s\" de %d.\n", my_rank, received, status.MPI_SOURCE);
    }
}
    MPI_Finalize();
}