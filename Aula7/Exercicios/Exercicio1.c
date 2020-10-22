// Aula 06 - Ex1

#include <stdio.h>
#include <sys/time.h>
#include "mede_time.h"

#define SIZE 1000
#define THREADS 2
int i, j, k;
int a[SIZE][SIZE];
int b[SIZE][SIZE];
int c[SIZE][SIZE];
int d[SIZE][SIZE];
int ab[SIZE][SIZE];
int cd[SIZE][SIZE];
int r[SIZE][SIZE];

int main()
{
    TIMER_CLEAR;
    TIMER_START;

    #pragma omp parallel num_threads(THREADS)
    {
        #pragma omp parallel for num_threads(THREADS) shared(a, b, c, d, ab, cd, r) private(i, j)
        for (i = 0; i < SIZE; i++)
        {
            for (j = 0; j < SIZE; j++)
            {
                a[i][j] = (i * 3 + j * 2) % SIZE;
                b[i][j] = (i * 4 + j * 2) % SIZE;
                c[i][j] = (i * 5 + j * 3) % SIZE;
                d[i][j] = (i * 2 + j * 4) % SIZE;
                ab[i][j] = 0;
                cd[i][j] = 0;
                r[i][j] = 0;
            }
        }

        #pragma omp sections
        {
            #pragma omp section
            {
                #pragma omp parallel for num_threads(THREADS) shared(ab) private(i, j, k)
                for (i = 0; i < SIZE; i++)
                {
                    for (k = 0; k < SIZE; k++)
                        for (j = 0; j < SIZE; j++)
                            ab[i][j] = ab[i][j] + a[i][k] * b[k][j];
                }
            }

            #pragma omp section
            {
                #pragma omp parallel for num_threads(THREADS) shared(cd) private(i, j, k)
                for (i = 0; i < SIZE; i++)
                {
                    for (k = 0; k < SIZE; k++)
                        for (j = 0; j < SIZE; j++)
                            cd[i][j] = cd[i][j] + c[i][k] * d[k][j];
                }
            }
        }

        #pragma omp parallel for num_threads(THREADS) shared(r) private(i, j, k)
        for (i = 0; i < SIZE; i++)
            for (j = 0; j < SIZE; j++)
                r[i][j] = ab[i][j] + cd[i][j];
    }

    TIMER_STOP;
    printf("TEMPO [%d]: %12.7f\n", SIZE, TIMER_ELAPSED);
    printf("r[0][0] = %d\n", r[0][0]);
    printf("r[0][0] = %d\n", r[10][10]);
    printf("r[999][999] = %d\n", r[999][999]);

    return 0;
}