/*
tarefa1: Calcula X
tarefa2: Calcula Y
tarefa3: Inicializa T e calcula W (W = X * T + Y)
*/

#include <stdio.h>
#include <sys/time.h>
#include <mpi.h>
#include "mede_time.h"

#define SIZE 1000

int X[SIZE][SIZE], Y[SIZE][SIZE], W[SIZE][SIZE];
int T[SIZE][SIZE];

MPI_Request req1 = MPI_REQUEST_NULL;
MPI_Request req2 = MPI_REQUEST_NULL;
MPI_Status status;

void tarefa1()
{
    int i, j, k;

    for (j = 0; j < SIZE; j++)
        X[0][j] = (SIZE * 2 - j) % 10;

    MPI_Isend(X[i], SIZE, MPI_INT, 2, 10, MPI_COMM_WORLD, &req1);
    MPI_Wait(&req1, &status);
    for (i = 1; i < SIZE; i++)
    {
        X[i][0] = (i * 2) % 10;

        for (j = 1; j < SIZE; j++)
            for (k = 0; k < SIZE; k++)
                X[i][j] = (X[i][j - 1] * 3 - (X[i - 1][k] * (k + 4)) % 30) % 100;

        MPI_Isend(X[i], SIZE, MPI_INT, 2, 10, MPI_COMM_WORLD, &req1);
        MPI_Wait(&req1, &status);
    }

    printf("X[0][0]=%d X[300][400]=%d  X[%d][%d]=%d\n", X[0][0], X[300][400], SIZE - 1, SIZE - 1, X[SIZE - 1][SIZE - 1]);
    fflush(stdout);
    printf("tarefa1 FIM\n");
    fflush(stdout);
}

void tarefa2()
{
    int i, j, k;
    for (j = 0; j < SIZE; j++)
        Y[0][j] = (SIZE * 3 - j) % 15;

    MPI_Isend(Y[i], SIZE, MPI_INT, 2, 20, MPI_COMM_WORLD, &req2);
    MPI_Wait(&req2, &status);

    for (i = 1; i < SIZE; i++)
    {
        Y[i][0] = (i * 3) % 15;

        for (j = 1; j < SIZE; j++)
            for (k = 0; k < SIZE; k++)
                Y[i][j] = (Y[i][j - 1] * 4 - (Y[i - 1][k] * (k + 5)) % 50) % 100;

        MPI_Isend(Y[i], SIZE, MPI_INT, 2, 20, MPI_COMM_WORLD, &req2);
        MPI_Wait(&req2, &status);
    }

    printf("Y[0][0]=%d Y[300][400]=%d  Y[%d][%d]=%d\n", Y[0][0], Y[300][400], SIZE - 1, SIZE - 1, Y[SIZE - 1][SIZE - 1]);
    fflush(stdout);
    printf("tarefa2 FIM\n");
    fflush(stdout);
}

void tarefa3()
{

    int i, j, k;

    for (i = 0; i < SIZE; i++)
        for (j = 0; j < SIZE; j++)
            W[i][j] = 0;

    for (i = 0; i < SIZE; i++)
        for (j = 0; j < SIZE; j++)
            T[i][j] = (SIZE * 3 - i + j) % 15;

    for (i = 0; i < SIZE; i++)
    {
        MPI_Irecv(X[i], SIZE, MPI_INT, 0, 10, MPI_COMM_WORLD, &req1);
        MPI_Irecv(Y[i], SIZE, MPI_INT, 1, 20, MPI_COMM_WORLD, &req2);

        MPI_Wait(&req1, &status);
        MPI_Wait(&req2, &status);
        
        for (j = 0; j < SIZE; j++)
            for (k = 0; k < SIZE; k++)
                W[i][j] = W[i][j] + X[i][k] * T[k][j];

        for (j = 0; j < SIZE; j++)
            W[i][j] = W[i][j] + Y[i][j];
    }
    printf("tarefa 3 W[0][0]=%d W[300][400]=%d  W[999][999]=%d\n", W[0][0], W[300][400], W[999][999]);
    fflush(stdout);
    printf("tarefa3 FIM\n");
    fflush(stdout);
}

int main(int argc, char *argv[])
{

    int n_nos, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &n_nos);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0)
        tarefa1();
    if (rank == 1)
        tarefa2();
    if (rank == 2)
    {
        TIMER_CLEAR;
        TIMER_START;
        tarefa3();
        TIMER_STOP;
        printf("TEMPO=%f\n", TIMER_ELAPSED);
        printf("TEMPO=%f\n", TIMER_ELAPSED);
    }

    MPI_Finalize();
    return 0;
}