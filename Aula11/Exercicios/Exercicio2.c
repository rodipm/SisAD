/*
    Obtém o valor máximo de uma matriz de inteiros gerados aleatoriamente
 */
#include <stdio.h>
#include <mpi.h>
#include <time.h>
#include <stdlib.h>

#define SIZE 1000
int A[SIZE][SIZE];

int main(int argc, char *argv[])
{
    int n, n_nos, rank;
    int k;
    int max_val_parcial, max_val;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &n_nos);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    n = SIZE;
    k = n / n_nos;

    //================= Divisao do vetor para todos os ranks ======================
    if (rank == 0)
    {
        srand(time(NULL));
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                A[i][j] = rand();

    }

    MPI_Scatter(A, k * n, MPI_INT, A, k * n, MPI_INT, 0, MPI_COMM_WORLD);

    //==============================================================================

    //============= Calculo do max_val parcial em cada rank ========================
    max_val_parcial = -1;
    int cur_val;

    for (int i = 0; i < k; i++)
        for (int j = 0; j < SIZE; j++)
        {
            cur_val = A[i][j];
            if (cur_val > max_val_parcial)
                max_val_parcial = cur_val;
        }

    printf("RANK[%d] - max_val_parcial=%d\n", rank, max_val_parcial);
    fflush(stdout);

    //=============================================================================

    //================ Recepcao dos max_val_parciais no rank 0 ====================
    MPI_Reduce(&max_val_parcial, &max_val, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
    if (rank == 0)
    {
        printf("\nRESULTADO=%d\n", max_val);
        fflush(stdout);
    }

    //===============================================================================
    MPI_Finalize();
    return 0;
}