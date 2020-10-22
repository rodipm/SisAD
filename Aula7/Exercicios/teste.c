#include <stdio.h>
#include <omp.h>
#include <sys/time.h>
#include "mede_time.h"

#define size 20000
int i, j;
int A[size], B[size], C[size];

omp_lock_t semA_empty;
omp_lock_t semA_full;
omp_lock_t semB_empty;
omp_lock_t semB_full;

int main()
{
    printf("INICIO\n");
    A[0] = 15;
    B[0] = 1;
    C[0] = 1;

    omp_init_lock(&semA_full);
    omp_init_lock(&semA_full);

    omp_set_lock(&semA_full);
    omp_set_lock(&semB_full);

    TIMER_CLEAR;
    TIMER_START;
#pragma omp parallel num_threads(8) private(i, j)
    {
#pragma omp sections
        {
#pragma omp section
            {
                for (i = 1; i < size; i++)
                {
                    for (j = 0; j < size; j++)
                    {
                        A[i] = (A[i - 1] + i * 3 + 15 + j) % 1000;
                    }
                    omp_unset_lock(&semA_full); //Produtor de A
                }
            }

#pragma omp section
            {
                for (i = 1; i < size; i++)
                {
                    omp_set_lock(&semA_full);  // Consumidor de A
                    for (j = 0; j < size; j++)
                    {
                        B[i] = (B[i - 1] * 3 + A[i - 1] + j) % 1000;
                    }
                    omp_unset_lock(&semB_full);  //Produtor de B
                }
            }

#pragma omp section
            {
                for (i = 1; i < size; i++)
                {
                    omp_set_lock(&semB_full); // Consumidor de B
                    for (j = 0; j < size; j++)
                    {
                        C[i] = (C[i - 1] + B[i - 1] * 2 + j) % 1000;
                    }
                }
            }
        }
    }
    TIMER_STOP;
    printf("TEMPO [%d]: %12.7f\n", size, TIMER_ELAPSED);
    printf("C[0]=%d  C[1]=%d  C[%d]=%d\n", C[0], C[1], (size - 1), C[size - 1]);
    fflush(stdout);
    printf("FIM\n");

    omp_destroy_lock(&semA_full);
    omp_destroy_lock(&semA_full);
}
