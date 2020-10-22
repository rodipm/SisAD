#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void main()
{
    int i, j = 0, total_sum = 0;

#pragma omp parallel num_threads(4) private(i) firstprivate(j) reduction(+:total_sum)
    {
        #pragma omp for
        for (i = 0; i < 100; i++)
        {
            j += 1;
        }
        total_sum = j;
        printf("Thread [%d] - j [%d] total_sum [%d] \n", omp_get_thread_num(), j, total_sum);
    }

    printf("total_sum pos [%d]", total_sum);
}
