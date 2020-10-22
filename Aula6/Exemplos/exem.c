#include <stdio.h>
#include <omp.h>
int a, i;
void main()
{
    a = 12;
#pragma omp parallel num_threads(4) shared(a) private(i)
    {
        i = 2;
        printf("a = % d i = % d \n", a, i);
        i = i + 1;
        printf("a = % d i = % d \n", a, i);
    }
    printf("FIM\n");
}