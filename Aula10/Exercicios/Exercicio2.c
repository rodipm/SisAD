/*
tarefa1:X = A x B
tarefa2:Y = E x F
tarefa3:Z = C x D
tarefa4: W = X + Y + Z + W i imprime 10 primeiros elementos da linha 499 de W 
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "mede_time.h"
#include <mpi.h>

#define n 1000

int X[n][n], Y[n][n], Z[n][n], W[n][n];
int X_i[n], Y_i[n], Z_i[n];
int *buf;

void tarefa1()
{
    int i, j, k;
    for (j = 0; j < n; j++)
        X[0][j] = (n * 2 - j) % 10;

    // Envia linha 0 gerada para rank 3 (tarefa 4)
    MPI_Bsend(&X[0], n, MPI_INT, 3, 11, MPI_COMM_WORLD);

    for (i = 1; i < n; i++)
    {
        X[i][0] = (i * 2) % 10;

        for (j = 1; j < n; j++)
            for (k = 0; k < n; k++)
                X[i][j] = X[i][j - 1] * 3 - (X[i - 1][k] * (k + 4)) % 30;

        // Envia linha i gerada para rank 3 (tarefa 4)
        MPI_Bsend(&X[i], n, MPI_INT, 3, 11, MPI_COMM_WORLD);
    }

    printf("tarefa1 FIM\n");
    fflush(stdout);
}

void tarefa2()
{
    int i, j, k;
    for (j = 0; j < n; j++)
        Y[0][j] = (n * 3 - j) % 15;

    // Envia linha 0 gerada para rank 3 (tarefa 4)
    MPI_Bsend(&Y[0], n, MPI_INT, 3, 12, MPI_COMM_WORLD);

    for (i = 1; i < n; i++)
    {
        for (j = 1; j < n; j++)
            for (k = 0; k < n; k++)
                Y[i][j] = Y[i][j - 1] * 4 - (Y[i - 1][k] * (k + 5)) % 50;

        // Envia linha i gerada para rank 3 (tarefa 4)
        MPI_Bsend(&Y[i], n, MPI_INT, 3, 12, MPI_COMM_WORLD);
    }

    printf("tarefa2 FIM\n");
    fflush(stdout);
}

void tarefa3()
{
    int i, j, k;
    for (j = 0; j < n; j++)
        Z[0][j] = (n * 2 + j) % 20;

    // Envia linha 0 gerada para rank 3 (tarefa 4)
    MPI_Bsend(&Z[0], n, MPI_INT, 3, 13, MPI_COMM_WORLD);

    for (i = 1; i < n; i++)
    {
        for (j = 1; j < n; j++)
            for (k = 0; k < n; k++)
                Z[i][j] = Z[i][j - 1] * 2 - (Z[i - 1][k] * (k + 3)) % 40;

        // Envia linha i gerada para rank 3 (tarefa 4)
        MPI_Bsend(&Z[i], n, MPI_INT, 3, 13, MPI_COMM_WORLD);
    }

    printf("tarefa3 FIM\n");
    fflush(stdout);
}

void tarefa4()
{
    int i, j, k;
    MPI_Status status;

    for (i = 0; i < n; i++)
    {
        MPI_Recv(X_i, n, MPI_INT, 0, 11, MPI_COMM_WORLD, &status);
        MPI_Recv(Y_i, n, MPI_INT, 1, 12, MPI_COMM_WORLD, &status);
        MPI_Recv(Z_i, n, MPI_INT, 2, 13, MPI_COMM_WORLD, &status);

        // printf("X_i[20]=%d, Y_i[20]=%d, Z_i[20]=%d, iter=%d\n", X_i[20], Y_i[20], Z_i[20], i);
        fflush(stdout);

        for (j = 0; j < n; j++)
            W[i][j] = X_i[j] + Y_i[j] + Z_i[j];
    }

    printf("tarefa4 W[0][0]=%d W[300][400]=%d  W[999][999]=%d\n", W[0][0], W[300][400], W[999][999]);
    fflush(stdout);
    printf("tarefa4 FIM\n");
    fflush(stdout);
}

int main(int argc, char *argv[])
{
    int rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0)
        tarefa1();
    if (rank == 1)
        tarefa2();
    if (rank == 2)
        tarefa3();
    if (rank == 3) {
        TIMER_CLEAR;
        TIMER_START;
        int size = n * 4 + MPI_BSEND_OVERHEAD * 3;
        buf = malloc(size);
        MPI_Buffer_attach(buf, size);
        tarefa4();
        MPI_Buffer_detach(buf, &size);
        TIMER_STOP;
        printf("TEMPO=%f\n", TIMER_ELAPSED);
    }

    MPI_Finalize();
    return 0;
}