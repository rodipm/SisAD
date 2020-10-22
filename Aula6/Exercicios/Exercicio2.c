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
                A[i][j] = (2 * i - j) % 1000;
            }
        }
    }
}

/*obtem numero de ocorrencias com reduction*/
int procura_reduction(valor) int valor;
{
    int i, j;
    int ocorrencias = 0;
    #pragma omp parallel num_threads(NUM_THREADS) shared(A) private(i, j) reduction(+:ocorrencias)
    {
        #pragma omp for
        for (i = 0; i < SIZE; i++)
            for (j = 0; j < SIZE; j++) 
                if (A[i][j] == valor)
                    ocorrencias++;
    }
    return ocorrencias;
}

/*obtem numero de ocorrencias com seção crítica*/
int procura_critical(valor)
    int valor;
{
    int i, j;
    int ocorrencias = 0.0f;
    int ocorrencias_locais = 0.0f;

    #pragma omp parallel num_threads(NUM_THREADS) shared(A, ocorrencias) firstprivate(ocorrencias_locais)  private(i, j)
    {
        #pragma omp for
        for (i = 0; i < SIZE; i++)
            for (j = 0; j < SIZE; j++) 
                if (A[i][j] == valor)
                    ocorrencias_locais++;

        #pragma omp critical
        {
            ocorrencias += ocorrencias_locais;
        }
    }
    return ocorrencias;
}

void main(argc, argv) int argc;
char **argv;
{
    inicia_matriz();

    int valor;
    int ocorrencias;

    printf("Digite um valor entre 0 e 1000: ");
    scanf("%d", &valor);

    TIMER_CLEAR;
    TIMER_START;
    ocorrencias = procura_reduction(valor);
    TIMER_STOP;

    
    printf("TEMPO REDUCTION [%d]: %12.7f\n", SIZE, TIMER_ELAPSED);
    printf("%d : %d\n", valor, ocorrencias);

    TIMER_CLEAR;
    TIMER_START;
    ocorrencias = procura_critical(valor);
    TIMER_STOP;

    printf("TEMPO CRITICAL [%d]: %12.7f\n", SIZE, TIMER_ELAPSED);
    printf("%d : %d\n", valor, ocorrencias);
}
