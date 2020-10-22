#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "mede_time.h"

#define SIZE 1000
#define NUM_THREADS 16

double A[SIZE][SIZE];

/*inicia matriz*/
void inicia_matriz()
{
    int i, j;
    #pragma omp parallel num_threads(NUM_THREADS) shared(A) private(i, j)
    {
    #pragma omp for
        for (i = 0; i < SIZE; i++)
        {
            for (j = 0; j < SIZE; j++)
            {
                A[i][j] = (3*i-j)/2;
            }
        }
    }
}

/*soma com reduction*/
double soma_mat_reduction()
{
    int i, j;
    double soma = 0.0f;
    #pragma omp parallel num_threads(NUM_THREADS) shared(A) private(i, j) reduction(+:soma)
    {
        #pragma omp for
        for (i = 0; i < SIZE; i++)
            for (j = 0; j < SIZE; j++)
                soma += A[i][j];
    }
    return soma;
}

/*soma com seção crítica*/
double soma_mat_critical()
{
    int i, j;
    double soma = 0.0f;
    double soma_parcial = 0.0f;
    #pragma omp parallel num_threads(NUM_THREADS) shared(A, soma) private(i, j) firstprivate(soma_parcial)
    {
        #pragma omp for
        for (i = 0; i < SIZE; i++)
            for (j = 0; j < SIZE; j++)
                soma_parcial += A[i][j];

        #pragma omp critical
        {
            soma += soma_parcial;
        }
    }
    return soma;
}

void main(argc, argv) int argc;
char **argv;
{
    inicia_matriz();
    TIMER_CLEAR;
    TIMER_START;
    double soma = soma_mat_reduction();
    TIMER_STOP;
    printf("TEMPO REDUCTION [%d]: %12.7f\n",SIZE,TIMER_ELAPSED );
    printf("SOMA = %0.1lf\n", soma);

    TIMER_CLEAR;
    TIMER_START;
    soma = soma_mat_critical();
    TIMER_STOP;
    printf("TEMPO CRITICAL [%d]: %12.7f\n", SIZE, TIMER_ELAPSED);
    printf("SOMA = %0.1lf\n", soma);
}
